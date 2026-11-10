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
inline std::vector<Stimulus> inputs={{0,2,1.0,1},{0,3,1.0,1},{0,4,1.0,1},{0,7,1.0,1},{800,2,0.0,1},{900,2,1.0,1},{1200,7,0.0,1},{1300,7,1.0,1},{1600,7,0.0,1},{1700,7,1.0,1},{2000,7,0.0,1},{2100,7,1.0,1},{2500,3,0.0,1},{2600,3,1.0,1},{2900,7,0.0,1},{4800,7,1.0,1},{5200,7,0.0,1},{5300,7,1.0,1},{5600,7,0.0,1},{5700,7,1.0,1},{6000,4,0.0,1},{6100,4,1.0,1},{6400,7,0.0,1},{6500,7,1.0,1},{7200,7,0.0,1},{10000,7,1.0,1},{10400,7,0.0,1},{12700,7,1.0,1},{13000,7,0.0,1},{13100,7,1.0,1},{13400,7,0.0,1},{13500,7,1.0,1},{13800,7,0.0,1},{13900,7,1.0,1},{14200,7,0.0,1},{14300,7,1.0,1},{14600,2,0.0,1},{14700,2,1.0,1},{15000,7,0.0,1},{17500,7,1.0,1},{17900,7,0.0,1},{20400,7,1.0,1},{20800,7,0.0,1},{20900,7,1.0,1},{21300,3,0.0,1},{21400,3,1.0,1},{21800,7,0.0,1},{21900,7,1.0,1},{22300,7,0.0,1},{22400,7,1.0,1},{23000,7,0.0,1},{25500,7,1.0,1},{25900,7,0.0,1},{28400,7,1.0,1},{28600,2,0.0,1},{28700,2,1.0,1},{28800,7,0.0,1},{28900,7,1.0,1},{29200,7,0.0,1},{29300,7,1.0,1}};
inline std::vector<SerialInput> serial_inputs={{6800,"RESTOCK3 2\n"},{8800,"RESTOCK3 4\n"},{9550,"RESTOCK3 2"},{12670,"RESTOCK3 5\n"},{17300,"RESTOCK1 0\n"},{17400,"RESTOCK2 9999\n"},{25300,"RESTOCK1 1\n"}};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=30000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
