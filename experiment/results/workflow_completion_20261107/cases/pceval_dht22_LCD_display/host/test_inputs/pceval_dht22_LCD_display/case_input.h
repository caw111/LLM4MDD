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
inline std::vector<Stimulus> inputs={{0,2,45.5,3},{0,2,21.25,2},{4000,2,70.5,3},{8000,2,32.5,2},{12000,2,18.25,3},{12000,2,-8.5,2},{16000,2,70.5,3},{16000,2,32.5,2},{20000,2,45.5,3},{20000,2,21.25,2}};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=25000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
