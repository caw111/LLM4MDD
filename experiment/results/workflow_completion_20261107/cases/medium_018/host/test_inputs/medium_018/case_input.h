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
inline std::vector<Stimulus> inputs={{0,2,1.0,1},{0,3,1.0,1},{0,14,0.0,0},{1200,14,1023.0,0},{3200,14,0.0,0},{5200,14,1023.0,0},{7200,14,0.0,0},{9000,14,1023.0,0},{9000,3,0.0,1},{9400,3,1.0,1},{9600,14,512.0,0},{10200,14,0.0,0},{10800,14,256.0,0},{11400,14,768.0,0},{12000,14,1023.0,0},{12400,3,0.0,1},{12800,3,1.0,1},{13000,2,0.0,1},{13400,2,1.0,1},{14000,14,0.0,0},{16000,14,512.0,0},{17600,2,0.0,1},{18000,2,1.0,1},{18500,3,0.0,1},{18900,3,1.0,1},{19100,14,0.0,0},{19700,14,1023.0,0}};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=20300;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
