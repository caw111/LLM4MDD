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
inline std::vector<Stimulus> inputs={{0,2,0.0,1},{0,3,0.0,1},{0,4,0.0,1},{0,5,0.0,1},{2000,2,1.0,1},{4000,2,0.0,1},{5000,3,1.0,1},{7000,3,0.0,1},{8000,4,1.0,1},{10000,4,0.0,1},{11000,5,1.0,1},{13000,5,0.0,1}};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=17000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1732481334UL;
inline std::vector<ImuSample>imu_samples={};
}
