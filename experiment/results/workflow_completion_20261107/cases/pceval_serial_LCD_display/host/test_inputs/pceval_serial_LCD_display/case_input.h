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
inline std::vector<SerialInput> serial_inputs={{1000,"Hello LCD!\n"},{2000," \tKeep  spaces\t \n"},{3000,"012345678901234567890123456789012345678901234567890123456789XYZ\n"},{4000,"Z\n"},{5000,"Split "},{5008,"message\r\n"},{6000,"Idle text"},{7000,"012345678901234567890123456789012345678901234567890123456789WXYZ\n"},{7600,"After oversized\n"},{8500,"CR only\r"},{9500,"Hello LCD!\n"}};
inline std::vector<Ultra>ultrasonic={};
inline std::vector<DirectDisplay>direct_displays={};
inline std::vector<Bus>buses={};
inline constexpr uint64_t horizon=11000;
inline std::vector<Multiplexer>multiplexers={};
inline constexpr uint32_t rtc_epoch=1767225600UL;
inline std::vector<ImuSample>imu_samples={};
}
