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
inline std::vector<Stimulus> inputs={{0,14,512.0,0},{0,15,52.0,0},{0,2,1.0,1},{8000,14,716.0,0},{8000,15,122.0,0},{10000,14,717.0,0},{12000,15,123.0,0},{14000,14,0.0,0},{16000,15,122.0,0},{18000,14,1023.0,0},{18000,15,205.0,0},{18000,2,0.0,1},{22000,2,1.0,1},{24000,14,307.0,0},{24000,15,0.0,0},{26000,15,7.0,0},{26000,2,0.0,1},{34000,2,1.0,1},{34000,14,0.0,0},{34000,15,0.0,0}};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=42500;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
