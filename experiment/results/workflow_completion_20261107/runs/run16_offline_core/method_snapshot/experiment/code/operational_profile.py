"""Disclosed benchmark conditions, stored separately from raw dataset requirements."""
import re

VERSION='operational-acceptance-v4'


def acceptance_profile(source):
    conditions=[
        'Explicit original requirements and supplied circuit connections override every default below.',
        'Where no electrical wiring is supplied, individual LEDs are active HIGH to GND and buttons connect the named input to GND with INPUT_PULLUP; model pressed values are normalized independently of electrical level.',
        'When unspecified, a counter starts at zero and an LED starts OFF. An explicitly required ON state takes precedence.',
        'When the original gives no reaction deadline, a digital/button/serial command, PWM duty command or servo ANGLE COMMAND must update within 250ms of its input event becoming available. This does not impose physical motor travel time, shorten an explicit delay, or change a specified sensor/report period.',
        'When the original gives no display refresh period or response deadline, a changed input must be reflected in the display within 250ms. Explicit original display periods, delays and startup sequences override this default. This additional display bound was explicitly authorized for this experimental variant.',
        'Where the requirement asks to continuously stream a Boolean button/switch state over Serial but gives no repeat interval, respond to a changed state within 250ms and emit its correct state message at least once in every 500ms stable-state interval after settling. Explicit original report periods and event-only reporting rules override this default. This authorized additional continuous-state-report condition does not apply to startup-only messages, command echoes, sensor sampling or persistent display contents. A display retaining the correct visible state requires no repeated print calls unless the source explicitly requires redraws.',
        'Unless the original specifies a warmup or delayed startup, setup() must return within 500ms. Deadlines stated after initialization are measured from the observed setup() return, not power-on.',
        'An unspecified continuous DHT report must eventually emit a fresh valid reading within 3000ms of that measurement becoming available; explicit reporting periods override this bound.',
    ]
    if re.fullmatch(r'读取连接在\s*A\d+\s*引脚的电位器，根据数值控制\s*D\d+\s*PWM\s*LED\s*的亮度。?',source.strip(),re.I):
        conditions.append('For this otherwise unspecified potentiometer dimmer, the accepted transfer is floor(ADC*255/1023), raw ADC 0..1023 to PWM 0..255, tolerance 1 PWM count after settling for 250ms. Repeated equal inputs give the same settled duty.')
    return '\nOperational acceptance profile '+VERSION+' (additional public experimental conditions, not raw dataset text):\n'+'\n'.join('- '+x for x in conditions)
