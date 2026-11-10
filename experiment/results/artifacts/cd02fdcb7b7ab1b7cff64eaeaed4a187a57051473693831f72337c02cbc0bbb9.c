#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_lazy_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_lazy_36 = 0;
    bool sysml_conditional_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_conditional_41 = 0;
    bool sysml_lazy_42 = 0;
    bool sysml_conditional_43 = 0;
    bool sysml_lazy_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_conditional_47 = 0;
    bool sysml_lazy_48 = 0;
    bool sysml_conditional_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_conditional_51 = 0;
    bool sysml_lazy_52 = 0;
    bool sysml_conditional_53 = 0;
    bool sysml_lazy_54 = 0;
    bool sysml_conditional_55 = 0;
    bool sysml_lazy_56 = 0;
    bool sysml_conditional_57 = 0;
    bool sysml_lazy_58 = 0;
    bool sysml_conditional_59 = 0;
    bool sysml_lazy_60 = 0;
    bool sysml_conditional_61 = 0;
    bool sysml_lazy_62 = 0;
    bool sysml_conditional_63 = 0;
    bool sysml_lazy_64 = 0;
    bool sysml_conditional_65 = 0;
    bool sysml_lazy_66 = 0;
    bool sysml_conditional_67 = 0;
    bool sysml_lazy_68 = 0;
    bool sysml_conditional_69 = 0;
    bool sysml_lazy_70 = 0;
    bool sysml_conditional_71 = 0;
    bool sysml_lazy_72 = 0;
    bool sysml_conditional_73 = 0;
    bool sysml_lazy_74 = 0;
    bool sysml_conditional_75 = 0;
    bool sysml_lazy_76 = 0;
    bool sysml_conditional_77 = 0;
    bool sysml_lazy_78 = 0;
    bool sysml_conditional_79 = 0;
    bool sysml_lazy_80 = 0;
    bool sysml_conditional_81 = 0;
    bool sysml_lazy_82 = 0;
    bool sysml_conditional_83 = 0;
    bool sysml_lazy_84 = 0;
    bool sysml_conditional_85 = 0;
    bool sysml_lazy_86 = 0;
    bool sysml_conditional_87 = 0;
    bool sysml_lazy_88 = 0;
    bool sysml_conditional_89 = 0;
    bool sysml_lazy_90 = 0;
    bool sysml_conditional_91 = 0;
    bool sysml_lazy_92 = 0;
    bool sysml_conditional_93 = 0;
    bool sysml_lazy_94 = 0;
    bool sysml_conditional_95 = 0;
    bool sysml_lazy_96 = 0;
    bool sysml_conditional_97 = 0;
    bool sysml_lazy_98 = 0;
    bool sysml_conditional_99 = 0;
    bool sysml_lazy_100 = 0;
    bool sysml_conditional_101 = 0;
    bool sysml_lazy_102 = 0;
    bool sysml_conditional_103 = 0;
    bool sysml_lazy_104 = 0;
    bool sysml_conditional_105 = 0;
    bool sysml_lazy_106 = 0;
    bool sysml_conditional_107 = 0;
    bool sysml_lazy_108 = 0;
    bool sysml_conditional_109 = 0;
    bool sysml_lazy_110 = 0;
    bool sysml_conditional_111 = 0;
    bool sysml_lazy_112 = 0;
    bool sysml_conditional_113 = 0;
    bool sysml_lazy_114 = 0;
    bool sysml_conditional_115 = 0;
    bool sysml_lazy_116 = 0;
    bool sysml_conditional_117 = 0;
    bool sysml_lazy_118 = 0;
    bool sysml_conditional_119 = 0;
    bool sysml_lazy_120 = 0;
    bool sysml_conditional_121 = 0;
    bool sysml_lazy_122 = 0;
    bool sysml_conditional_123 = 0;
    bool sysml_lazy_124 = 0;
    bool sysml_conditional_125 = 0;
    bool sysml_lazy_126 = 0;
    bool sysml_conditional_127 = 0;
    bool sysml_lazy_128 = 0;
    bool sysml_conditional_129 = 0;
    bool sysml_lazy_130 = 0;
    bool sysml_conditional_131 = 0;
    bool sysml_lazy_132 = 0;
    bool sysml_conditional_133 = 0;
    bool sysml_lazy_134 = 0;
    bool sysml_conditional_135 = 0;
    bool sysml_lazy_136 = 0;
    bool sysml_conditional_137 = 0;
    bool sysml_lazy_138 = 0;
    bool sysml_conditional_139 = 0;
    bool sysml_conditional_140 = 0;
    sysml_lazy_0 = io_8_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_8 == 0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = false;
    }
    else {
        sysml_lazy_2 = io_8_valid;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_8 == 1;
        }
        if (sysml_lazy_2) {
            sysml_conditional_3 = true;
        }
        else {
            sysml_lazy_4 = io_8_valid;
            if (sysml_lazy_4) {
                sysml_lazy_4 = io_8 == 2;
            }
            if (sysml_lazy_4) {
                sysml_conditional_5 = true;
            }
            else {
                sysml_lazy_6 = io_8_valid;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = io_8 == 3;
                }
                if (sysml_lazy_6) {
                    sysml_conditional_7 = true;
                }
                else {
                    sysml_lazy_8 = io_8_valid;
                    if (sysml_lazy_8) {
                        sysml_lazy_8 = io_8 == 4;
                    }
                    if (sysml_lazy_8) {
                        sysml_conditional_9 = true;
                    }
                    else {
                        sysml_lazy_10 = io_8_valid;
                        if (sysml_lazy_10) {
                            sysml_lazy_10 = io_8 == 5;
                        }
                        if (sysml_lazy_10) {
                            sysml_conditional_11 = true;
                        }
                        else {
                            sysml_lazy_12 = io_8_valid;
                            if (sysml_lazy_12) {
                                sysml_lazy_12 = io_8 == 6;
                            }
                            if (sysml_lazy_12) {
                                sysml_conditional_13 = false;
                            }
                            else {
                                sysml_lazy_14 = io_8_valid;
                                if (sysml_lazy_14) {
                                    sysml_lazy_14 = io_8 == 7;
                                }
                                if (sysml_lazy_14) {
                                    sysml_conditional_15 = true;
                                }
                                else {
                                    sysml_lazy_16 = io_8_valid;
                                    if (sysml_lazy_16) {
                                        sysml_lazy_16 = io_8 == 8;
                                    }
                                    if (sysml_lazy_16) {
                                        sysml_conditional_17 = true;
                                    }
                                    else {
                                        sysml_lazy_18 = io_8_valid;
                                        if (sysml_lazy_18) {
                                            sysml_lazy_18 = io_8 == 9;
                                        }
                                        if (sysml_lazy_18) {
                                            sysml_conditional_19 = true;
                                        }
                                        else {
                                            sysml_conditional_19 = false;
                                        }
                                        sysml_conditional_17 = sysml_conditional_19;
                                    }
                                    sysml_conditional_15 = sysml_conditional_17;
                                }
                                sysml_conditional_13 = sysml_conditional_15;
                            }
                            sysml_conditional_11 = sysml_conditional_13;
                        }
                        sysml_conditional_9 = sysml_conditional_11;
                    }
                    sysml_conditional_7 = sysml_conditional_9;
                }
                sysml_conditional_5 = sysml_conditional_7;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_20 = io_8_valid;
    if (sysml_lazy_20) {
        sysml_lazy_20 = io_8 == 0;
    }
    if (sysml_lazy_20) {
        sysml_conditional_21 = true;
    }
    else {
        sysml_lazy_22 = io_8_valid;
        if (sysml_lazy_22) {
            sysml_lazy_22 = io_8 == 1;
        }
        if (sysml_lazy_22) {
            sysml_conditional_23 = true;
        }
        else {
            sysml_lazy_24 = io_8_valid;
            if (sysml_lazy_24) {
                sysml_lazy_24 = io_8 == 2;
            }
            if (sysml_lazy_24) {
                sysml_conditional_25 = true;
            }
            else {
                sysml_lazy_26 = io_8_valid;
                if (sysml_lazy_26) {
                    sysml_lazy_26 = io_8 == 3;
                }
                if (sysml_lazy_26) {
                    sysml_conditional_27 = true;
                }
                else {
                    sysml_lazy_28 = io_8_valid;
                    if (sysml_lazy_28) {
                        sysml_lazy_28 = io_8 == 4;
                    }
                    if (sysml_lazy_28) {
                        sysml_conditional_29 = true;
                    }
                    else {
                        sysml_lazy_30 = io_8_valid;
                        if (sysml_lazy_30) {
                            sysml_lazy_30 = io_8 == 5;
                        }
                        if (sysml_lazy_30) {
                            sysml_conditional_31 = false;
                        }
                        else {
                            sysml_lazy_32 = io_8_valid;
                            if (sysml_lazy_32) {
                                sysml_lazy_32 = io_8 == 6;
                            }
                            if (sysml_lazy_32) {
                                sysml_conditional_33 = false;
                            }
                            else {
                                sysml_lazy_34 = io_8_valid;
                                if (sysml_lazy_34) {
                                    sysml_lazy_34 = io_8 == 7;
                                }
                                if (sysml_lazy_34) {
                                    sysml_conditional_35 = true;
                                }
                                else {
                                    sysml_lazy_36 = io_8_valid;
                                    if (sysml_lazy_36) {
                                        sysml_lazy_36 = io_8 == 8;
                                    }
                                    if (sysml_lazy_36) {
                                        sysml_conditional_37 = true;
                                    }
                                    else {
                                        sysml_lazy_38 = io_8_valid;
                                        if (sysml_lazy_38) {
                                            sysml_lazy_38 = io_8 == 9;
                                        }
                                        if (sysml_lazy_38) {
                                            sysml_conditional_39 = true;
                                        }
                                        else {
                                            sysml_conditional_39 = false;
                                        }
                                        sysml_conditional_37 = sysml_conditional_39;
                                    }
                                    sysml_conditional_35 = sysml_conditional_37;
                                }
                                sysml_conditional_33 = sysml_conditional_35;
                            }
                            sysml_conditional_31 = sysml_conditional_33;
                        }
                        sysml_conditional_29 = sysml_conditional_31;
                    }
                    sysml_conditional_27 = sysml_conditional_29;
                }
                sysml_conditional_25 = sysml_conditional_27;
            }
            sysml_conditional_23 = sysml_conditional_25;
        }
        sysml_conditional_21 = sysml_conditional_23;
    }
    sysml_value_1 = sysml_conditional_21;
    bool sysml_value_2 = 0;
    sysml_lazy_40 = io_8_valid;
    if (sysml_lazy_40) {
        sysml_lazy_40 = io_8 == 0;
    }
    if (sysml_lazy_40) {
        sysml_conditional_41 = true;
    }
    else {
        sysml_lazy_42 = io_8_valid;
        if (sysml_lazy_42) {
            sysml_lazy_42 = io_8 == 1;
        }
        if (sysml_lazy_42) {
            sysml_conditional_43 = false;
        }
        else {
            sysml_lazy_44 = io_8_valid;
            if (sysml_lazy_44) {
                sysml_lazy_44 = io_8 == 2;
            }
            if (sysml_lazy_44) {
                sysml_conditional_45 = false;
            }
            else {
                sysml_lazy_46 = io_8_valid;
                if (sysml_lazy_46) {
                    sysml_lazy_46 = io_8 == 3;
                }
                if (sysml_lazy_46) {
                    sysml_conditional_47 = true;
                }
                else {
                    sysml_lazy_48 = io_8_valid;
                    if (sysml_lazy_48) {
                        sysml_lazy_48 = io_8 == 4;
                    }
                    if (sysml_lazy_48) {
                        sysml_conditional_49 = true;
                    }
                    else {
                        sysml_lazy_50 = io_8_valid;
                        if (sysml_lazy_50) {
                            sysml_lazy_50 = io_8 == 5;
                        }
                        if (sysml_lazy_50) {
                            sysml_conditional_51 = false;
                        }
                        else {
                            sysml_lazy_52 = io_8_valid;
                            if (sysml_lazy_52) {
                                sysml_lazy_52 = io_8 == 6;
                            }
                            if (sysml_lazy_52) {
                                sysml_conditional_53 = true;
                            }
                            else {
                                sysml_lazy_54 = io_8_valid;
                                if (sysml_lazy_54) {
                                    sysml_lazy_54 = io_8 == 7;
                                }
                                if (sysml_lazy_54) {
                                    sysml_conditional_55 = true;
                                }
                                else {
                                    sysml_lazy_56 = io_8_valid;
                                    if (sysml_lazy_56) {
                                        sysml_lazy_56 = io_8 == 8;
                                    }
                                    if (sysml_lazy_56) {
                                        sysml_conditional_57 = true;
                                    }
                                    else {
                                        sysml_lazy_58 = io_8_valid;
                                        if (sysml_lazy_58) {
                                            sysml_lazy_58 = io_8 == 9;
                                        }
                                        if (sysml_lazy_58) {
                                            sysml_conditional_59 = true;
                                        }
                                        else {
                                            sysml_conditional_59 = false;
                                        }
                                        sysml_conditional_57 = sysml_conditional_59;
                                    }
                                    sysml_conditional_55 = sysml_conditional_57;
                                }
                                sysml_conditional_53 = sysml_conditional_55;
                            }
                            sysml_conditional_51 = sysml_conditional_53;
                        }
                        sysml_conditional_49 = sysml_conditional_51;
                    }
                    sysml_conditional_47 = sysml_conditional_49;
                }
                sysml_conditional_45 = sysml_conditional_47;
            }
            sysml_conditional_43 = sysml_conditional_45;
        }
        sysml_conditional_41 = sysml_conditional_43;
    }
    sysml_value_2 = sysml_conditional_41;
    bool sysml_value_3 = 0;
    sysml_lazy_60 = io_8_valid;
    if (sysml_lazy_60) {
        sysml_lazy_60 = io_8 == 0;
    }
    if (sysml_lazy_60) {
        sysml_conditional_61 = true;
    }
    else {
        sysml_lazy_62 = io_8_valid;
        if (sysml_lazy_62) {
            sysml_lazy_62 = io_8 == 1;
        }
        if (sysml_lazy_62) {
            sysml_conditional_63 = false;
        }
        else {
            sysml_lazy_64 = io_8_valid;
            if (sysml_lazy_64) {
                sysml_lazy_64 = io_8 == 2;
            }
            if (sysml_lazy_64) {
                sysml_conditional_65 = true;
            }
            else {
                sysml_lazy_66 = io_8_valid;
                if (sysml_lazy_66) {
                    sysml_lazy_66 = io_8 == 3;
                }
                if (sysml_lazy_66) {
                    sysml_conditional_67 = true;
                }
                else {
                    sysml_lazy_68 = io_8_valid;
                    if (sysml_lazy_68) {
                        sysml_lazy_68 = io_8 == 4;
                    }
                    if (sysml_lazy_68) {
                        sysml_conditional_69 = false;
                    }
                    else {
                        sysml_lazy_70 = io_8_valid;
                        if (sysml_lazy_70) {
                            sysml_lazy_70 = io_8 == 5;
                        }
                        if (sysml_lazy_70) {
                            sysml_conditional_71 = true;
                        }
                        else {
                            sysml_lazy_72 = io_8_valid;
                            if (sysml_lazy_72) {
                                sysml_lazy_72 = io_8 == 6;
                            }
                            if (sysml_lazy_72) {
                                sysml_conditional_73 = true;
                            }
                            else {
                                sysml_lazy_74 = io_8_valid;
                                if (sysml_lazy_74) {
                                    sysml_lazy_74 = io_8 == 7;
                                }
                                if (sysml_lazy_74) {
                                    sysml_conditional_75 = true;
                                }
                                else {
                                    sysml_lazy_76 = io_8_valid;
                                    if (sysml_lazy_76) {
                                        sysml_lazy_76 = io_8 == 8;
                                    }
                                    if (sysml_lazy_76) {
                                        sysml_conditional_77 = true;
                                    }
                                    else {
                                        sysml_lazy_78 = io_8_valid;
                                        if (sysml_lazy_78) {
                                            sysml_lazy_78 = io_8 == 9;
                                        }
                                        if (sysml_lazy_78) {
                                            sysml_conditional_79 = true;
                                        }
                                        else {
                                            sysml_conditional_79 = false;
                                        }
                                        sysml_conditional_77 = sysml_conditional_79;
                                    }
                                    sysml_conditional_75 = sysml_conditional_77;
                                }
                                sysml_conditional_73 = sysml_conditional_75;
                            }
                            sysml_conditional_71 = sysml_conditional_73;
                        }
                        sysml_conditional_69 = sysml_conditional_71;
                    }
                    sysml_conditional_67 = sysml_conditional_69;
                }
                sysml_conditional_65 = sysml_conditional_67;
            }
            sysml_conditional_63 = sysml_conditional_65;
        }
        sysml_conditional_61 = sysml_conditional_63;
    }
    sysml_value_3 = sysml_conditional_61;
    bool sysml_value_4 = 0;
    sysml_lazy_80 = io_8_valid;
    if (sysml_lazy_80) {
        sysml_lazy_80 = io_8 == 0;
    }
    if (sysml_lazy_80) {
        sysml_conditional_81 = true;
    }
    else {
        sysml_lazy_82 = io_8_valid;
        if (sysml_lazy_82) {
            sysml_lazy_82 = io_8 == 1;
        }
        if (sysml_lazy_82) {
            sysml_conditional_83 = false;
        }
        else {
            sysml_lazy_84 = io_8_valid;
            if (sysml_lazy_84) {
                sysml_lazy_84 = io_8 == 2;
            }
            if (sysml_lazy_84) {
                sysml_conditional_85 = true;
            }
            else {
                sysml_lazy_86 = io_8_valid;
                if (sysml_lazy_86) {
                    sysml_lazy_86 = io_8 == 3;
                }
                if (sysml_lazy_86) {
                    sysml_conditional_87 = false;
                }
                else {
                    sysml_lazy_88 = io_8_valid;
                    if (sysml_lazy_88) {
                        sysml_lazy_88 = io_8 == 4;
                    }
                    if (sysml_lazy_88) {
                        sysml_conditional_89 = true;
                    }
                    else {
                        sysml_lazy_90 = io_8_valid;
                        if (sysml_lazy_90) {
                            sysml_lazy_90 = io_8 == 5;
                        }
                        if (sysml_lazy_90) {
                            sysml_conditional_91 = true;
                        }
                        else {
                            sysml_lazy_92 = io_8_valid;
                            if (sysml_lazy_92) {
                                sysml_lazy_92 = io_8 == 6;
                            }
                            if (sysml_lazy_92) {
                                sysml_conditional_93 = true;
                            }
                            else {
                                sysml_lazy_94 = io_8_valid;
                                if (sysml_lazy_94) {
                                    sysml_lazy_94 = io_8 == 7;
                                }
                                if (sysml_lazy_94) {
                                    sysml_conditional_95 = false;
                                }
                                else {
                                    sysml_lazy_96 = io_8_valid;
                                    if (sysml_lazy_96) {
                                        sysml_lazy_96 = io_8 == 8;
                                    }
                                    if (sysml_lazy_96) {
                                        sysml_conditional_97 = true;
                                    }
                                    else {
                                        sysml_lazy_98 = io_8_valid;
                                        if (sysml_lazy_98) {
                                            sysml_lazy_98 = io_8 == 9;
                                        }
                                        if (sysml_lazy_98) {
                                            sysml_conditional_99 = false;
                                        }
                                        else {
                                            sysml_conditional_99 = false;
                                        }
                                        sysml_conditional_97 = sysml_conditional_99;
                                    }
                                    sysml_conditional_95 = sysml_conditional_97;
                                }
                                sysml_conditional_93 = sysml_conditional_95;
                            }
                            sysml_conditional_91 = sysml_conditional_93;
                        }
                        sysml_conditional_89 = sysml_conditional_91;
                    }
                    sysml_conditional_87 = sysml_conditional_89;
                }
                sysml_conditional_85 = sysml_conditional_87;
            }
            sysml_conditional_83 = sysml_conditional_85;
        }
        sysml_conditional_81 = sysml_conditional_83;
    }
    sysml_value_4 = sysml_conditional_81;
    bool sysml_value_5 = 0;
    sysml_lazy_100 = io_8_valid;
    if (sysml_lazy_100) {
        sysml_lazy_100 = io_8 == 0;
    }
    if (sysml_lazy_100) {
        sysml_conditional_101 = true;
    }
    else {
        sysml_lazy_102 = io_8_valid;
        if (sysml_lazy_102) {
            sysml_lazy_102 = io_8 == 1;
        }
        if (sysml_lazy_102) {
            sysml_conditional_103 = true;
        }
        else {
            sysml_lazy_104 = io_8_valid;
            if (sysml_lazy_104) {
                sysml_lazy_104 = io_8 == 2;
            }
            if (sysml_lazy_104) {
                sysml_conditional_105 = false;
            }
            else {
                sysml_lazy_106 = io_8_valid;
                if (sysml_lazy_106) {
                    sysml_lazy_106 = io_8 == 3;
                }
                if (sysml_lazy_106) {
                    sysml_conditional_107 = false;
                }
                else {
                    sysml_lazy_108 = io_8_valid;
                    if (sysml_lazy_108) {
                        sysml_lazy_108 = io_8 == 4;
                    }
                    if (sysml_lazy_108) {
                        sysml_conditional_109 = false;
                    }
                    else {
                        sysml_lazy_110 = io_8_valid;
                        if (sysml_lazy_110) {
                            sysml_lazy_110 = io_8 == 5;
                        }
                        if (sysml_lazy_110) {
                            sysml_conditional_111 = false;
                        }
                        else {
                            sysml_lazy_112 = io_8_valid;
                            if (sysml_lazy_112) {
                                sysml_lazy_112 = io_8 == 6;
                            }
                            if (sysml_lazy_112) {
                                sysml_conditional_113 = false;
                            }
                            else {
                                sysml_lazy_114 = io_8_valid;
                                if (sysml_lazy_114) {
                                    sysml_lazy_114 = io_8 == 7;
                                }
                                if (sysml_lazy_114) {
                                    sysml_conditional_115 = true;
                                }
                                else {
                                    sysml_lazy_116 = io_8_valid;
                                    if (sysml_lazy_116) {
                                        sysml_lazy_116 = io_8 == 8;
                                    }
                                    if (sysml_lazy_116) {
                                        sysml_conditional_117 = true;
                                    }
                                    else {
                                        sysml_lazy_118 = io_8_valid;
                                        if (sysml_lazy_118) {
                                            sysml_lazy_118 = io_8 == 9;
                                        }
                                        if (sysml_lazy_118) {
                                            sysml_conditional_119 = true;
                                        }
                                        else {
                                            sysml_conditional_119 = false;
                                        }
                                        sysml_conditional_117 = sysml_conditional_119;
                                    }
                                    sysml_conditional_115 = sysml_conditional_117;
                                }
                                sysml_conditional_113 = sysml_conditional_115;
                            }
                            sysml_conditional_111 = sysml_conditional_113;
                        }
                        sysml_conditional_109 = sysml_conditional_111;
                    }
                    sysml_conditional_107 = sysml_conditional_109;
                }
                sysml_conditional_105 = sysml_conditional_107;
            }
            sysml_conditional_103 = sysml_conditional_105;
        }
        sysml_conditional_101 = sysml_conditional_103;
    }
    sysml_value_5 = sysml_conditional_101;
    bool sysml_value_6 = 0;
    sysml_lazy_120 = io_8_valid;
    if (sysml_lazy_120) {
        sysml_lazy_120 = io_8 == 0;
    }
    if (sysml_lazy_120) {
        sysml_conditional_121 = false;
    }
    else {
        sysml_lazy_122 = io_8_valid;
        if (sysml_lazy_122) {
            sysml_lazy_122 = io_8 == 1;
        }
        if (sysml_lazy_122) {
            sysml_conditional_123 = true;
        }
        else {
            sysml_lazy_124 = io_8_valid;
            if (sysml_lazy_124) {
                sysml_lazy_124 = io_8 == 2;
            }
            if (sysml_lazy_124) {
                sysml_conditional_125 = false;
            }
            else {
                sysml_lazy_126 = io_8_valid;
                if (sysml_lazy_126) {
                    sysml_lazy_126 = io_8 == 3;
                }
                if (sysml_lazy_126) {
                    sysml_conditional_127 = false;
                }
                else {
                    sysml_lazy_128 = io_8_valid;
                    if (sysml_lazy_128) {
                        sysml_lazy_128 = io_8 == 4;
                    }
                    if (sysml_lazy_128) {
                        sysml_conditional_129 = false;
                    }
                    else {
                        sysml_lazy_130 = io_8_valid;
                        if (sysml_lazy_130) {
                            sysml_lazy_130 = io_8 == 5;
                        }
                        if (sysml_lazy_130) {
                            sysml_conditional_131 = false;
                        }
                        else {
                            sysml_lazy_132 = io_8_valid;
                            if (sysml_lazy_132) {
                                sysml_lazy_132 = io_8 == 6;
                            }
                            if (sysml_lazy_132) {
                                sysml_conditional_133 = true;
                            }
                            else {
                                sysml_lazy_134 = io_8_valid;
                                if (sysml_lazy_134) {
                                    sysml_lazy_134 = io_8 == 7;
                                }
                                if (sysml_lazy_134) {
                                    sysml_conditional_135 = false;
                                }
                                else {
                                    sysml_lazy_136 = io_8_valid;
                                    if (sysml_lazy_136) {
                                        sysml_lazy_136 = io_8 == 8;
                                    }
                                    if (sysml_lazy_136) {
                                        sysml_conditional_137 = false;
                                    }
                                    else {
                                        sysml_lazy_138 = io_8_valid;
                                        if (sysml_lazy_138) {
                                            sysml_lazy_138 = io_8 == 9;
                                        }
                                        if (sysml_lazy_138) {
                                            sysml_conditional_139 = false;
                                        }
                                        else {
                                            sysml_conditional_139 = false;
                                        }
                                        sysml_conditional_137 = sysml_conditional_139;
                                    }
                                    sysml_conditional_135 = sysml_conditional_137;
                                }
                                sysml_conditional_133 = sysml_conditional_135;
                            }
                            sysml_conditional_131 = sysml_conditional_133;
                        }
                        sysml_conditional_129 = sysml_conditional_131;
                    }
                    sysml_conditional_127 = sysml_conditional_129;
                }
                sysml_conditional_125 = sysml_conditional_127;
            }
            sysml_conditional_123 = sysml_conditional_125;
        }
        sysml_conditional_121 = sysml_conditional_123;
    }
    sysml_value_6 = sysml_conditional_121;
    bool sysml_value_7 = 0;
    if (false) {
        sysml_conditional_140 = true;
    }
    else {
        sysml_conditional_140 = false;
    }
    sysml_value_7 = sysml_conditional_140;
    bool sysml_value_8 = 0;
    sysml_value_8 = io_8_valid;
    int sysml_value_9 = 0;
    sysml_value_9 = io_8;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *serial_0_trigger = sysml_value_8;
    *serial_0_value_0 = sysml_value_9;
}