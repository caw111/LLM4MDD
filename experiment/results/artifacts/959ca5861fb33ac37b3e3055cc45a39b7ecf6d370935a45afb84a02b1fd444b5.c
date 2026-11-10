#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_valueNumber = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2Pressed = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3Pressed = false;
/* Global Variable define of  */
int sysml_state_debounceBtn1ActiveMs = 0;
/* Global Variable define of  */
int sysml_state_debounceBtn2ActiveMs = 0;
/* Global Variable define of  */
int sysml_state_debounceBtn3ActiveMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
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
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
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
    int sysml_value_0 = 0;
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