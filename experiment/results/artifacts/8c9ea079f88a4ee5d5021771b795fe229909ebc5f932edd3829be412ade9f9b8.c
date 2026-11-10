#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_digit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_8, bool io_8_valid, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    bool sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    bool sysml_conditional_19 = 0;
    bool sysml_conditional_20 = 0;
    bool sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    bool sysml_conditional_23 = 0;
    bool sysml_conditional_24 = 0;
    bool sysml_conditional_25 = 0;
    bool sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    bool sysml_conditional_28 = 0;
    bool sysml_conditional_29 = 0;
    bool sysml_conditional_30 = 0;
    bool sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    bool sysml_conditional_33 = 0;
    bool sysml_conditional_34 = 0;
    bool sysml_conditional_35 = 0;
    bool sysml_conditional_36 = 0;
    bool sysml_conditional_37 = 0;
    bool sysml_conditional_38 = 0;
    bool sysml_conditional_39 = 0;
    bool sysml_conditional_40 = 0;
    bool sysml_conditional_41 = 0;
    bool sysml_conditional_42 = 0;
    bool sysml_conditional_43 = 0;
    bool sysml_conditional_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_conditional_46 = 0;
    bool sysml_conditional_47 = 0;
    bool sysml_conditional_48 = 0;
    bool sysml_conditional_49 = 0;
    bool sysml_conditional_50 = 0;
    bool sysml_conditional_51 = 0;
    bool sysml_conditional_52 = 0;
    bool sysml_conditional_53 = 0;
    bool sysml_conditional_54 = 0;
    bool sysml_conditional_55 = 0;
    bool sysml_conditional_56 = 0;
    bool sysml_conditional_57 = 0;
    bool sysml_conditional_58 = 0;
    bool sysml_conditional_59 = 0;
    bool sysml_conditional_60 = 0;
    bool sysml_conditional_61 = 0;
    bool sysml_conditional_62 = 0;
    bool sysml_conditional_63 = 0;
    bool sysml_conditional_64 = 0;
    bool sysml_conditional_65 = 0;
    bool sysml_conditional_66 = 0;
    bool sysml_conditional_67 = 0;
    bool sysml_conditional_68 = 0;
    bool sysml_conditional_69 = 0;
    if (io_8_valid) {
        sysml_state_digit = io_8;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_0 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_1 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_2 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_3 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_4 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_5 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_6 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_7 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_8 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_9 = true;
                                        }
                                        else {
                                            sysml_conditional_9 = false;
                                        }
                                        sysml_conditional_8 = sysml_conditional_9;
                                    }
                                    sysml_conditional_7 = sysml_conditional_8;
                                }
                                sysml_conditional_6 = sysml_conditional_7;
                            }
                            sysml_conditional_5 = sysml_conditional_6;
                        }
                        sysml_conditional_4 = sysml_conditional_5;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_10 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_11 = true;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_12 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_13 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_14 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_15 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_16 = false;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_17 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_18 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_19 = true;
                                        }
                                        else {
                                            sysml_conditional_19 = false;
                                        }
                                        sysml_conditional_18 = sysml_conditional_19;
                                    }
                                    sysml_conditional_17 = sysml_conditional_18;
                                }
                                sysml_conditional_16 = sysml_conditional_17;
                            }
                            sysml_conditional_15 = sysml_conditional_16;
                        }
                        sysml_conditional_14 = sysml_conditional_15;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_value_1 = sysml_conditional_10;
    bool sysml_value_2 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_20 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_21 = true;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_22 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_23 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_24 = false;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_25 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_26 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_27 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_28 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_29 = true;
                                        }
                                        else {
                                            sysml_conditional_29 = false;
                                        }
                                        sysml_conditional_28 = sysml_conditional_29;
                                    }
                                    sysml_conditional_27 = sysml_conditional_28;
                                }
                                sysml_conditional_26 = sysml_conditional_27;
                            }
                            sysml_conditional_25 = sysml_conditional_26;
                        }
                        sysml_conditional_24 = sysml_conditional_25;
                    }
                    sysml_conditional_23 = sysml_conditional_24;
                }
                sysml_conditional_22 = sysml_conditional_23;
            }
            sysml_conditional_21 = sysml_conditional_22;
        }
        sysml_conditional_20 = sysml_conditional_21;
    }
    sysml_value_2 = sysml_conditional_20;
    bool sysml_value_3 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_30 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_31 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_32 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_33 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_34 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_35 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_36 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_37 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_38 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_39 = true;
                                        }
                                        else {
                                            sysml_conditional_39 = false;
                                        }
                                        sysml_conditional_38 = sysml_conditional_39;
                                    }
                                    sysml_conditional_37 = sysml_conditional_38;
                                }
                                sysml_conditional_36 = sysml_conditional_37;
                            }
                            sysml_conditional_35 = sysml_conditional_36;
                        }
                        sysml_conditional_34 = sysml_conditional_35;
                    }
                    sysml_conditional_33 = sysml_conditional_34;
                }
                sysml_conditional_32 = sysml_conditional_33;
            }
            sysml_conditional_31 = sysml_conditional_32;
        }
        sysml_conditional_30 = sysml_conditional_31;
    }
    sysml_value_3 = sysml_conditional_30;
    bool sysml_value_4 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_40 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_41 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_42 = false;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_43 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_44 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_45 = false;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_46 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_47 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_48 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_49 = false;
                                        }
                                        else {
                                            sysml_conditional_49 = false;
                                        }
                                        sysml_conditional_48 = sysml_conditional_49;
                                    }
                                    sysml_conditional_47 = sysml_conditional_48;
                                }
                                sysml_conditional_46 = sysml_conditional_47;
                            }
                            sysml_conditional_45 = sysml_conditional_46;
                        }
                        sysml_conditional_44 = sysml_conditional_45;
                    }
                    sysml_conditional_43 = sysml_conditional_44;
                }
                sysml_conditional_42 = sysml_conditional_43;
            }
            sysml_conditional_41 = sysml_conditional_42;
        }
        sysml_conditional_40 = sysml_conditional_41;
    }
    sysml_value_4 = sysml_conditional_40;
    bool sysml_value_5 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_50 = true;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_51 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_52 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_53 = false;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_54 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_55 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_56 = false;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_57 = false;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_58 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_59 = true;
                                        }
                                        else {
                                            sysml_conditional_59 = false;
                                        }
                                        sysml_conditional_58 = sysml_conditional_59;
                                    }
                                    sysml_conditional_57 = sysml_conditional_58;
                                }
                                sysml_conditional_56 = sysml_conditional_57;
                            }
                            sysml_conditional_55 = sysml_conditional_56;
                        }
                        sysml_conditional_54 = sysml_conditional_55;
                    }
                    sysml_conditional_53 = sysml_conditional_54;
                }
                sysml_conditional_52 = sysml_conditional_53;
            }
            sysml_conditional_51 = sysml_conditional_52;
        }
        sysml_conditional_50 = sysml_conditional_51;
    }
    sysml_value_5 = sysml_conditional_50;
    bool sysml_value_6 = 0;
    if (sysml_state_digit == 0) {
        sysml_conditional_60 = false;
    }
    else {
        if (sysml_state_digit == 1) {
            sysml_conditional_61 = false;
        }
        else {
            if (sysml_state_digit == 2) {
                sysml_conditional_62 = true;
            }
            else {
                if (sysml_state_digit == 3) {
                    sysml_conditional_63 = true;
                }
                else {
                    if (sysml_state_digit == 4) {
                        sysml_conditional_64 = true;
                    }
                    else {
                        if (sysml_state_digit == 5) {
                            sysml_conditional_65 = true;
                        }
                        else {
                            if (sysml_state_digit == 6) {
                                sysml_conditional_66 = true;
                            }
                            else {
                                if (sysml_state_digit == 7) {
                                    sysml_conditional_67 = true;
                                }
                                else {
                                    if (sysml_state_digit == 8) {
                                        sysml_conditional_68 = true;
                                    }
                                    else {
                                        if (sysml_state_digit == 9) {
                                            sysml_conditional_69 = true;
                                        }
                                        else {
                                            sysml_conditional_69 = false;
                                        }
                                        sysml_conditional_68 = sysml_conditional_69;
                                    }
                                    sysml_conditional_67 = sysml_conditional_68;
                                }
                                sysml_conditional_66 = sysml_conditional_67;
                            }
                            sysml_conditional_65 = sysml_conditional_66;
                        }
                        sysml_conditional_64 = sysml_conditional_65;
                    }
                    sysml_conditional_63 = sysml_conditional_64;
                }
                sysml_conditional_62 = sysml_conditional_63;
            }
            sysml_conditional_61 = sysml_conditional_62;
        }
        sysml_conditional_60 = sysml_conditional_61;
    }
    sysml_value_6 = sysml_conditional_60;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
}