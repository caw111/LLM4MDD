#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_clockReady;
/* Global Variable define of  */
extern int32_t sysml_state_clockMs;
/* Global Variable define of  */
extern int32_t sysml_state_oldClockMs;
/* Global Variable define of  */
extern int32_t sysml_state_alarmOne;
/* Global Variable define of  */
extern int32_t sysml_state_alarmTwo;
/* Global Variable define of  */
extern int32_t sysml_state_alarmThree;
/* Global Variable define of  */
extern bool sysml_state_firedOne;
/* Global Variable define of  */
extern bool sysml_state_firedTwo;
/* Global Variable define of  */
extern bool sysml_state_firedThree;
/* Global Variable define of  */
extern bool sysml_state_activeTodayOne;
/* Global Variable define of  */
extern bool sysml_state_activeTodayTwo;
/* Global Variable define of  */
extern bool sysml_state_activeTodayThree;
/* Global Variable define of  */
extern int32_t sysml_state_statusOne;
/* Global Variable define of  */
extern int32_t sysml_state_statusTwo;
/* Global Variable define of  */
extern int32_t sysml_state_statusThree;
/* Global Variable define of  */
extern int32_t sysml_state_ageOne;
/* Global Variable define of  */
extern int32_t sysml_state_ageTwo;
/* Global Variable define of  */
extern int32_t sysml_state_ageThree;
/* Global Variable define of  */
extern int32_t sysml_state_occurrencesOne;
/* Global Variable define of  */
extern int32_t sysml_state_occurrencesTwo;
/* Global Variable define of  */
extern int32_t sysml_state_occurrencesThree;
/* Global Variable define of  */
extern int32_t sysml_state_batchMisses;
/* Global Variable define of  */
extern int32_t sysml_state_missedCount;
/* Global Variable define of  */
extern int32_t sysml_state_pendingWarnings;
/* Global Variable define of  */
extern bool sysml_state_warningStep;
/* Global Variable define of  */
extern int32_t sysml_state_displayPage;
/* Global Variable define of  */
extern bool sysml_state_confirmArmed;
/* Global Variable define of  */
extern bool sysml_state_previousArmed;
/* Global Variable define of  */
extern bool sysml_state_nextArmed;
/* Global Variable define of  */
extern int32_t sysml_state_confirmReleaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_previousReleaseMs;
/* Global Variable define of  */
extern int32_t sysml_state_nextReleaseMs;
/* Global Variable define of  */
extern bool sysml_state_confirmPress;
/* Global Variable define of  */
extern bool sysml_state_previousPress;
/* Global Variable define of  */
extern bool sysml_state_nextPress;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool io_1, bool io_2, bool io_3, int32_t io_8, int32_t* io_4, bool* io_5, int32_t* io_6, bool* io_7);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_clockReady = false;
/* Global Variable define of  */
int32_t sysml_state_clockMs = 0;
/* Global Variable define of  */
int32_t sysml_state_oldClockMs = 0;
/* Global Variable define of  */
int32_t sysml_state_alarmOne = 420;
/* Global Variable define of  */
int32_t sysml_state_alarmTwo = 720;
/* Global Variable define of  */
int32_t sysml_state_alarmThree = 1080;
/* Global Variable define of  */
bool sysml_state_firedOne = false;
/* Global Variable define of  */
bool sysml_state_firedTwo = false;
/* Global Variable define of  */
bool sysml_state_firedThree = false;
/* Global Variable define of  */
bool sysml_state_activeTodayOne = false;
/* Global Variable define of  */
bool sysml_state_activeTodayTwo = false;
/* Global Variable define of  */
bool sysml_state_activeTodayThree = false;
/* Global Variable define of  */
int32_t sysml_state_statusOne = 0;
/* Global Variable define of  */
int32_t sysml_state_statusTwo = 0;
/* Global Variable define of  */
int32_t sysml_state_statusThree = 0;
/* Global Variable define of  */
int32_t sysml_state_ageOne = 0;
/* Global Variable define of  */
int32_t sysml_state_ageTwo = 0;
/* Global Variable define of  */
int32_t sysml_state_ageThree = 0;
/* Global Variable define of  */
int32_t sysml_state_occurrencesOne = 0;
/* Global Variable define of  */
int32_t sysml_state_occurrencesTwo = 0;
/* Global Variable define of  */
int32_t sysml_state_occurrencesThree = 0;
/* Global Variable define of  */
int32_t sysml_state_batchMisses = 0;
/* Global Variable define of  */
int32_t sysml_state_missedCount = 0;
/* Global Variable define of  */
int32_t sysml_state_pendingWarnings = 0;
/* Global Variable define of  */
bool sysml_state_warningStep = false;
/* Global Variable define of  */
int32_t sysml_state_displayPage = 0;
/* Global Variable define of  */
bool sysml_state_confirmArmed = true;
/* Global Variable define of  */
bool sysml_state_previousArmed = true;
/* Global Variable define of  */
bool sysml_state_nextArmed = true;
/* Global Variable define of  */
int32_t sysml_state_confirmReleaseMs = 30;
/* Global Variable define of  */
int32_t sysml_state_previousReleaseMs = 30;
/* Global Variable define of  */
int32_t sysml_state_nextReleaseMs = 30;
/* Global Variable define of  */
bool sysml_state_confirmPress = false;
/* Global Variable define of  */
bool sysml_state_previousPress = false;
/* Global Variable define of  */
bool sysml_state_nextPress = false;
/* Init function of CompositeActor named "ProgramPage1" */
#line 1896 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_parse(int32_t* value);
#line 1914 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_finish(int32_t* value);
#line 1919 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
static bool hw_rx_poll(uint32_t hw_now, int32_t* value);
#line 1955 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
void setup();
#line 1978 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
void loop();
#line 165 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\workflow_completion_20261107\\run5_remaining\\cases\\advanced_004\\attempt_0\\arduino\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_clockReady = false;
    sysml_state_clockMs = 0;
    sysml_state_oldClockMs = 0;
    sysml_state_alarmOne = 420;
    sysml_state_alarmTwo = 720;
    sysml_state_alarmThree = 1080;
    sysml_state_firedOne = false;
    sysml_state_firedTwo = false;
    sysml_state_firedThree = false;
    sysml_state_activeTodayOne = false;
    sysml_state_activeTodayTwo = false;
    sysml_state_activeTodayThree = false;
    sysml_state_statusOne = 0;
    sysml_state_statusTwo = 0;
    sysml_state_statusThree = 0;
    sysml_state_ageOne = 0;
    sysml_state_ageTwo = 0;
    sysml_state_ageThree = 0;
    sysml_state_occurrencesOne = 0;
    sysml_state_occurrencesTwo = 0;
    sysml_state_occurrencesThree = 0;
    sysml_state_batchMisses = 0;
    sysml_state_missedCount = 0;
    sysml_state_pendingWarnings = 0;
    sysml_state_warningStep = false;
    sysml_state_displayPage = 0;
    sysml_state_confirmArmed = true;
    sysml_state_previousArmed = true;
    sysml_state_nextArmed = true;
    sysml_state_confirmReleaseMs = 30;
    sysml_state_previousReleaseMs = 30;
    sysml_state_nextReleaseMs = 30;
    sysml_state_confirmPress = false;
    sysml_state_previousPress = false;
    sysml_state_nextPress = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, bool io_0_valid, bool io_1, bool io_2, bool io_3, int32_t io_8, int32_t* io_4, bool* io_5, int32_t* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    int32_t sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    int32_t sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_arithmetic_27 = 0;
    int32_t sysml_conditional_28 = 0;
    int32_t sysml_arithmetic_29 = 0;
    int32_t sysml_conditional_30 = 0;
    int32_t sysml_arithmetic_31 = 0;
    int32_t sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    int32_t sysml_conditional_34 = 0;
    int32_t sysml_arithmetic_35 = 0;
    int32_t sysml_conditional_36 = 0;
    int32_t sysml_arithmetic_37 = 0;
    int32_t sysml_conditional_38 = 0;
    int32_t sysml_arithmetic_39 = 0;
    int32_t sysml_conditional_40 = 0;
    int32_t sysml_conditional_41 = 0;
    int32_t sysml_arithmetic_42 = 0;
    int32_t sysml_conditional_43 = 0;
    int32_t sysml_arithmetic_44 = 0;
    int32_t sysml_conditional_45 = 0;
    int32_t sysml_arithmetic_46 = 0;
    int32_t sysml_conditional_47 = 0;
    int32_t sysml_conditional_48 = 0;
    int32_t sysml_conditional_49 = 0;
    int32_t sysml_arithmetic_50 = 0;
    int32_t sysml_conditional_51 = 0;
    int32_t sysml_arithmetic_52 = 0;
    int32_t sysml_conditional_53 = 0;
    int32_t sysml_arithmetic_54 = 0;
    int32_t sysml_conditional_55 = 0;
    int32_t sysml_conditional_56 = 0;
    int32_t sysml_arithmetic_57 = 0;
    int32_t sysml_conditional_58 = 0;
    int32_t sysml_arithmetic_59 = 0;
    int32_t sysml_conditional_60 = 0;
    int32_t sysml_arithmetic_61 = 0;
    int32_t sysml_conditional_62 = 0;
    int32_t sysml_conditional_63 = 0;
    int32_t sysml_conditional_64 = 0;
    int32_t sysml_arithmetic_65 = 0;
    int32_t sysml_conditional_66 = 0;
    int32_t sysml_arithmetic_67 = 0;
    int32_t sysml_conditional_68 = 0;
    int32_t sysml_arithmetic_69 = 0;
    int32_t sysml_conditional_70 = 0;
    bool sysml_lazy_71 = 0;
    int32_t sysml_conditional_72 = 0;
    bool sysml_lazy_73 = 0;
    int32_t sysml_conditional_74 = 0;
    bool sysml_lazy_75 = 0;
    int32_t sysml_conditional_76 = 0;
    int32_t sysml_conditional_77 = 0;
    int32_t sysml_conditional_78 = 0;
    int32_t sysml_conditional_79 = 0;
    int32_t sysml_conditional_80 = 0;
    bool sysml_lazy_81 = 0;
    int32_t sysml_conditional_82 = 0;
    int32_t sysml_conditional_83 = 0;
    int32_t sysml_conditional_84 = 0;
    int32_t sysml_conditional_85 = 0;
    int32_t sysml_conditional_86 = 0;
    bool sysml_lazy_87 = 0;
    int32_t sysml_conditional_88 = 0;
    bool sysml_lazy_89 = 0;
    int32_t sysml_conditional_90 = 0;
    int32_t sysml_conditional_91 = 0;
    int32_t sysml_conditional_92 = 0;
    int32_t sysml_conditional_93 = 0;
    int32_t sysml_conditional_94 = 0;
    int32_t sysml_conditional_95 = 0;
    int32_t sysml_conditional_96 = 0;
    int32_t sysml_conditional_97 = 0;
    int32_t sysml_conditional_98 = 0;
    int32_t sysml_conditional_99 = 0;
    int32_t sysml_conditional_100 = 0;
    bool sysml_lazy_101 = 0;
    bool sysml_lazy_102 = 0;
    int32_t sysml_conditional_103 = 0;
    int32_t sysml_conditional_104 = 0;
    int32_t sysml_conditional_105 = 0;
    int32_t sysml_conditional_106 = 0;
    int32_t sysml_conditional_107 = 0;
    int32_t sysml_conditional_108 = 0;
    int32_t sysml_conditional_109 = 0;
    int32_t sysml_conditional_110 = 0;
    int32_t sysml_conditional_111 = 0;
    bool sysml_lazy_112 = 0;
    bool sysml_lazy_113 = 0;
    int32_t sysml_conditional_114 = 0;
    int32_t sysml_conditional_115 = 0;
    int32_t sysml_conditional_116 = 0;
    int32_t sysml_conditional_117 = 0;
    int32_t sysml_conditional_118 = 0;
    int32_t sysml_conditional_119 = 0;
    int32_t sysml_conditional_120 = 0;
    int32_t sysml_conditional_121 = 0;
    int32_t sysml_conditional_122 = 0;
    bool sysml_lazy_123 = 0;
    bool sysml_lazy_124 = 0;
    int32_t sysml_conditional_125 = 0;
    int32_t sysml_conditional_126 = 0;
    int32_t sysml_conditional_127 = 0;
    int32_t sysml_conditional_128 = 0;
    int32_t sysml_conditional_129 = 0;
    int32_t sysml_conditional_130 = 0;
    int32_t sysml_conditional_131 = 0;
    int32_t sysml_conditional_132 = 0;
    int32_t sysml_conditional_133 = 0;
    int32_t sysml_conditional_134 = 0;
    int32_t sysml_conditional_135 = 0;
    int32_t sysml_conditional_136 = 0;
    int32_t sysml_conditional_137 = 0;
    int32_t sysml_conditional_138 = 0;
    int32_t sysml_conditional_139 = 0;
    int32_t sysml_conditional_140 = 0;
    int32_t sysml_conditional_141 = 0;
    int32_t sysml_conditional_142 = 0;
    int32_t sysml_conditional_143 = 0;
    int32_t sysml_conditional_144 = 0;
    int32_t sysml_conditional_145 = 0;
    int32_t sysml_conditional_146 = 0;
    int32_t sysml_conditional_147 = 0;
    int32_t sysml_conditional_148 = 0;
    int32_t sysml_conditional_149 = 0;
    int32_t sysml_conditional_150 = 0;
    int32_t sysml_conditional_151 = 0;
    int32_t sysml_conditional_152 = 0;
    int32_t sysml_conditional_153 = 0;
    int32_t sysml_conditional_154 = 0;
    int32_t sysml_conditional_155 = 0;
    int32_t sysml_conditional_156 = 0;
    int32_t sysml_conditional_157 = 0;
    int32_t sysml_conditional_158 = 0;
    int32_t sysml_conditional_159 = 0;
    int32_t sysml_conditional_160 = 0;
    int32_t sysml_conditional_161 = 0;
    int32_t sysml_conditional_162 = 0;
    int32_t sysml_conditional_163 = 0;
    bool sysml_lazy_164 = 0;
    int32_t sysml_conditional_165 = 0;
    bool sysml_lazy_166 = 0;
    int32_t sysml_conditional_167 = 0;
    bool sysml_lazy_168 = 0;
    int32_t sysml_conditional_169 = 0;
    bool sysml_lazy_170 = 0;
    int32_t sysml_conditional_171 = 0;
    bool sysml_lazy_172 = 0;
    int32_t sysml_conditional_173 = 0;
    bool sysml_lazy_174 = 0;
    int32_t sysml_conditional_175 = 0;
    int32_t sysml_conditional_176 = 0;
    int32_t sysml_conditional_177 = 0;
    bool sysml_lazy_178 = 0;
    bool sysml_lazy_179 = 0;
    int32_t sysml_conditional_180 = 0;
    bool sysml_lazy_181 = 0;
    bool sysml_lazy_182 = 0;
    int32_t sysml_conditional_183 = 0;
    int32_t sysml_conditional_184 = 0;
    int32_t sysml_conditional_185 = 0;
    bool sysml_lazy_186 = 0;
    int32_t sysml_conditional_187 = 0;
    int32_t sysml_arithmetic_188 = 0;
    bool sysml_lazy_189 = 0;
    int32_t sysml_conditional_190 = 0;
    int32_t sysml_arithmetic_191 = 0;
    bool sysml_lazy_192 = 0;
    int32_t sysml_conditional_193 = 0;
    int32_t sysml_arithmetic_194 = 0;
    int32_t sysml_conditional_195 = 0;
    int32_t sysml_conditional_196 = 0;
    int32_t sysml_conditional_197 = 0;
    int32_t sysml_conditional_198 = 0;
    int32_t sysml_conditional_199 = 0;
    int32_t sysml_conditional_200 = 0;
    int32_t sysml_conditional_201 = 0;
    int32_t sysml_conditional_202 = 0;
    int32_t sysml_arithmetic_203 = 0;
    int32_t sysml_conditional_204 = 0;
    int32_t sysml_arithmetic_205 = 0;
    int32_t sysml_conditional_206 = 0;
    int32_t sysml_arithmetic_207 = 0;
    int32_t sysml_conditional_208 = 0;
    int32_t sysml_conditional_209 = 0;
    int32_t sysml_arithmetic_210 = 0;
    int32_t sysml_conditional_211 = 0;
    int32_t sysml_arithmetic_212 = 0;
    int32_t sysml_conditional_213 = 0;
    int32_t sysml_arithmetic_214 = 0;
    int32_t sysml_conditional_215 = 0;
    int32_t sysml_conditional_216 = 0;
    int32_t sysml_arithmetic_217 = 0;
    int32_t sysml_conditional_218 = 0;
    int32_t sysml_arithmetic_219 = 0;
    int32_t sysml_conditional_220 = 0;
    int32_t sysml_arithmetic_221 = 0;
    int32_t sysml_conditional_222 = 0;
    int32_t sysml_conditional_223 = 0;
    bool sysml_lazy_224 = 0;
    bool sysml_lazy_225 = 0;
    int32_t sysml_conditional_226 = 0;
    int32_t sysml_conditional_227 = 0;
    int32_t sysml_conditional_228 = 0;
    int32_t sysml_conditional_229 = 0;
    int32_t sysml_conditional_230 = 0;
    bool sysml_lazy_231 = 0;
    bool sysml_lazy_232 = 0;
    bool sysml_lazy_233 = 0;
    bool sysml_lazy_234 = 0;
    bool sysml_lazy_235 = 0;
    bool sysml_lazy_236 = 0;
    sysml_state_warningStep = false;
    sysml_state_confirmPress = false;
    sysml_state_previousPress = false;
    sysml_state_nextPress = false;
    sysml_state_batchMisses = 0;
    if (io_1) {
        if (sysml_state_confirmArmed) {
            sysml_state_confirmPress = true;
            sysml_state_confirmArmed = false;
        }
        sysml_state_confirmReleaseMs = 0;
    }
    else {
        if (io_8 >= (30 - sysml_state_confirmReleaseMs)) {
            sysml_conditional_0 = 30;
        }
        else {
            sysml_conditional_0 = sysml_state_confirmReleaseMs + io_8;
        }
        sysml_state_confirmReleaseMs = sysml_conditional_0;
        if ((sysml_state_confirmReleaseMs >= 30)) {
            sysml_state_confirmArmed = true;
        }
    }
    if (io_2) {
        if (sysml_state_previousArmed) {
            sysml_state_previousPress = true;
            sysml_state_previousArmed = false;
        }
        sysml_state_previousReleaseMs = 0;
    }
    else {
        if (io_8 >= (30 - sysml_state_previousReleaseMs)) {
            sysml_conditional_1 = 30;
        }
        else {
            sysml_conditional_1 = sysml_state_previousReleaseMs + io_8;
        }
        sysml_state_previousReleaseMs = sysml_conditional_1;
        if ((sysml_state_previousReleaseMs >= 30)) {
            sysml_state_previousArmed = true;
        }
    }
    if (io_3) {
        if (sysml_state_nextArmed) {
            sysml_state_nextPress = true;
            sysml_state_nextArmed = false;
        }
        sysml_state_nextReleaseMs = 0;
    }
    else {
        if (io_8 >= (30 - sysml_state_nextReleaseMs)) {
            sysml_conditional_2 = 30;
        }
        else {
            sysml_conditional_2 = sysml_state_nextReleaseMs + io_8;
        }
        sysml_state_nextReleaseMs = sysml_conditional_2;
        if ((sysml_state_nextReleaseMs >= 30)) {
            sysml_state_nextArmed = true;
        }
    }
    sysml_state_oldClockMs = sysml_state_clockMs;
    if ((sysml_state_statusOne == 1)) {
        if (io_8 >= (30001 - sysml_state_ageOne)) {
            sysml_conditional_3 = 30001;
        }
        else {
            sysml_conditional_3 = sysml_state_ageOne + io_8;
        }
        sysml_state_ageOne = sysml_conditional_3;
    }
    if ((sysml_state_statusTwo == 1)) {
        if (io_8 >= (30001 - sysml_state_ageTwo)) {
            sysml_conditional_4 = 30001;
        }
        else {
            sysml_conditional_4 = sysml_state_ageTwo + io_8;
        }
        sysml_state_ageTwo = sysml_conditional_4;
    }
    if ((sysml_state_statusThree == 1)) {
        if (io_8 >= (30001 - sysml_state_ageThree)) {
            sysml_conditional_5 = 30001;
        }
        else {
            sysml_conditional_5 = sysml_state_ageThree + io_8;
        }
        sysml_state_ageThree = sysml_conditional_5;
    }
    if (sysml_state_clockReady) {
        if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_lazy_8 = sysml_state_firedOne;
        if (sysml_lazy_8) {
            sysml_lazy_8 = sysml_state_oldClockMs < (sysml_state_alarmOne * 60000);
        }
        sysml_lazy_9 = sysml_lazy_8;
        if (sysml_lazy_9) {
            sysml_lazy_9 = ((sysml_state_oldClockMs * 1.0) + io_8) >= (sysml_state_alarmOne * 60000.0);
        }
        if (sysml_lazy_9) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        sysml_state_occurrencesOne = ((((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_6) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_7)) - sysml_conditional_10;
        if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        sysml_lazy_13 = sysml_state_firedTwo;
        if (sysml_lazy_13) {
            sysml_lazy_13 = sysml_state_oldClockMs < (sysml_state_alarmTwo * 60000);
        }
        sysml_lazy_14 = sysml_lazy_13;
        if (sysml_lazy_14) {
            sysml_lazy_14 = ((sysml_state_oldClockMs * 1.0) + io_8) >= (sysml_state_alarmTwo * 60000.0);
        }
        if (sysml_lazy_14) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_state_occurrencesTwo = ((((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_11) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_12)) - sysml_conditional_15;
        if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_lazy_18 = sysml_state_firedThree;
        if (sysml_lazy_18) {
            sysml_lazy_18 = sysml_state_oldClockMs < (sysml_state_alarmThree * 60000);
        }
        sysml_lazy_19 = sysml_lazy_18;
        if (sysml_lazy_19) {
            sysml_lazy_19 = ((sysml_state_oldClockMs * 1.0) + io_8) >= (sysml_state_alarmThree * 60000.0);
        }
        if (sysml_lazy_19) {
            sysml_conditional_20 = 1;
        }
        else {
            sysml_conditional_20 = 0;
        }
        sysml_state_occurrencesThree = ((((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_16) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_17)) - sysml_conditional_20;
        if ((((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0) < ((double)((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)))) {
            sysml_conditional_21 = 1;
        }
        else {
            sysml_conditional_21 = 0;
        }
        if ((((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0) < ((double)((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)))) {
            sysml_conditional_22 = 1;
        }
        else {
            sysml_conditional_22 = 0;
        }
        if ((((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0) < ((double)((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)))) {
            sysml_conditional_23 = 1;
        }
        else {
            sysml_conditional_23 = 0;
        }
        if ((((sysml_state_oldClockMs * 1.0) + io_8) - ((((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)) - sysml_conditional_22) * 86400000.0)) < ((double)((int32_t)(((sysml_state_oldClockMs * 1.0) + io_8) - ((((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)) - sysml_conditional_23) * 86400000.0))))) {
            sysml_conditional_24 = 1;
        }
        else {
            sysml_conditional_24 = 0;
        }
        sysml_state_clockMs = ((int32_t)(((sysml_state_oldClockMs * 1.0) + io_8) - ((((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)) - sysml_conditional_21) * 86400000.0))) - sysml_conditional_24;
        if ((((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0) < ((double)((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)))) {
            sysml_conditional_25 = 1;
        }
        else {
            sysml_conditional_25 = 0;
        }
        if ((((int32_t)(((double)((sysml_state_oldClockMs * 1.0) + io_8)) / 86400000.0)) - sysml_conditional_25) > 0) {
            sysml_state_firedOne = false;
            sysml_state_firedTwo = false;
            sysml_state_firedThree = false;
            sysml_state_activeTodayOne = false;
            sysml_state_activeTodayTwo = false;
            sysml_state_activeTodayThree = false;
            if ((sysml_state_statusOne != 1)) {
                sysml_state_statusOne = 0;
            }
            if ((sysml_state_statusTwo != 1)) {
                sysml_state_statusTwo = 0;
            }
            if ((sysml_state_statusThree != 1)) {
                sysml_state_statusThree = 0;
            }
        }
        if ((sysml_state_occurrencesOne > 0)) {
            sysml_state_batchMisses = ((sysml_state_batchMisses + sysml_state_occurrencesOne) - 1);
            if ((sysml_state_statusOne == 1)) {
                sysml_state_batchMisses = (sysml_state_batchMisses + 1);
            }
            sysml_state_statusOne = 1;
            sysml_state_activeTodayOne = (sysml_state_clockMs >= (sysml_state_alarmOne * 60000));
            sysml_state_firedOne = sysml_state_activeTodayOne;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                sysml_conditional_26 = 1;
            }
            else {
                sysml_conditional_26 = 0;
            }
            sysml_arithmetic_27 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_26;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                sysml_conditional_28 = 1;
            }
            else {
                sysml_conditional_28 = 0;
            }
            sysml_arithmetic_29 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_28;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                sysml_conditional_30 = 1;
            }
            else {
                sysml_conditional_30 = 0;
            }
            sysml_arithmetic_31 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_30;
            if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_29)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_31))))) {
                sysml_conditional_32 = 1;
            }
            else {
                sysml_conditional_32 = 0;
            }
            if ((((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_27))) - sysml_conditional_32) > 30001) {
                sysml_conditional_33 = 30001;
            }
            else {
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_34 = 1;
                }
                else {
                    sysml_conditional_34 = 0;
                }
                sysml_arithmetic_35 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_34;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_36 = 1;
                }
                else {
                    sysml_conditional_36 = 0;
                }
                sysml_arithmetic_37 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_36;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_38 = 1;
                }
                else {
                    sysml_conditional_38 = 0;
                }
                sysml_arithmetic_39 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_38;
                if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_37)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_39))))) {
                    sysml_conditional_40 = 1;
                }
                else {
                    sysml_conditional_40 = 0;
                }
                sysml_conditional_33 = ((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_35))) - sysml_conditional_40;
            }
            sysml_state_ageOne = sysml_conditional_33;
        }
        if ((sysml_state_occurrencesTwo > 0)) {
            sysml_state_batchMisses = ((sysml_state_batchMisses + sysml_state_occurrencesTwo) - 1);
            if ((sysml_state_statusTwo == 1)) {
                sysml_state_batchMisses = (sysml_state_batchMisses + 1);
            }
            sysml_state_statusTwo = 1;
            sysml_state_activeTodayTwo = (sysml_state_clockMs >= (sysml_state_alarmTwo * 60000));
            sysml_state_firedTwo = sysml_state_activeTodayTwo;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                sysml_conditional_41 = 1;
            }
            else {
                sysml_conditional_41 = 0;
            }
            sysml_arithmetic_42 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_41;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                sysml_conditional_43 = 1;
            }
            else {
                sysml_conditional_43 = 0;
            }
            sysml_arithmetic_44 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_43;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                sysml_conditional_45 = 1;
            }
            else {
                sysml_conditional_45 = 0;
            }
            sysml_arithmetic_46 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_45;
            if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_44)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_46))))) {
                sysml_conditional_47 = 1;
            }
            else {
                sysml_conditional_47 = 0;
            }
            if ((((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_42))) - sysml_conditional_47) > 30001) {
                sysml_conditional_48 = 30001;
            }
            else {
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_49 = 1;
                }
                else {
                    sysml_conditional_49 = 0;
                }
                sysml_arithmetic_50 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_49;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_51 = 1;
                }
                else {
                    sysml_conditional_51 = 0;
                }
                sysml_arithmetic_52 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_51;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_53 = 1;
                }
                else {
                    sysml_conditional_53 = 0;
                }
                sysml_arithmetic_54 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_53;
                if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_52)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_54))))) {
                    sysml_conditional_55 = 1;
                }
                else {
                    sysml_conditional_55 = 0;
                }
                sysml_conditional_48 = ((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_50))) - sysml_conditional_55;
            }
            sysml_state_ageTwo = sysml_conditional_48;
        }
        if ((sysml_state_occurrencesThree > 0)) {
            sysml_state_batchMisses = ((sysml_state_batchMisses + sysml_state_occurrencesThree) - 1);
            if ((sysml_state_statusThree == 1)) {
                sysml_state_batchMisses = (sysml_state_batchMisses + 1);
            }
            sysml_state_statusThree = 1;
            sysml_state_activeTodayThree = (sysml_state_clockMs >= (sysml_state_alarmThree * 60000));
            sysml_state_firedThree = sysml_state_activeTodayThree;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                sysml_conditional_56 = 1;
            }
            else {
                sysml_conditional_56 = 0;
            }
            sysml_arithmetic_57 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_56;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                sysml_conditional_58 = 1;
            }
            else {
                sysml_conditional_58 = 0;
            }
            sysml_arithmetic_59 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_58;
            if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                sysml_conditional_60 = 1;
            }
            else {
                sysml_conditional_60 = 0;
            }
            sysml_arithmetic_61 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_60;
            if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_59)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_61))))) {
                sysml_conditional_62 = 1;
            }
            else {
                sysml_conditional_62 = 0;
            }
            if ((((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_57))) - sysml_conditional_62) > 30001) {
                sysml_conditional_63 = 30001;
            }
            else {
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_64 = 1;
                }
                else {
                    sysml_conditional_64 = 0;
                }
                sysml_arithmetic_65 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_64;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_66 = 1;
                }
                else {
                    sysml_conditional_66 = 0;
                }
                sysml_arithmetic_67 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_66;
                if ((((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
                    sysml_conditional_68 = 1;
                }
                else {
                    sysml_conditional_68 = 0;
                }
                sysml_arithmetic_69 = ((int32_t)(((double)(((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_68;
                if (((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_67)) < ((double)((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_69))))) {
                    sysml_conditional_70 = 1;
                }
                else {
                    sysml_conditional_70 = 0;
                }
                sysml_conditional_63 = ((int32_t)((((sysml_state_oldClockMs * 1.0) + io_8) - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_65))) - sysml_conditional_70;
            }
            sysml_state_ageThree = sysml_conditional_63;
        }
    }
    sysml_lazy_71 = io_0_valid;
    if (sysml_lazy_71) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_72 = 1;
        }
        else {
            sysml_conditional_72 = 0;
        }
        sysml_lazy_71 = (io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_72) * 10000)) >= 0;
    }
    sysml_lazy_73 = sysml_lazy_71;
    if (sysml_lazy_73) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_74 = 1;
        }
        else {
            sysml_conditional_74 = 0;
        }
        sysml_lazy_73 = (io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_74) * 10000)) <= 2359;
    }
    sysml_lazy_75 = sysml_lazy_73;
    if (sysml_lazy_75) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_76 = 1;
        }
        else {
            sysml_conditional_76 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_77 = 1;
        }
        else {
            sysml_conditional_77 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_78 = 1;
        }
        else {
            sysml_conditional_78 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_79 = 1;
        }
        else {
            sysml_conditional_79 = 0;
        }
        if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_78) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_79) * 10000))) / 100)))) {
            sysml_conditional_80 = 1;
        }
        else {
            sysml_conditional_80 = 0;
        }
        sysml_lazy_75 = ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_76) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_77) * 10000))) / 100)) - sysml_conditional_80) * 100)) >= 0;
    }
    sysml_lazy_81 = sysml_lazy_75;
    if (sysml_lazy_81) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_82 = 1;
        }
        else {
            sysml_conditional_82 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_83 = 1;
        }
        else {
            sysml_conditional_83 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_84 = 1;
        }
        else {
            sysml_conditional_84 = 0;
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_85 = 1;
        }
        else {
            sysml_conditional_85 = 0;
        }
        if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_84) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_85) * 10000))) / 100)))) {
            sysml_conditional_86 = 1;
        }
        else {
            sysml_conditional_86 = 0;
        }
        sysml_lazy_81 = ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_82) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_83) * 10000))) / 100)) - sysml_conditional_86) * 100)) < 60;
    }
    sysml_lazy_87 = sysml_lazy_81;
    if (sysml_lazy_87) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_88 = 1;
        }
        else {
            sysml_conditional_88 = 0;
        }
        sysml_lazy_87 = (((int32_t)(((double)io_0) / 10000)) - sysml_conditional_88) >= 1;
    }
    sysml_lazy_89 = sysml_lazy_87;
    if (sysml_lazy_89) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_90 = 1;
        }
        else {
            sysml_conditional_90 = 0;
        }
        sysml_lazy_89 = (((int32_t)(((double)io_0) / 10000)) - sysml_conditional_90) <= 4;
    }
    if (sysml_lazy_89) {
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_91 = 1;
        }
        else {
            sysml_conditional_91 = 0;
        }
        if ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_91) == 1) {
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_92 = 1;
            }
            else {
                sysml_conditional_92 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_93 = 1;
            }
            else {
                sysml_conditional_93 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_94 = 1;
            }
            else {
                sysml_conditional_94 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_93) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_94) * 10000))) / 100)))) {
                sysml_conditional_95 = 1;
            }
            else {
                sysml_conditional_95 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_96 = 1;
            }
            else {
                sysml_conditional_96 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_97 = 1;
            }
            else {
                sysml_conditional_97 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_98 = 1;
            }
            else {
                sysml_conditional_98 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_99 = 1;
            }
            else {
                sysml_conditional_99 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_98) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_99) * 10000))) / 100)))) {
                sysml_conditional_100 = 1;
            }
            else {
                sysml_conditional_100 = 0;
            }
            sysml_state_clockMs = (((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_92) * 10000))) / 100)) - sysml_conditional_95) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_96) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_97) * 10000))) / 100)) - sysml_conditional_100) * 100))) * 60000;
            sysml_state_clockReady = true;
            sysml_lazy_101 = !sysml_state_firedOne;
            if (sysml_lazy_101) {
                sysml_lazy_101 = sysml_state_statusOne != 1;
            }
            sysml_lazy_102 = sysml_lazy_101;
            if (sysml_lazy_102) {
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_103 = 1;
                }
                else {
                    sysml_conditional_103 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_104 = 1;
                }
                else {
                    sysml_conditional_104 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_105 = 1;
                }
                else {
                    sysml_conditional_105 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_104) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_105) * 10000))) / 100)))) {
                    sysml_conditional_106 = 1;
                }
                else {
                    sysml_conditional_106 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_107 = 1;
                }
                else {
                    sysml_conditional_107 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_108 = 1;
                }
                else {
                    sysml_conditional_108 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_109 = 1;
                }
                else {
                    sysml_conditional_109 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_110 = 1;
                }
                else {
                    sysml_conditional_110 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_109) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_110) * 10000))) / 100)))) {
                    sysml_conditional_111 = 1;
                }
                else {
                    sysml_conditional_111 = 0;
                }
                sysml_lazy_102 = (((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_103) * 10000))) / 100)) - sysml_conditional_106) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_107) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_108) * 10000))) / 100)) - sysml_conditional_111) * 100))) == sysml_state_alarmOne;
            }
            if (sysml_lazy_102) {
                sysml_state_firedOne = true;
                sysml_state_activeTodayOne = true;
                sysml_state_statusOne = 1;
                sysml_state_ageOne = 0;
            }
            sysml_lazy_112 = !sysml_state_firedTwo;
            if (sysml_lazy_112) {
                sysml_lazy_112 = sysml_state_statusTwo != 1;
            }
            sysml_lazy_113 = sysml_lazy_112;
            if (sysml_lazy_113) {
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_114 = 1;
                }
                else {
                    sysml_conditional_114 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_115 = 1;
                }
                else {
                    sysml_conditional_115 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_116 = 1;
                }
                else {
                    sysml_conditional_116 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_115) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_116) * 10000))) / 100)))) {
                    sysml_conditional_117 = 1;
                }
                else {
                    sysml_conditional_117 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_118 = 1;
                }
                else {
                    sysml_conditional_118 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_119 = 1;
                }
                else {
                    sysml_conditional_119 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_120 = 1;
                }
                else {
                    sysml_conditional_120 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_121 = 1;
                }
                else {
                    sysml_conditional_121 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_120) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_121) * 10000))) / 100)))) {
                    sysml_conditional_122 = 1;
                }
                else {
                    sysml_conditional_122 = 0;
                }
                sysml_lazy_113 = (((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_114) * 10000))) / 100)) - sysml_conditional_117) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_118) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_119) * 10000))) / 100)) - sysml_conditional_122) * 100))) == sysml_state_alarmTwo;
            }
            if (sysml_lazy_113) {
                sysml_state_firedTwo = true;
                sysml_state_activeTodayTwo = true;
                sysml_state_statusTwo = 1;
                sysml_state_ageTwo = 0;
            }
            sysml_lazy_123 = !sysml_state_firedThree;
            if (sysml_lazy_123) {
                sysml_lazy_123 = sysml_state_statusThree != 1;
            }
            sysml_lazy_124 = sysml_lazy_123;
            if (sysml_lazy_124) {
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_125 = 1;
                }
                else {
                    sysml_conditional_125 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_126 = 1;
                }
                else {
                    sysml_conditional_126 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_127 = 1;
                }
                else {
                    sysml_conditional_127 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_126) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_127) * 10000))) / 100)))) {
                    sysml_conditional_128 = 1;
                }
                else {
                    sysml_conditional_128 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_129 = 1;
                }
                else {
                    sysml_conditional_129 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_130 = 1;
                }
                else {
                    sysml_conditional_130 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_131 = 1;
                }
                else {
                    sysml_conditional_131 = 0;
                }
                if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                    sysml_conditional_132 = 1;
                }
                else {
                    sysml_conditional_132 = 0;
                }
                if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_131) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_132) * 10000))) / 100)))) {
                    sysml_conditional_133 = 1;
                }
                else {
                    sysml_conditional_133 = 0;
                }
                sysml_lazy_124 = (((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_125) * 10000))) / 100)) - sysml_conditional_128) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_129) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_130) * 10000))) / 100)) - sysml_conditional_133) * 100))) == sysml_state_alarmThree;
            }
            if (sysml_lazy_124) {
                sysml_state_firedThree = true;
                sysml_state_activeTodayThree = true;
                sysml_state_statusThree = 1;
                sysml_state_ageThree = 0;
            }
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_134 = 1;
        }
        else {
            sysml_conditional_134 = 0;
        }
        if ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_134) == 2) {
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_135 = 1;
            }
            else {
                sysml_conditional_135 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_136 = 1;
            }
            else {
                sysml_conditional_136 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_137 = 1;
            }
            else {
                sysml_conditional_137 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_136) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_137) * 10000))) / 100)))) {
                sysml_conditional_138 = 1;
            }
            else {
                sysml_conditional_138 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_139 = 1;
            }
            else {
                sysml_conditional_139 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_140 = 1;
            }
            else {
                sysml_conditional_140 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_141 = 1;
            }
            else {
                sysml_conditional_141 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_142 = 1;
            }
            else {
                sysml_conditional_142 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_141) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_142) * 10000))) / 100)))) {
                sysml_conditional_143 = 1;
            }
            else {
                sysml_conditional_143 = 0;
            }
            sysml_state_alarmOne = ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_135) * 10000))) / 100)) - sysml_conditional_138) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_139) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_140) * 10000))) / 100)) - sysml_conditional_143) * 100));
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_144 = 1;
        }
        else {
            sysml_conditional_144 = 0;
        }
        if ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_144) == 3) {
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_145 = 1;
            }
            else {
                sysml_conditional_145 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_146 = 1;
            }
            else {
                sysml_conditional_146 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_147 = 1;
            }
            else {
                sysml_conditional_147 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_146) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_147) * 10000))) / 100)))) {
                sysml_conditional_148 = 1;
            }
            else {
                sysml_conditional_148 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_149 = 1;
            }
            else {
                sysml_conditional_149 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_150 = 1;
            }
            else {
                sysml_conditional_150 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_151 = 1;
            }
            else {
                sysml_conditional_151 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_152 = 1;
            }
            else {
                sysml_conditional_152 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_151) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_152) * 10000))) / 100)))) {
                sysml_conditional_153 = 1;
            }
            else {
                sysml_conditional_153 = 0;
            }
            sysml_state_alarmTwo = ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_145) * 10000))) / 100)) - sysml_conditional_148) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_149) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_150) * 10000))) / 100)) - sysml_conditional_153) * 100));
        }
        if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
            sysml_conditional_154 = 1;
        }
        else {
            sysml_conditional_154 = 0;
        }
        if ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_154) == 4) {
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_155 = 1;
            }
            else {
                sysml_conditional_155 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_156 = 1;
            }
            else {
                sysml_conditional_156 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_157 = 1;
            }
            else {
                sysml_conditional_157 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_156) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_157) * 10000))) / 100)))) {
                sysml_conditional_158 = 1;
            }
            else {
                sysml_conditional_158 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_159 = 1;
            }
            else {
                sysml_conditional_159 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_160 = 1;
            }
            else {
                sysml_conditional_160 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_161 = 1;
            }
            else {
                sysml_conditional_161 = 0;
            }
            if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
                sysml_conditional_162 = 1;
            }
            else {
                sysml_conditional_162 = 0;
            }
            if ((((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_161) * 10000))) / 100) < ((double)((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_162) * 10000))) / 100)))) {
                sysml_conditional_163 = 1;
            }
            else {
                sysml_conditional_163 = 0;
            }
            sysml_state_alarmThree = ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_155) * 10000))) / 100)) - sysml_conditional_158) * 60) + ((io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_159) * 10000)) - ((((int32_t)(((double)(io_0 - ((((int32_t)(((double)io_0) / 10000)) - sysml_conditional_160) * 10000))) / 100)) - sysml_conditional_163) * 100));
        }
    }
    if (sysml_state_confirmPress) {
        sysml_state_displayPage = 0;
        sysml_lazy_164 = sysml_state_statusOne == 1;
        if (sysml_lazy_164) {
            sysml_lazy_164 = sysml_state_ageOne <= 30000;
        }
        if (sysml_lazy_164) {
            if (sysml_state_activeTodayOne) {
                sysml_conditional_165 = 2;
            }
            else {
                sysml_conditional_165 = 0;
            }
            sysml_state_statusOne = sysml_conditional_165;
            sysml_state_ageOne = 0;
        }
        else {
            sysml_lazy_166 = sysml_state_statusTwo == 1;
            if (sysml_lazy_166) {
                sysml_lazy_166 = sysml_state_ageTwo <= 30000;
            }
            if (sysml_lazy_166) {
                if (sysml_state_activeTodayTwo) {
                    sysml_conditional_167 = 2;
                }
                else {
                    sysml_conditional_167 = 0;
                }
                sysml_state_statusTwo = sysml_conditional_167;
                sysml_state_ageTwo = 0;
            }
            else {
                sysml_lazy_168 = sysml_state_statusThree == 1;
                if (sysml_lazy_168) {
                    sysml_lazy_168 = sysml_state_ageThree <= 30000;
                }
                if (sysml_lazy_168) {
                    if (sysml_state_activeTodayThree) {
                        sysml_conditional_169 = 2;
                    }
                    else {
                        sysml_conditional_169 = 0;
                    }
                    sysml_state_statusThree = sysml_conditional_169;
                    sysml_state_ageThree = 0;
                }
            }
        }
    }
    sysml_lazy_170 = sysml_state_statusOne == 1;
    if (sysml_lazy_170) {
        sysml_lazy_170 = sysml_state_ageOne >= 30000;
    }
    if (sysml_lazy_170) {
        if (sysml_state_activeTodayOne) {
            sysml_conditional_171 = 3;
        }
        else {
            sysml_conditional_171 = 0;
        }
        sysml_state_statusOne = sysml_conditional_171;
        sysml_state_ageOne = 0;
        sysml_state_batchMisses = (sysml_state_batchMisses + 1);
    }
    sysml_lazy_172 = sysml_state_statusTwo == 1;
    if (sysml_lazy_172) {
        sysml_lazy_172 = sysml_state_ageTwo >= 30000;
    }
    if (sysml_lazy_172) {
        if (sysml_state_activeTodayTwo) {
            sysml_conditional_173 = 3;
        }
        else {
            sysml_conditional_173 = 0;
        }
        sysml_state_statusTwo = sysml_conditional_173;
        sysml_state_ageTwo = 0;
        sysml_state_batchMisses = (sysml_state_batchMisses + 1);
    }
    sysml_lazy_174 = sysml_state_statusThree == 1;
    if (sysml_lazy_174) {
        sysml_lazy_174 = sysml_state_ageThree >= 30000;
    }
    if (sysml_lazy_174) {
        if (sysml_state_activeTodayThree) {
            sysml_conditional_175 = 3;
        }
        else {
            sysml_conditional_175 = 0;
        }
        sysml_state_statusThree = sysml_conditional_175;
        sysml_state_ageThree = 0;
        sysml_state_batchMisses = (sysml_state_batchMisses + 1);
    }
    if ((sysml_state_batchMisses > 0)) {
        if (sysml_state_batchMisses >= (2147483647 - sysml_state_missedCount)) {
            sysml_conditional_176 = 2147483647;
        }
        else {
            sysml_conditional_176 = sysml_state_missedCount + sysml_state_batchMisses;
        }
        sysml_state_missedCount = sysml_conditional_176;
        if (sysml_state_batchMisses >= (2147483647 - sysml_state_pendingWarnings)) {
            sysml_conditional_177 = 2147483647;
        }
        else {
            sysml_conditional_177 = sysml_state_pendingWarnings + sysml_state_batchMisses;
        }
        sysml_state_pendingWarnings = sysml_conditional_177;
    }
    if ((sysml_state_pendingWarnings > 0)) {
        sysml_state_warningStep = true;
        sysml_state_pendingWarnings = (sysml_state_pendingWarnings - 1);
    }
    sysml_lazy_178 = sysml_state_previousPress;
    if (sysml_lazy_178) {
        sysml_lazy_178 = !sysml_state_nextPress;
    }
    sysml_lazy_179 = sysml_lazy_178;
    if (sysml_lazy_179) {
        sysml_lazy_179 = !sysml_state_confirmPress;
    }
    if (sysml_lazy_179) {
        if (sysml_state_displayPage == 0) {
            sysml_conditional_180 = 3;
        }
        else {
            sysml_conditional_180 = sysml_state_displayPage - 1;
        }
        sysml_state_displayPage = sysml_conditional_180;
    }
    sysml_lazy_181 = sysml_state_nextPress;
    if (sysml_lazy_181) {
        sysml_lazy_181 = !sysml_state_previousPress;
    }
    sysml_lazy_182 = sysml_lazy_181;
    if (sysml_lazy_182) {
        sysml_lazy_182 = !sysml_state_confirmPress;
    }
    if (sysml_lazy_182) {
        if (sysml_state_displayPage == 3) {
            sysml_conditional_183 = 0;
        }
        else {
            sysml_conditional_183 = sysml_state_displayPage + 1;
        }
        sysml_state_displayPage = sysml_conditional_183;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = ((sysml_state_oldClockMs * 1.0) + io_8);
    int32_t sysml_value_1 = 0;
    if ((((double)io_0) / 10000) < ((double)((int32_t)(((double)io_0) / 10000)))) {
        sysml_conditional_184 = 1;
    }
    else {
        sysml_conditional_184 = 0;
    }
    sysml_value_1 = ((int32_t)(((double)io_0) / 10000)) - sysml_conditional_184;
    int32_t sysml_value_2 = 0;
    if ((((double)sysml_state_clockMs) / 3600000) < ((double)((int32_t)(((double)sysml_state_clockMs) / 3600000)))) {
        sysml_conditional_185 = 1;
    }
    else {
        sysml_conditional_185 = 0;
    }
    sysml_value_2 = ((int32_t)(((double)sysml_state_clockMs) / 3600000)) - sysml_conditional_185;
    bool sysml_value_3 = 0;
    sysml_lazy_186 = sysml_state_statusOne == 1;
    if (sysml_lazy_186) {
        if ((((double)sysml_state_ageOne) / 1000) < ((double)((int32_t)(((double)sysml_state_ageOne) / 1000)))) {
            sysml_conditional_187 = 1;
        }
        else {
            sysml_conditional_187 = 0;
        }
        sysml_arithmetic_188 = ((int32_t)(((double)sysml_state_ageOne) / 1000)) - sysml_conditional_187;
        sysml_lazy_186 = (sysml_state_ageOne - (1000 * sysml_arithmetic_188)) < 500;
    }
    sysml_value_3 = sysml_lazy_186;
    bool sysml_value_4 = 0;
    sysml_lazy_189 = sysml_state_statusTwo == 1;
    if (sysml_lazy_189) {
        if ((((double)sysml_state_ageTwo) / 1000) < ((double)((int32_t)(((double)sysml_state_ageTwo) / 1000)))) {
            sysml_conditional_190 = 1;
        }
        else {
            sysml_conditional_190 = 0;
        }
        sysml_arithmetic_191 = ((int32_t)(((double)sysml_state_ageTwo) / 1000)) - sysml_conditional_190;
        sysml_lazy_189 = (sysml_state_ageTwo - (1000 * sysml_arithmetic_191)) < 500;
    }
    sysml_value_4 = sysml_lazy_189;
    bool sysml_value_5 = 0;
    sysml_lazy_192 = sysml_state_statusThree == 1;
    if (sysml_lazy_192) {
        if ((((double)sysml_state_ageThree) / 1000) < ((double)((int32_t)(((double)sysml_state_ageThree) / 1000)))) {
            sysml_conditional_193 = 1;
        }
        else {
            sysml_conditional_193 = 0;
        }
        sysml_arithmetic_194 = ((int32_t)(((double)sysml_state_ageThree) / 1000)) - sysml_conditional_193;
        sysml_lazy_192 = (sysml_state_ageThree - (1000 * sysml_arithmetic_194)) < 500;
    }
    sysml_value_5 = sysml_lazy_192;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_warningStep;
    int32_t sysml_value_7 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
        sysml_conditional_195 = 1;
    }
    else {
        sysml_conditional_195 = 0;
    }
    if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
        sysml_conditional_196 = 1;
    }
    else {
        sysml_conditional_196 = 0;
    }
    sysml_value_7 = (((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_195) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_196);
    int32_t sysml_value_8 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
        sysml_conditional_197 = 1;
    }
    else {
        sysml_conditional_197 = 0;
    }
    if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
        sysml_conditional_198 = 1;
    }
    else {
        sysml_conditional_198 = 0;
    }
    sysml_value_8 = (((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_197) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_198);
    int32_t sysml_value_9 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
        sysml_conditional_199 = 1;
    }
    else {
        sysml_conditional_199 = 0;
    }
    if ((((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
        sysml_conditional_200 = 1;
    }
    else {
        sysml_conditional_200 = 0;
    }
    sysml_value_9 = (((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_199) - (((int32_t)(((double)(sysml_state_oldClockMs - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_200);
    int32_t sysml_value_10 = 0;
    if ((((double)sysml_value_0) / 86400000.0) < ((double)((int32_t)(((double)sysml_value_0) / 86400000.0)))) {
        sysml_conditional_201 = 1;
    }
    else {
        sysml_conditional_201 = 0;
    }
    sysml_value_10 = ((int32_t)(((double)sysml_value_0) / 86400000.0)) - sysml_conditional_201;
    int32_t sysml_value_11 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
        sysml_conditional_202 = 1;
    }
    else {
        sysml_conditional_202 = 0;
    }
    sysml_arithmetic_203 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_202;
    if ((((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
        sysml_conditional_204 = 1;
    }
    else {
        sysml_conditional_204 = 0;
    }
    sysml_arithmetic_205 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_204;
    if ((((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)))) {
        sysml_conditional_206 = 1;
    }
    else {
        sysml_conditional_206 = 0;
    }
    sysml_arithmetic_207 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmOne * 60000.0))) / 86400000.0)) - sysml_conditional_206;
    if (((sysml_value_0 - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_205)) < ((double)((int32_t)((sysml_value_0 - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_207))))) {
        sysml_conditional_208 = 1;
    }
    else {
        sysml_conditional_208 = 0;
    }
    sysml_value_11 = ((int32_t)((sysml_value_0 - (sysml_state_alarmOne * 60000.0)) - (86400000.0 * sysml_arithmetic_203))) - sysml_conditional_208;
    int32_t sysml_value_12 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
        sysml_conditional_209 = 1;
    }
    else {
        sysml_conditional_209 = 0;
    }
    sysml_arithmetic_210 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_209;
    if ((((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
        sysml_conditional_211 = 1;
    }
    else {
        sysml_conditional_211 = 0;
    }
    sysml_arithmetic_212 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_211;
    if ((((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)))) {
        sysml_conditional_213 = 1;
    }
    else {
        sysml_conditional_213 = 0;
    }
    sysml_arithmetic_214 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmTwo * 60000.0))) / 86400000.0)) - sysml_conditional_213;
    if (((sysml_value_0 - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_212)) < ((double)((int32_t)((sysml_value_0 - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_214))))) {
        sysml_conditional_215 = 1;
    }
    else {
        sysml_conditional_215 = 0;
    }
    sysml_value_12 = ((int32_t)((sysml_value_0 - (sysml_state_alarmTwo * 60000.0)) - (86400000.0 * sysml_arithmetic_210))) - sysml_conditional_215;
    int32_t sysml_value_13 = 0;
    if ((((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
        sysml_conditional_216 = 1;
    }
    else {
        sysml_conditional_216 = 0;
    }
    sysml_arithmetic_217 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_216;
    if ((((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
        sysml_conditional_218 = 1;
    }
    else {
        sysml_conditional_218 = 0;
    }
    sysml_arithmetic_219 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_218;
    if ((((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0) < ((double)((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)))) {
        sysml_conditional_220 = 1;
    }
    else {
        sysml_conditional_220 = 0;
    }
    sysml_arithmetic_221 = ((int32_t)(((double)(sysml_value_0 - (sysml_state_alarmThree * 60000.0))) / 86400000.0)) - sysml_conditional_220;
    if (((sysml_value_0 - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_219)) < ((double)((int32_t)((sysml_value_0 - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_221))))) {
        sysml_conditional_222 = 1;
    }
    else {
        sysml_conditional_222 = 0;
    }
    sysml_value_13 = ((int32_t)((sysml_value_0 - (sysml_state_alarmThree * 60000.0)) - (86400000.0 * sysml_arithmetic_217))) - sysml_conditional_222;
    int32_t sysml_value_14 = 0;
    sysml_value_14 = (io_0 - (sysml_value_1 * 10000));
    int32_t sysml_value_15 = 0;
    if ((((double)sysml_state_clockMs) / 60000) < ((double)((int32_t)(((double)sysml_state_clockMs) / 60000)))) {
        sysml_conditional_223 = 1;
    }
    else {
        sysml_conditional_223 = 0;
    }
    sysml_value_15 = (((int32_t)(((double)sysml_state_clockMs) / 60000)) - sysml_conditional_223) - (sysml_value_2 * 60);
    bool sysml_value_16 = 0;
    sysml_lazy_224 = sysml_value_3;
    if (!sysml_lazy_224) {
        sysml_lazy_224 = sysml_value_4;
    }
    sysml_lazy_225 = sysml_lazy_224;
    if (!sysml_lazy_225) {
        sysml_lazy_225 = sysml_value_5;
    }
    sysml_value_16 = sysml_lazy_225;
    int32_t sysml_value_17 = 0;
    if ((((double)sysml_value_14) / 100) < ((double)((int32_t)(((double)sysml_value_14) / 100)))) {
        sysml_conditional_226 = 1;
    }
    else {
        sysml_conditional_226 = 0;
    }
    sysml_value_17 = ((int32_t)(((double)sysml_value_14) / 100)) - sysml_conditional_226;
    int32_t sysml_value_18 = 0;
    if (sysml_state_displayPage == 0) {
        sysml_conditional_227 = (sysml_value_2 * 100) + sysml_value_15;
    }
    else {
        if (sysml_state_displayPage == 1) {
            sysml_conditional_228 = 1000 + sysml_state_statusOne;
        }
        else {
            if (sysml_state_displayPage == 2) {
                sysml_conditional_229 = 2000 + sysml_state_statusTwo;
            }
            else {
                sysml_conditional_229 = 3000 + sysml_state_statusThree;
            }
            sysml_conditional_228 = sysml_conditional_229;
        }
        sysml_conditional_227 = sysml_conditional_228;
    }
    sysml_value_18 = sysml_conditional_227;
    int32_t sysml_value_19 = 0;
    if (sysml_value_16) {
        sysml_conditional_230 = 2000;
    }
    else {
        sysml_conditional_230 = 0;
    }
    sysml_value_19 = sysml_conditional_230;
    bool sysml_value_20 = 0;
    sysml_value_20 = sysml_value_16;
    int32_t sysml_value_21 = 0;
    sysml_value_21 = (sysml_value_14 - (sysml_value_17 * 100));
    bool sysml_value_22 = 0;
    sysml_lazy_231 = io_0_valid;
    if (sysml_lazy_231) {
        sysml_lazy_231 = sysml_value_14 >= 0;
    }
    sysml_lazy_232 = sysml_lazy_231;
    if (sysml_lazy_232) {
        sysml_lazy_232 = sysml_value_14 <= 2359;
    }
    sysml_lazy_233 = sysml_lazy_232;
    if (sysml_lazy_233) {
        sysml_lazy_233 = sysml_value_21 >= 0;
    }
    sysml_lazy_234 = sysml_lazy_233;
    if (sysml_lazy_234) {
        sysml_lazy_234 = sysml_value_21 < 60;
    }
    sysml_lazy_235 = sysml_lazy_234;
    if (sysml_lazy_235) {
        sysml_lazy_235 = sysml_value_1 >= 1;
    }
    sysml_lazy_236 = sysml_lazy_235;
    if (sysml_lazy_236) {
        sysml_lazy_236 = sysml_value_1 <= 4;
    }
    sysml_value_22 = sysml_lazy_236;
    int32_t sysml_value_23 = 0;
    sysml_value_23 = ((sysml_value_17 * 60) + sysml_value_21);
    *io_4 = sysml_value_19;
    *io_5 = sysml_value_20;
    *io_6 = sysml_value_18;
    *io_7 = sysml_value_6;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <TM1637Display.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static char hw_rx_text[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\0';
  char* begin = hw_rx_buffer;
  while (*begin == ' ' || *begin == '\t') ++begin;
  char* end = hw_rx_buffer + hw_rx_size;
  while (end > begin && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
  if (!*begin) return false;
  char* space=strchr(begin,' ');if(!space) return false;*space++='\0';
  while(*space==' '||*space=='\t')++space;if(!*space)return false;
  uint32_t payload=0;
  for(char* cursor=space;*cursor;++cursor){if(*cursor<'0'||*cursor>'9')return false;payload=payload*10+(*cursor-'0');if(payload>9999)return false;}
  if(payload<0 || payload>2359) return false;
  if(strcmp(begin,"TIME")==0) {*value=1L*10000L+(int32_t)payload;return true;}
  if(strcmp(begin,"SET1")==0) {*value=2L*10000L+(int32_t)payload;return true;}
  if(strcmp(begin,"SET2")==0) {*value=3L*10000L+(int32_t)payload;return true;}
  if(strcmp(begin,"SET3")==0) {*value=4L*10000L+(int32_t)payload;return true;}
  return false;
}
static bool hw_rx_finish(int32_t* value) {
  bool accepted = !hw_rx_overflow && hw_rx_parse(value);
  hw_rx_size = 0; hw_rx_overflow = false;
  return accepted;
}
static bool hw_rx_poll(uint32_t hw_now, int32_t* value) {
  for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
    int byte = Serial.read();
    if (byte < 0) break;
    hw_rx_last_byte = hw_now;
    if (byte == '\r' || byte == '\n') {
      if (!hw_rx_size && !hw_rx_overflow) continue;
      return hw_rx_finish(value);
    }
    if (byte == 0 || byte > 127) hw_rx_overflow = true;
    if (hw_rx_size < 63) hw_rx_buffer[hw_rx_size++] = (char)byte;
    else hw_rx_overflow = true;
  }
  if (Serial.available() > 0) return false;
  if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= 20UL) return hw_rx_finish(value);
  return false;
}
static bool hw_value_io_0_valid = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static int32_t hw_value_io_4 = 0;
static int32_t hw_tone_io_4 = -1;
static bool hw_value_io_5 = false;
static int32_t hw_value_io_6 = 0;
TM1637Display hw_display_io_6(7, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static bool hw_value_io_7 = false;
static int32_t hw_value_io_8 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < 0 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, true, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_8 = (int32_t)hw_elapsed;
  hw_value_io_0_valid = hw_rx_poll(hw_now, &hw_value_io_0);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_0_valid, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_8, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_7);
  int32_t hw_tone_command_io_4 = (hw_value_io_4 >= 31 && hw_value_io_4 <= 65535) ? hw_value_io_4 : 0;
  if (hw_tone_command_io_4 != hw_tone_io_4) {
    if (hw_tone_command_io_4 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_4);
    hw_tone_io_4 = hw_tone_command_io_4;
  }
  digitalWrite(13, hw_value_io_5 ? HIGH : LOW);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < 0 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, true, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_io_7) Serial.println("missed dose");
}

