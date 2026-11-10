#line 1 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
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
extern int32_t sysml_state_valueNumber;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3Pressed;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn1ActiveMs;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn2ActiveMs;
/* Global Variable define of  */
extern int32_t sysml_state_debounceBtn3ActiveMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_valueNumber = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3Pressed = false;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn1ActiveMs = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn2ActiveMs = 0;
/* Global Variable define of  */
int32_t sysml_state_debounceBtn3ActiveMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
#line 646 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
static void hw_shift595_io_3(int32_t value);
#line 690 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void setup();
#line 723 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void loop();
#line 53 "C:\\Users\\27872\\Desktop\\work\\GeneratedCodeOfMing\\LLM4MDD\\experiment\\output\\paper_ablation_v2_20261017\\jobs\\embedbench_81\\repeat_1\\workflow\\attempt_0\\sketch\\sketch.ino"
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_valueNumber = 0;
    sysml_state_prevBtn1Pressed = false;
    sysml_state_prevBtn2Pressed = false;
    sysml_state_prevBtn3Pressed = false;
    sysml_state_debounceBtn1ActiveMs = 0;
    sysml_state_debounceBtn2ActiveMs = 0;
    sysml_state_debounceBtn3ActiveMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int32_t* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int32_t sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_lazy_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_lazy_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_lazy_47 = 0;
    bool sysml_lazy_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_lazy_52 = 0;
    bool sysml_lazy_53 = 0;
    bool sysml_lazy_54 = 0;
    bool sysml_lazy_55 = 0;
    bool sysml_lazy_56 = 0;
    bool sysml_lazy_57 = 0;
    bool sysml_lazy_58 = 0;
    bool sysml_lazy_59 = 0;
    bool sysml_lazy_60 = 0;
    bool sysml_lazy_61 = 0;
    bool sysml_lazy_62 = 0;
    bool sysml_lazy_63 = 0;
    bool sysml_lazy_64 = 0;
    bool sysml_lazy_65 = 0;
    bool sysml_lazy_66 = 0;
    bool sysml_lazy_67 = 0;
    bool sysml_lazy_68 = 0;
    bool sysml_lazy_69 = 0;
    bool sysml_lazy_70 = 0;
    bool sysml_lazy_71 = 0;
    bool sysml_lazy_72 = 0;
    bool sysml_lazy_73 = 0;
    bool sysml_lazy_74 = 0;
    bool sysml_lazy_75 = 0;
    bool sysml_lazy_76 = 0;
    bool sysml_lazy_77 = 0;
    bool sysml_lazy_78 = 0;
    bool sysml_lazy_79 = 0;
    bool sysml_lazy_80 = 0;
    bool sysml_lazy_81 = 0;
    bool sysml_lazy_82 = 0;
    bool sysml_lazy_83 = 0;
    bool sysml_lazy_84 = 0;
    bool sysml_lazy_85 = 0;
    bool sysml_lazy_86 = 0;
    bool sysml_lazy_87 = 0;
    bool sysml_lazy_88 = 0;
    bool sysml_lazy_89 = 0;
    bool sysml_lazy_90 = 0;
    bool sysml_lazy_91 = 0;
    bool sysml_lazy_92 = 0;
    if (io_2) {
        if (sysml_state_debounceBtn3ActiveMs > 0) {
            sysml_conditional_0 = sysml_state_debounceBtn3ActiveMs;
        }
        else {
            sysml_conditional_0 = 15;
        }
        sysml_state_debounceBtn3ActiveMs = sysml_conditional_0;
    }
    else {
        if (sysml_state_debounceBtn3ActiveMs > 0) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_debounceBtn3ActiveMs = sysml_conditional_1;
    }
    if ((sysml_state_debounceBtn3ActiveMs > 0)) {
        sysml_state_debounceBtn3ActiveMs = 0;
    }
    if (io_0) {
        if (sysml_state_debounceBtn1ActiveMs > 0) {
            sysml_conditional_2 = sysml_state_debounceBtn1ActiveMs;
        }
        else {
            sysml_conditional_2 = 15;
        }
        sysml_state_debounceBtn1ActiveMs = sysml_conditional_2;
    }
    else {
        if (sysml_state_debounceBtn1ActiveMs > 0) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_state_debounceBtn1ActiveMs = sysml_conditional_3;
    }
    if ((sysml_state_debounceBtn1ActiveMs > 0)) {
        sysml_state_debounceBtn1ActiveMs = 0;
    }
    if (io_1) {
        if (sysml_state_debounceBtn2ActiveMs > 0) {
            sysml_conditional_4 = sysml_state_debounceBtn2ActiveMs;
        }
        else {
            sysml_conditional_4 = 15;
        }
        sysml_state_debounceBtn2ActiveMs = sysml_conditional_4;
    }
    else {
        if (sysml_state_debounceBtn2ActiveMs > 0) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_state_debounceBtn2ActiveMs = sysml_conditional_5;
    }
    if ((sysml_state_debounceBtn2ActiveMs > 0)) {
        sysml_state_debounceBtn2ActiveMs = 0;
    }
    sysml_lazy_6 = io_2;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevBtn3Pressed;
    }
    if (sysml_lazy_6) {
        sysml_state_valueNumber = 0;
    }
    sysml_lazy_7 = io_2;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevBtn3Pressed;
    }
    if (sysml_lazy_7) {
    }
    else {
        sysml_lazy_8 = io_0;
        if (sysml_lazy_8) {
            sysml_lazy_8 = !sysml_state_prevBtn1Pressed;
        }
        if (sysml_lazy_8) {
            if ((sysml_state_valueNumber + 1) > 99) {
                sysml_conditional_9 = 0;
            }
            else {
                sysml_conditional_9 = sysml_state_valueNumber + 1;
            }
            sysml_state_valueNumber = sysml_conditional_9;
        }
        else {
            sysml_lazy_10 = io_1;
            if (sysml_lazy_10) {
                sysml_lazy_10 = !sysml_state_prevBtn2Pressed;
            }
            if (sysml_lazy_10) {
                if ((sysml_state_valueNumber + 5) > 99) {
                    sysml_conditional_11 = 0;
                }
                else {
                    sysml_conditional_11 = sysml_state_valueNumber + 5;
                }
                sysml_state_valueNumber = sysml_conditional_11;
            }
        }
    }
    sysml_state_prevBtn1Pressed = io_0;
    sysml_state_prevBtn2Pressed = io_1;
    sysml_state_prevBtn3Pressed = io_2;
    int32_t sysml_value_0 = 0;
    sysml_value_0 = sysml_state_valueNumber;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_valueNumber == 0);
    bool sysml_value_2 = 0;
    sysml_lazy_12 = sysml_state_valueNumber >= 10;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_valueNumber <= 19;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_14 = sysml_state_valueNumber >= 20;
        if (sysml_lazy_14) {
            sysml_lazy_14 = sysml_state_valueNumber <= 29;
        }
        sysml_lazy_13 = sysml_lazy_14;
    }
    sysml_lazy_15 = sysml_lazy_13;
    if (!sysml_lazy_15) {
        sysml_lazy_16 = sysml_state_valueNumber >= 30;
        if (sysml_lazy_16) {
            sysml_lazy_16 = sysml_state_valueNumber <= 39;
        }
        sysml_lazy_15 = sysml_lazy_16;
    }
    sysml_lazy_17 = sysml_lazy_15;
    if (!sysml_lazy_17) {
        sysml_lazy_18 = sysml_state_valueNumber >= 40;
        if (sysml_lazy_18) {
            sysml_lazy_18 = sysml_state_valueNumber <= 49;
        }
        sysml_lazy_17 = sysml_lazy_18;
    }
    sysml_lazy_19 = sysml_lazy_17;
    if (!sysml_lazy_19) {
        sysml_lazy_20 = sysml_state_valueNumber >= 50;
        if (sysml_lazy_20) {
            sysml_lazy_20 = sysml_state_valueNumber <= 59;
        }
        sysml_lazy_19 = sysml_lazy_20;
    }
    sysml_lazy_21 = sysml_lazy_19;
    if (!sysml_lazy_21) {
        sysml_lazy_22 = sysml_state_valueNumber >= 60;
        if (sysml_lazy_22) {
            sysml_lazy_22 = sysml_state_valueNumber <= 69;
        }
        sysml_lazy_21 = sysml_lazy_22;
    }
    sysml_lazy_23 = sysml_lazy_21;
    if (!sysml_lazy_23) {
        sysml_lazy_24 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_24) {
            sysml_lazy_24 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_23 = sysml_lazy_24;
    }
    sysml_lazy_25 = sysml_lazy_23;
    if (!sysml_lazy_25) {
        sysml_lazy_26 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_26) {
            sysml_lazy_26 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_25 = sysml_lazy_26;
    }
    sysml_lazy_27 = sysml_lazy_25;
    if (!sysml_lazy_27) {
        sysml_lazy_28 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_28) {
            sysml_lazy_28 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_27 = sysml_lazy_28;
    }
    sysml_value_2 = sysml_lazy_27;
    bool sysml_value_3 = 0;
    sysml_lazy_29 = sysml_state_valueNumber >= 20;
    if (sysml_lazy_29) {
        sysml_lazy_29 = sysml_state_valueNumber <= 29;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (!sysml_lazy_30) {
        sysml_lazy_31 = sysml_state_valueNumber >= 30;
        if (sysml_lazy_31) {
            sysml_lazy_31 = sysml_state_valueNumber <= 39;
        }
        sysml_lazy_30 = sysml_lazy_31;
    }
    sysml_lazy_32 = sysml_lazy_30;
    if (!sysml_lazy_32) {
        sysml_lazy_33 = sysml_state_valueNumber >= 40;
        if (sysml_lazy_33) {
            sysml_lazy_33 = sysml_state_valueNumber <= 49;
        }
        sysml_lazy_32 = sysml_lazy_33;
    }
    sysml_lazy_34 = sysml_lazy_32;
    if (!sysml_lazy_34) {
        sysml_lazy_35 = sysml_state_valueNumber >= 50;
        if (sysml_lazy_35) {
            sysml_lazy_35 = sysml_state_valueNumber <= 59;
        }
        sysml_lazy_34 = sysml_lazy_35;
    }
    sysml_lazy_36 = sysml_lazy_34;
    if (!sysml_lazy_36) {
        sysml_lazy_37 = sysml_state_valueNumber >= 60;
        if (sysml_lazy_37) {
            sysml_lazy_37 = sysml_state_valueNumber <= 69;
        }
        sysml_lazy_36 = sysml_lazy_37;
    }
    sysml_lazy_38 = sysml_lazy_36;
    if (!sysml_lazy_38) {
        sysml_lazy_39 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_39) {
            sysml_lazy_39 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_38 = sysml_lazy_39;
    }
    sysml_lazy_40 = sysml_lazy_38;
    if (!sysml_lazy_40) {
        sysml_lazy_41 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_41) {
            sysml_lazy_41 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_40 = sysml_lazy_41;
    }
    sysml_lazy_42 = sysml_lazy_40;
    if (!sysml_lazy_42) {
        sysml_lazy_43 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_43) {
            sysml_lazy_43 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_42 = sysml_lazy_43;
    }
    sysml_value_3 = sysml_lazy_42;
    bool sysml_value_4 = 0;
    sysml_lazy_44 = sysml_state_valueNumber >= 30;
    if (sysml_lazy_44) {
        sysml_lazy_44 = sysml_state_valueNumber <= 39;
    }
    sysml_lazy_45 = sysml_lazy_44;
    if (!sysml_lazy_45) {
        sysml_lazy_46 = sysml_state_valueNumber >= 40;
        if (sysml_lazy_46) {
            sysml_lazy_46 = sysml_state_valueNumber <= 49;
        }
        sysml_lazy_45 = sysml_lazy_46;
    }
    sysml_lazy_47 = sysml_lazy_45;
    if (!sysml_lazy_47) {
        sysml_lazy_48 = sysml_state_valueNumber >= 50;
        if (sysml_lazy_48) {
            sysml_lazy_48 = sysml_state_valueNumber <= 59;
        }
        sysml_lazy_47 = sysml_lazy_48;
    }
    sysml_lazy_49 = sysml_lazy_47;
    if (!sysml_lazy_49) {
        sysml_lazy_50 = sysml_state_valueNumber >= 60;
        if (sysml_lazy_50) {
            sysml_lazy_50 = sysml_state_valueNumber <= 69;
        }
        sysml_lazy_49 = sysml_lazy_50;
    }
    sysml_lazy_51 = sysml_lazy_49;
    if (!sysml_lazy_51) {
        sysml_lazy_52 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_52) {
            sysml_lazy_52 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_51 = sysml_lazy_52;
    }
    sysml_lazy_53 = sysml_lazy_51;
    if (!sysml_lazy_53) {
        sysml_lazy_54 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_54) {
            sysml_lazy_54 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_53 = sysml_lazy_54;
    }
    sysml_lazy_55 = sysml_lazy_53;
    if (!sysml_lazy_55) {
        sysml_lazy_56 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_56) {
            sysml_lazy_56 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_55 = sysml_lazy_56;
    }
    sysml_value_4 = sysml_lazy_55;
    bool sysml_value_5 = 0;
    sysml_lazy_57 = sysml_state_valueNumber >= 40;
    if (sysml_lazy_57) {
        sysml_lazy_57 = sysml_state_valueNumber <= 49;
    }
    sysml_lazy_58 = sysml_lazy_57;
    if (!sysml_lazy_58) {
        sysml_lazy_59 = sysml_state_valueNumber >= 50;
        if (sysml_lazy_59) {
            sysml_lazy_59 = sysml_state_valueNumber <= 59;
        }
        sysml_lazy_58 = sysml_lazy_59;
    }
    sysml_lazy_60 = sysml_lazy_58;
    if (!sysml_lazy_60) {
        sysml_lazy_61 = sysml_state_valueNumber >= 60;
        if (sysml_lazy_61) {
            sysml_lazy_61 = sysml_state_valueNumber <= 69;
        }
        sysml_lazy_60 = sysml_lazy_61;
    }
    sysml_lazy_62 = sysml_lazy_60;
    if (!sysml_lazy_62) {
        sysml_lazy_63 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_63) {
            sysml_lazy_63 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_62 = sysml_lazy_63;
    }
    sysml_lazy_64 = sysml_lazy_62;
    if (!sysml_lazy_64) {
        sysml_lazy_65 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_65) {
            sysml_lazy_65 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_64 = sysml_lazy_65;
    }
    sysml_lazy_66 = sysml_lazy_64;
    if (!sysml_lazy_66) {
        sysml_lazy_67 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_67) {
            sysml_lazy_67 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_66 = sysml_lazy_67;
    }
    sysml_value_5 = sysml_lazy_66;
    bool sysml_value_6 = 0;
    sysml_lazy_68 = sysml_state_valueNumber >= 50;
    if (sysml_lazy_68) {
        sysml_lazy_68 = sysml_state_valueNumber <= 59;
    }
    sysml_lazy_69 = sysml_lazy_68;
    if (!sysml_lazy_69) {
        sysml_lazy_70 = sysml_state_valueNumber >= 60;
        if (sysml_lazy_70) {
            sysml_lazy_70 = sysml_state_valueNumber <= 69;
        }
        sysml_lazy_69 = sysml_lazy_70;
    }
    sysml_lazy_71 = sysml_lazy_69;
    if (!sysml_lazy_71) {
        sysml_lazy_72 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_72) {
            sysml_lazy_72 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_71 = sysml_lazy_72;
    }
    sysml_lazy_73 = sysml_lazy_71;
    if (!sysml_lazy_73) {
        sysml_lazy_74 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_74) {
            sysml_lazy_74 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_73 = sysml_lazy_74;
    }
    sysml_lazy_75 = sysml_lazy_73;
    if (!sysml_lazy_75) {
        sysml_lazy_76 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_76) {
            sysml_lazy_76 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_75 = sysml_lazy_76;
    }
    sysml_value_6 = sysml_lazy_75;
    bool sysml_value_7 = 0;
    sysml_lazy_77 = sysml_state_valueNumber >= 60;
    if (sysml_lazy_77) {
        sysml_lazy_77 = sysml_state_valueNumber <= 69;
    }
    sysml_lazy_78 = sysml_lazy_77;
    if (!sysml_lazy_78) {
        sysml_lazy_79 = sysml_state_valueNumber >= 70;
        if (sysml_lazy_79) {
            sysml_lazy_79 = sysml_state_valueNumber <= 79;
        }
        sysml_lazy_78 = sysml_lazy_79;
    }
    sysml_lazy_80 = sysml_lazy_78;
    if (!sysml_lazy_80) {
        sysml_lazy_81 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_81) {
            sysml_lazy_81 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_80 = sysml_lazy_81;
    }
    sysml_lazy_82 = sysml_lazy_80;
    if (!sysml_lazy_82) {
        sysml_lazy_83 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_83) {
            sysml_lazy_83 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_82 = sysml_lazy_83;
    }
    sysml_value_7 = sysml_lazy_82;
    bool sysml_value_8 = 0;
    sysml_lazy_84 = sysml_state_valueNumber >= 70;
    if (sysml_lazy_84) {
        sysml_lazy_84 = sysml_state_valueNumber <= 79;
    }
    sysml_lazy_85 = sysml_lazy_84;
    if (!sysml_lazy_85) {
        sysml_lazy_86 = sysml_state_valueNumber >= 80;
        if (sysml_lazy_86) {
            sysml_lazy_86 = sysml_state_valueNumber <= 89;
        }
        sysml_lazy_85 = sysml_lazy_86;
    }
    sysml_lazy_87 = sysml_lazy_85;
    if (!sysml_lazy_87) {
        sysml_lazy_88 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_88) {
            sysml_lazy_88 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_87 = sysml_lazy_88;
    }
    sysml_value_8 = sysml_lazy_87;
    bool sysml_value_9 = 0;
    sysml_lazy_89 = sysml_state_valueNumber >= 80;
    if (sysml_lazy_89) {
        sysml_lazy_89 = sysml_state_valueNumber <= 89;
    }
    sysml_lazy_90 = sysml_lazy_89;
    if (!sysml_lazy_90) {
        sysml_lazy_91 = sysml_state_valueNumber >= 90;
        if (sysml_lazy_91) {
            sysml_lazy_91 = sysml_state_valueNumber <= 99;
        }
        sysml_lazy_90 = sysml_lazy_91;
    }
    sysml_value_9 = sysml_lazy_90;
    bool sysml_value_10 = 0;
    sysml_lazy_92 = sysml_state_valueNumber >= 90;
    if (sysml_lazy_92) {
        sysml_lazy_92 = sysml_state_valueNumber <= 99;
    }
    sysml_value_10 = sysml_lazy_92;
    *io_3 = sysml_value_0;
    *io_4_segment_0 = sysml_value_1;
    *io_4_segment_1 = sysml_value_2;
    *io_4_segment_2 = sysml_value_3;
    *io_4_segment_3 = sysml_value_4;
    *io_4_segment_4 = sysml_value_5;
    *io_4_segment_5 = sysml_value_6;
    *io_4_segment_6 = sysml_value_7;
    *io_4_segment_7 = sysml_value_8;
    *io_4_segment_8 = sysml_value_9;
    *io_4_segment_9 = sysml_value_10;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static int32_t hw_value_io_3 = 0;
static void hw_shift595_io_3(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_3 = INT32_MIN;
static bool hw_value_io_4_segment_0 = false;
static bool hw_value_io_4_segment_1 = false;
static bool hw_value_io_4_segment_2 = false;
static bool hw_value_io_4_segment_3 = false;
static bool hw_value_io_4_segment_4 = false;
static bool hw_value_io_4_segment_5 = false;
static bool hw_value_io_4_segment_6 = false;
static bool hw_value_io_4_segment_7 = false;
static bool hw_value_io_4_segment_8 = false;
static bool hw_value_io_4_segment_9 = false;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  pinMode(44, OUTPUT);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, &hw_value_io_3, &hw_value_io_4_segment_0, &hw_value_io_4_segment_1, &hw_value_io_4_segment_2, &hw_value_io_4_segment_3, &hw_value_io_4_segment_4, &hw_value_io_4_segment_5, &hw_value_io_4_segment_6, &hw_value_io_4_segment_7, &hw_value_io_4_segment_8, &hw_value_io_4_segment_9);
  if (hw_value_io_3 != hw_shift595_previous_io_3) {
    hw_shift595_io_3(hw_value_io_3); hw_shift595_previous_io_3 = hw_value_io_3;
  }
  digitalWrite(5, hw_value_io_4_segment_0 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_4_segment_1 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_4_segment_2 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_4_segment_3 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_4_segment_4 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_4_segment_5 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_4_segment_6 ? HIGH : LOW);
  digitalWrite(12, hw_value_io_4_segment_7 ? HIGH : LOW);
  digitalWrite(13, hw_value_io_4_segment_8 ? HIGH : LOW);
  digitalWrite(44, hw_value_io_4_segment_9 ? HIGH : LOW);
}

