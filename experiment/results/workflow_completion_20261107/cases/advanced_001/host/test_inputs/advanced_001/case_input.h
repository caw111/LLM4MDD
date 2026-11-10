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
inline std::vector<Stimulus> inputs={{0,2,1.0,1},{0,3,1.0,1},{0,7,30.0,2},{0,7,50.0,3},{0,14,30.0,0},{0,15,30.0,0},{0,16,0.0,0},{6500,7,25.0,2},{6500,14,400.0,0},{6500,15,400.0,0},{10000,7,35.0,2},{10000,14,399.0,0},{10000,15,401.0,0},{13500,7,30.0,2},{13500,14,401.0,0},{13500,15,399.0,0},{17000,2,0.0,1},{17100,2,1.0,1},{17400,16,1023.0,0},{18000,3,0.0,1},{18100,3,1.0,1},{18500,16,511.0,0},{19000,16,512.0,0},{19500,3,0.0,1},{19600,3,1.0,1},{19800,7,25.0,2},{19800,14,401.0,0},{19800,15,399.0,0},{20000,16,511.0,0},{20500,16,0.0,0},{21000,3,0.0,1},{21100,3,1.0,1},{21500,16,512.0,0},{22000,2,0.0,1},{22100,2,1.0,1},{23000,7,NAN,2},{27000,7,30.0,2},{31000,14,0.0,0},{35000,14,401.0,0},{39000,15,1023.0,0},{43000,15,399.0,0},{47000,14,1023.0,0},{51000,14,401.0,0},{55000,15,0.0,0}};
inline std::vector<SerialInput> serial_inputs={};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=59000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
