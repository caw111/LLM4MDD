#pragma once
namespace pilot {
struct Stimulus{uint64_t at;int pin;double value;int kind;};
struct SerialInput{uint64_t at;std::string text;};struct Ultra{int trigger,echo;};
struct DirectDisplay{std::string name;int pins[7];bool anode;};
struct ShiftDisplay{std::string name;int reg;int bits[7];bool anode;};
struct Bus{int data,clock,latch,registers;uint64_t bits;std::vector<ShiftDisplay>displays;};
struct MuxSample{uint64_t at;unsigned channel;int value;};
struct Multiplexer{int signal,select[4];std::vector<MuxSample>samples;};
struct ImuSample{uint64_t at;float acceleration[3],gyro[3];int raw_y;};
inline std::vector<Stimulus> inputs={};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={{"sevseg1",{2,3,4,5,6,7,8},false}};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=12500;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={{0,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},0},{600,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},15000},{1200,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},15001},{1900,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},25000},{2500,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},2000},{3100,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},20000},{3700,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},1999},{4300,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},-32768},{5000,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},-10000},{5700,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},0},{6200,{0.00000000f,0.00000000f,0.00000000f},{0.00000000f,0.00000000f,0.00000000f},-18000}};
}
