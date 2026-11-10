#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_pressHoldMs = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debouncedPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 0;
    sysml_state_pressHoldMs = 0;
    sysml_state_previousPressed = false;
    sysml_state_debouncedPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, bool* io_7_segment_0, bool* io_7_segment_1, bool* io_7_segment_2, bool* io_7_segment_3, bool* io_7_segment_4, bool* io_7_segment_5, bool* io_7_segment_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    double sysml_conditional_16 = 0;
    double sysml_conditional_17 = 0;
    double sysml_conditional_18 = 0;
    double sysml_conditional_19 = 0;
    double sysml_conditional_20 = 0;
    double sysml_conditional_21 = 0;
    double sysml_conditional_22 = 0;
    double sysml_conditional_23 = 0;
    double sysml_conditional_24 = 0;
    double sysml_conditional_25 = 0;
    double sysml_conditional_26 = 0;
    double sysml_conditional_27 = 0;
    double sysml_conditional_28 = 0;
    double sysml_conditional_29 = 0;
    double sysml_conditional_30 = 0;
    double sysml_conditional_31 = 0;
    double sysml_conditional_32 = 0;
    double sysml_conditional_33 = 0;
    double sysml_conditional_34 = 0;
    double sysml_conditional_35 = 0;
    double sysml_conditional_36 = 0;
    double sysml_conditional_37 = 0;
    double sysml_conditional_38 = 0;
    double sysml_conditional_39 = 0;
    double sysml_conditional_40 = 0;
    double sysml_conditional_41 = 0;
    double sysml_conditional_42 = 0;
    double sysml_conditional_43 = 0;
    bool sysml_conditional_44 = 0;
    bool sysml_conditional_45 = 0;
    bool sysml_conditional_46 = 0;
    bool sysml_conditional_47 = 0;
    bool sysml_conditional_48 = 0;
    bool sysml_conditional_49 = 0;
    bool sysml_conditional_50 = 0;
    if (io_0) {
        sysml_state_pressHoldMs = (sysml_state_pressHoldMs + 10);
        sysml_state_debouncedPressed = (sysml_state_pressHoldMs >= 150);
    }
    else {
        sysml_state_pressHoldMs = 0;
        sysml_state_debouncedPressed = false;
    }
    sysml_lazy_0 = sysml_state_debouncedPressed;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode >= 7) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_mode + 1;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_previousPressed = sysml_state_debouncedPressed;
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_2 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_3 = 255.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_4 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_5 = 255.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_6 = 0.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_7 = 255.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_8 = 255.0;
                            }
                            else {
                                sysml_conditional_8 = 0.0;
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
    sysml_value_0 = sysml_conditional_2;
    double sysml_value_1 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_9 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_10 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_11 = 255.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_12 = 0.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_13 = 255.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_14 = 0.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_15 = 255.0;
                            }
                            else {
                                sysml_conditional_15 = 255.0;
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
        sysml_conditional_9 = sysml_conditional_10;
    }
    sysml_value_1 = sysml_conditional_9;
    double sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_16 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_17 = 255.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_18 = 0.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_19 = 255.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_20 = 255.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_21 = 255.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_22 = 255.0;
                            }
                            else {
                                sysml_conditional_22 = 255.0;
                            }
                            sysml_conditional_21 = sysml_conditional_22;
                        }
                        sysml_conditional_20 = sysml_conditional_21;
                    }
                    sysml_conditional_19 = sysml_conditional_20;
                }
                sysml_conditional_18 = sysml_conditional_19;
            }
            sysml_conditional_17 = sysml_conditional_18;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_16;
    double sysml_value_3 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_23 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_24 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_25 = 255.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_26 = 0.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_27 = 255.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_28 = 255.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_29 = 0.0;
                            }
                            else {
                                sysml_conditional_29 = 0.0;
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
    sysml_value_3 = sysml_conditional_23;
    double sysml_value_4 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_30 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_31 = 255.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_32 = 255.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_33 = 0.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_34 = 255.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_35 = 255.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_36 = 255.0;
                            }
                            else {
                                sysml_conditional_36 = 0.0;
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
    sysml_value_4 = sysml_conditional_30;
    double sysml_value_5 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_37 = 0.0;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_38 = 0.0;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_39 = 255.0;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_40 = 255.0;
                }
                else {
                    if (sysml_state_mode == 4) {
                        sysml_conditional_41 = 0.0;
                    }
                    else {
                        if (sysml_state_mode == 5) {
                            sysml_conditional_42 = 0.0;
                        }
                        else {
                            if (sysml_state_mode == 6) {
                                sysml_conditional_43 = 0.0;
                            }
                            else {
                                sysml_conditional_43 = 255.0;
                            }
                            sysml_conditional_42 = sysml_conditional_43;
                        }
                        sysml_conditional_41 = sysml_conditional_42;
                    }
                    sysml_conditional_40 = sysml_conditional_41;
                }
                sysml_conditional_39 = sysml_conditional_40;
            }
            sysml_conditional_38 = sysml_conditional_39;
        }
        sysml_conditional_37 = sysml_conditional_38;
    }
    sysml_value_5 = sysml_conditional_37;
    bool sysml_value_6 = 0;
    if (sysml_state_mode >= 1) {
        sysml_conditional_44 = true;
    }
    else {
        sysml_conditional_44 = false;
    }
    sysml_value_6 = sysml_conditional_44;
    bool sysml_value_7 = 0;
    if (sysml_state_mode >= 2) {
        sysml_conditional_45 = true;
    }
    else {
        sysml_conditional_45 = false;
    }
    sysml_value_7 = sysml_conditional_45;
    bool sysml_value_8 = 0;
    if (sysml_state_mode >= 3) {
        sysml_conditional_46 = true;
    }
    else {
        sysml_conditional_46 = false;
    }
    sysml_value_8 = sysml_conditional_46;
    bool sysml_value_9 = 0;
    if (sysml_state_mode >= 4) {
        sysml_conditional_47 = true;
    }
    else {
        sysml_conditional_47 = false;
    }
    sysml_value_9 = sysml_conditional_47;
    bool sysml_value_10 = 0;
    if (sysml_state_mode >= 5) {
        sysml_conditional_48 = true;
    }
    else {
        sysml_conditional_48 = false;
    }
    sysml_value_10 = sysml_conditional_48;
    bool sysml_value_11 = 0;
    if (sysml_state_mode >= 6) {
        sysml_conditional_49 = true;
    }
    else {
        sysml_conditional_49 = false;
    }
    sysml_value_11 = sysml_conditional_49;
    bool sysml_value_12 = 0;
    if (sysml_state_mode >= 7) {
        sysml_conditional_50 = true;
    }
    else {
        sysml_conditional_50 = false;
    }
    sysml_value_12 = sysml_conditional_50;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7_segment_0 = sysml_value_6;
    *io_7_segment_1 = sysml_value_7;
    *io_7_segment_2 = sysml_value_8;
    *io_7_segment_3 = sysml_value_9;
    *io_7_segment_4 = sysml_value_10;
    *io_7_segment_5 = sysml_value_11;
    *io_7_segment_6 = sysml_value_12;
}