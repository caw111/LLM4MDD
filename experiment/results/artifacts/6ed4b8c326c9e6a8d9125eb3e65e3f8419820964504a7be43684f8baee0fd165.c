#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousAllFull = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousAllFull = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, bool* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2, bool* serial_0_value_3, int* serial_0_value_4, bool* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    int sysml_conditional_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    int sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    int sysml_conditional_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    int sysml_conditional_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    int sysml_conditional_31 = 0;
    bool sysml_lazy_32 = 0;
    bool sysml_lazy_33 = 0;
    bool sysml_lazy_34 = 0;
    bool sysml_lazy_35 = 0;
    bool sysml_lazy_36 = 0;
    int sysml_conditional_37 = 0;
    bool sysml_lazy_38 = 0;
    bool sysml_lazy_39 = 0;
    bool sysml_lazy_40 = 0;
    bool sysml_lazy_41 = 0;
    int sysml_conditional_42 = 0;
    bool sysml_lazy_43 = 0;
    bool sysml_lazy_44 = 0;
    int sysml_conditional_45 = 0;
    bool sysml_lazy_46 = 0;
    bool sysml_lazy_47 = 0;
    bool sysml_lazy_48 = 0;
    bool sysml_lazy_49 = 0;
    bool sysml_lazy_50 = 0;
    bool sysml_lazy_51 = 0;
    bool sysml_lazy_52 = 0;
    bool sysml_lazy_53 = 0;
    int sysml_conditional_54 = 0;
    bool sysml_lazy_55 = 0;
    bool sysml_lazy_56 = 0;
    bool sysml_lazy_57 = 0;
    bool sysml_lazy_58 = 0;
    bool sysml_lazy_59 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_state_previousAllFull = sysml_lazy_1;
    int sysml_value_0 = 0;
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_2;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousAllFull;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    sysml_lazy_6 = io_0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_2;
    }
    sysml_value_1 = sysml_lazy_7;
    int sysml_value_2 = 0;
    sysml_lazy_8 = io_0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !io_1;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !io_2;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_lazy_11 = io_0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = io_1;
        }
        sysml_lazy_12 = sysml_lazy_11;
        if (sysml_lazy_12) {
            sysml_lazy_12 = !io_2;
        }
        if (sysml_lazy_12) {
            sysml_conditional_13 = 2;
        }
        else {
            sysml_lazy_14 = io_0;
            if (sysml_lazy_14) {
                sysml_lazy_14 = !io_1;
            }
            sysml_lazy_15 = sysml_lazy_14;
            if (sysml_lazy_15) {
                sysml_lazy_15 = io_2;
            }
            if (sysml_lazy_15) {
                sysml_conditional_16 = 2;
            }
            else {
                sysml_lazy_17 = !io_0;
                if (sysml_lazy_17) {
                    sysml_lazy_17 = io_1;
                }
                sysml_lazy_18 = sysml_lazy_17;
                if (sysml_lazy_18) {
                    sysml_lazy_18 = io_2;
                }
                if (sysml_lazy_18) {
                    sysml_conditional_19 = 2;
                }
                else {
                    sysml_lazy_20 = io_0;
                    if (sysml_lazy_20) {
                        sysml_lazy_20 = io_1;
                    }
                    sysml_lazy_21 = sysml_lazy_20;
                    if (sysml_lazy_21) {
                        sysml_lazy_21 = io_2;
                    }
                    if (sysml_lazy_21) {
                        sysml_conditional_22 = 3;
                    }
                    else {
                        sysml_lazy_23 = io_0;
                        if (sysml_lazy_23) {
                            sysml_lazy_23 = !io_1;
                        }
                        sysml_lazy_24 = sysml_lazy_23;
                        if (sysml_lazy_24) {
                            sysml_lazy_24 = !io_2;
                        }
                        if (sysml_lazy_24) {
                            sysml_conditional_25 = 1;
                        }
                        else {
                            sysml_lazy_26 = !io_0;
                            if (sysml_lazy_26) {
                                sysml_lazy_26 = io_1;
                            }
                            sysml_lazy_27 = sysml_lazy_26;
                            if (sysml_lazy_27) {
                                sysml_lazy_27 = !io_2;
                            }
                            if (sysml_lazy_27) {
                                sysml_conditional_28 = 1;
                            }
                            else {
                                sysml_lazy_29 = !io_0;
                                if (sysml_lazy_29) {
                                    sysml_lazy_29 = !io_1;
                                }
                                sysml_lazy_30 = sysml_lazy_29;
                                if (sysml_lazy_30) {
                                    sysml_lazy_30 = io_2;
                                }
                                if (sysml_lazy_30) {
                                    sysml_conditional_31 = 1;
                                }
                                else {
                                    sysml_conditional_31 = 0;
                                }
                                sysml_conditional_28 = sysml_conditional_31;
                            }
                            sysml_conditional_25 = sysml_conditional_28;
                        }
                        sysml_conditional_22 = sysml_conditional_25;
                    }
                    sysml_conditional_19 = sysml_conditional_22;
                }
                sysml_conditional_16 = sysml_conditional_19;
            }
            sysml_conditional_13 = sysml_conditional_16;
        }
        sysml_conditional_10 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_10;
    bool sysml_value_3 = 0;
    sysml_lazy_32 = io_0;
    if (!sysml_lazy_32) {
        sysml_lazy_32 = io_1;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_33 = io_2;
    }
    sysml_lazy_34 = sysml_lazy_33;
    if (sysml_lazy_34) {
        sysml_lazy_34 = !sysml_state_previousAllFull;
    }
    sysml_value_3 = sysml_lazy_34;
    int sysml_value_4 = 0;
    sysml_lazy_35 = io_0;
    if (sysml_lazy_35) {
        sysml_lazy_35 = io_1;
    }
    sysml_lazy_36 = sysml_lazy_35;
    if (sysml_lazy_36) {
        sysml_lazy_36 = io_2;
    }
    if (sysml_lazy_36) {
        sysml_conditional_37 = 3;
    }
    else {
        sysml_lazy_38 = io_0;
        if (sysml_lazy_38) {
            sysml_lazy_39 = io_1;
            if (!sysml_lazy_39) {
                sysml_lazy_39 = io_2;
            }
            sysml_lazy_38 = sysml_lazy_39;
        }
        sysml_lazy_40 = sysml_lazy_38;
        if (sysml_lazy_40) {
            sysml_lazy_41 = io_1;
            if (sysml_lazy_41) {
                sysml_lazy_41 = io_2;
            }
            sysml_lazy_40 = !sysml_lazy_41;
        }
        if (sysml_lazy_40) {
            sysml_conditional_42 = 2;
        }
        else {
            sysml_lazy_43 = !io_0;
            if (sysml_lazy_43) {
                sysml_lazy_43 = io_1;
            }
            sysml_lazy_44 = sysml_lazy_43;
            if (sysml_lazy_44) {
                sysml_lazy_44 = io_2;
            }
            if (sysml_lazy_44) {
                sysml_conditional_45 = 2;
            }
            else {
                sysml_lazy_46 = io_0;
                if (sysml_lazy_46) {
                    sysml_lazy_46 = !io_1;
                }
                sysml_lazy_47 = sysml_lazy_46;
                if (sysml_lazy_47) {
                    sysml_lazy_47 = !io_2;
                }
                sysml_lazy_48 = sysml_lazy_47;
                if (!sysml_lazy_48) {
                    sysml_lazy_49 = !io_0;
                    if (sysml_lazy_49) {
                        sysml_lazy_49 = io_1;
                    }
                    sysml_lazy_50 = sysml_lazy_49;
                    if (sysml_lazy_50) {
                        sysml_lazy_50 = !io_2;
                    }
                    sysml_lazy_48 = sysml_lazy_50;
                }
                sysml_lazy_51 = sysml_lazy_48;
                if (!sysml_lazy_51) {
                    sysml_lazy_52 = !io_0;
                    if (sysml_lazy_52) {
                        sysml_lazy_52 = !io_1;
                    }
                    sysml_lazy_53 = sysml_lazy_52;
                    if (sysml_lazy_53) {
                        sysml_lazy_53 = io_2;
                    }
                    sysml_lazy_51 = sysml_lazy_53;
                }
                if (sysml_lazy_51) {
                    sysml_conditional_54 = 1;
                }
                else {
                    sysml_conditional_54 = 0;
                }
                sysml_conditional_45 = sysml_conditional_54;
            }
            sysml_conditional_42 = sysml_conditional_45;
        }
        sysml_conditional_37 = sysml_conditional_42;
    }
    sysml_value_4 = sysml_conditional_37;
    bool sysml_value_5 = 0;
    sysml_value_5 = io_0;
    bool sysml_value_6 = 0;
    sysml_value_6 = io_1;
    bool sysml_value_7 = 0;
    sysml_value_7 = io_2;
    int sysml_value_8 = 0;
    sysml_value_8 = 0;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    bool sysml_value_10 = 0;
    sysml_lazy_55 = io_0;
    if (sysml_lazy_55) {
        sysml_lazy_55 = io_1;
    }
    sysml_lazy_56 = sysml_lazy_55;
    if (sysml_lazy_56) {
        sysml_lazy_56 = io_2;
    }
    sysml_value_10 = sysml_lazy_56;
    bool sysml_value_11 = 0;
    sysml_lazy_57 = io_0;
    if (sysml_lazy_57) {
        sysml_lazy_57 = io_1;
    }
    sysml_lazy_58 = sysml_lazy_57;
    if (sysml_lazy_58) {
        sysml_lazy_58 = io_2;
    }
    sysml_lazy_59 = sysml_lazy_58;
    if (sysml_lazy_59) {
        sysml_lazy_59 = !sysml_state_previousAllFull;
    }
    sysml_value_11 = sysml_lazy_59;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
    *serial_0_value_5 = sysml_value_9;
    *serial_0_value_6 = sysml_value_10;
    *serial_0_value_7 = sysml_value_11;
}