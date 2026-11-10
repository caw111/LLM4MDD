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
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=16000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={{0,{1.25000000f,-2.50000000f,7.75000000f},{0.00000000f,0.00000000f,0.00000000f},0},{3000,{-6.50000000f,3.25000000f,-1.75000000f},{0.20000000f,-0.30000000f,0.40000000f},0},{6000,{0.00000000f,0.00000000f,0.00000000f},{-0.40000000f,0.10000000f,0.30000000f},0},{9000,{7.00000000f,-4.00000000f,2.00000000f},{0.00000000f,0.00000000f,0.00000000f},0},{12000,{1.25000000f,-2.50000000f,7.75000000f},{-0.20000000f,0.30000000f,-0.40000000f},0}};
}
