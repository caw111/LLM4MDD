"""Read real PICSimLab ILI9341 pixels and recognize the built-in GFX bitmap font.

Expected strings are deliberately not inputs to the recognizer.
"""
from array import array
from collections import Counter
from functools import reduce
import gzip
import math
from pathlib import Path
import re

from PIL import Image,ImageOps


def glyphs():
    path=Path.home()/'.cache/llm4mdd-arduino/native_user/libraries/Adafruit_GFX_Library/glcdfont.c'
    text=path.read_text();start=text.index('{',text.index('font[]'));end=text.index('}',start)
    text=re.sub(r'/\*.*?\*/|//[^\n]*','',text[start+1:end],flags=re.S)
    values=[int(x,0) for x in re.findall(r'0x[0-9a-fA-F]+|\b\d+\b',text)]
    assert len(values)>=128*5
    return {tuple(values[i*5:i*5+5]):chr(i) for i in range(32,127)}


def read_frames(path,frequency=16000000):
    with gzip.open(path,'rb') as file:
        while True:
            header=file.readline()
            if not header:return
            fields=header.decode('ascii').split();assert fields[0]=='L',fields
            raw=file.read(240*320*4)
            if len(raw)!=240*320*4:raise ValueError('Incomplete native LCD pixel frame')
            pixels=array('I');pixels.frombytes(raw)
            image=Image.frombytes('RGB',(240,320),raw,'raw','BGRX')
            yield dict(at_ms=int(fields[1])*1000/frequency,mac=int(fields[2]),initialized=bool(int(fields[3])),
                       cs=int(fields[4]),dc=int(fields[5]),rst=int(fields[6])),image,pixels


def recognize(image,font):
    background=Counter(image.getdata()).most_common(1)[0][0]
    mask=Image.new('1',image.size)
    mask.putdata([pixel!=background for pixel in image.getdata()])
    w,h=image.size;pix=mask.load();bands=[];begin=None
    for y in range(h+1):
        occupied=y<h and mask.crop((0,y,w,y+1)).getbbox() is not None
        if occupied and begin is None:begin=y
        if not occupied and begin is not None:bands.append((begin,y));begin=None
    result=[]
    for top,bottom in bands:
        if bottom-top>80:continue
        bbox=mask.crop((0,top,w,bottom)).getbbox()
        if not bbox:continue
        left,right=bbox[0],bbox[2]
        runs=[]
        for y in range(top,bottom):
            start=None
            for x in range(left,right+1):
                on=x<right and pix[x,y]
                if on and start is None:start=x
                if not on and start is not None:runs.append(x-start);start=None
        gcd=reduce(math.gcd,runs,0)
        sizes=[s for s in range(1,9) if (not gcd or gcd%s==0) and 4*s<=bottom-top<=8*s]
        best=None
        for size in sizes:
            for dy in range(3*size+1):
                y0=top-dy
                if y0+8*size<bottom:continue
                for dx in range(5*size+1):
                    x0=left-dx
                    text=[];known=0;unknown=0
                    n=max(1,math.ceil((right-x0)/(6*size)))
                    for c in range(n):
                        columns=[]
                        for col in range(5):
                            bits=0
                            for row in range(8):
                                x=x0+(6*c+col)*size+size//2;y=y0+row*size+size//2
                                if 0<=x<w and 0<=y<h and pix[x,y]:bits|=1<<row
                            columns.append(bits)
                        ch=font.get(tuple(columns),'?')
                        if ch=='?':unknown+=1
                        elif ch!=' ':known+=1
                        text.append(ch)
                    text=''.join(text).rstrip()
                    score=known-unknown*3
                    proposal=(score,known,-unknown,-abs(y0),-abs(x0),size,text,x0,y0)
                    if best is None or proposal[:6]>best[:6]:best=proposal
        if best and best[1]>=2 and best[0]>0:
            _,known,neg_unknown,_,_,size,text,x0,y0=best
            if -neg_unknown>max(1,len(text)//5):continue
            ink=[]
            for y in range(max(0,y0),min(h,y0+8*size)):
                for x in range(max(0,x0),min(w,x0+len(text)*6*size)):
                    value=image.getpixel((x,y))
                    if value!=background:ink.append(value)
            color=Counter(ink).most_common(1)[0][0] if ink else (255,255,255)
            def rgb565(rgb):return ((rgb[0]>>3)<<11)|((rgb[1]>>2)<<5)|(rgb[2]>>3)
            result.append(dict(text=text,x=max(0,x0),y=max(0,y0),size=size,color=rgb565(color),background=rgb565(background),recognized_characters=known))
    return result


def observe(path,preview_dir=None):
    font=glyphs();events=[];preview_saved=False
    for metadata,image,pixels in read_frames(path):
        if not metadata['initialized']:continue
        options=[]
        for rotation in range(4):
            rotated=image.rotate(rotation*90,expand=True)
            for flip in (False,True):
                oriented=ImageOps.mirror(rotated) if flip else rotated
                lines=recognize(oriented,font)
                score=sum(line['recognized_characters']-3*line['text'].count('?') for line in lines)
                options.append((score,lines,oriented))
        score,lines,oriented=max(options,key=lambda x:x[0])
        if score<2:continue
        rotation_map={0x48:0,0x28:1,0x88:2,0xe8:3}
        for line in lines:
            events.append(dict(type='lcd',observation='native_pixel_font_decode',rotation=rotation_map.get(metadata['mac'],None),**metadata,**line))
        if preview_dir and not preview_saved:
            directory=Path(preview_dir);directory.mkdir(parents=True,exist_ok=True)
            oriented.save(directory/'lcd_actual.png');preview_saved=True
    return events


def self_check():
    font=glyphs();reverse={char:columns for columns,char in font.items()}
    image=Image.new('RGB',(160,40),'black')
    for i,ch in enumerate('Hello 123'):
        for x,column in enumerate(reverse[ch]):
            for y in range(8):
                if column&(1<<y):
                    for dx in range(2):
                        for dy in range(2):image.putpixel((i*12+x*2+dx,y*2+dy),(255,255,255))
    lines=recognize(image,font)
    assert any(line['text']=='Hello 123' and line['size']==2 for line in lines),lines
    print('Bitmap font recognition control passed')


if __name__=='__main__':self_check()
