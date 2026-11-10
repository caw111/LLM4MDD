#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phase = 0;
/* Global Variable define of  */
int sysml_state_remainingMs = 3000;
/* Global Variable define of  */
int sysml_state_roundNumber = 0;
/* Global Variable define of  */
int sysml_state_score = 0;
/* Global Variable define of  */
int sysml_state_selectedColor = 0;
/* Global Variable define of  */
int sysml_state_randomState = 173;
/* Global Variable define of  */
bool sysml_state_seenRed = false;
/* Global Variable define of  */
bool sysml_state_seenYellow = false;
/* Global Variable define of  */
bool sysml_state_seenGreen = false;
/* Global Variable define of  */
bool sysml_state_previousStop = false;
/* Global Variable define of  */
bool sysml_state_previousBack = false;
/* Global Variable define of  */
bool sysml_state_previousForward = false;
/* Global Variable define of  */
bool sysml_state_started = false;
/* Global Variable define of  */
bool sysml_state_readyReport = false;
/* Global Variable define of  */
bool sysml_state_wrongReport = false;
/* Global Variable define of  */
bool sysml_state_timeoutReport = false;
/* Global Variable define of  */
bool sysml_state_successReport = false;
/* Global Variable define of  */
bool sysml_state_endReport = false;
/* Global Variable define of  */
bool sysml_state_beginRound = false;
/* Global Variable define of  */
bool sysml_state_finishRound = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phase = 0;
    sysml_state_remainingMs = 3000;
    sysml_state_roundNumber = 0;
    sysml_state_score = 0;
    sysml_state_selectedColor = 0;
    sysml_state_randomState = 173;
    sysml_state_seenRed = false;
    sysml_state_seenYellow = false;
    sysml_state_seenGreen = false;
    sysml_state_previousStop = false;
    sysml_state_previousBack = false;
    sysml_state_previousForward = false;
    sysml_state_started = false;
    sysml_state_readyReport = false;
    sysml_state_wrongReport = false;
    sysml_state_timeoutReport = false;
    sysml_state_successReport = false;
    sysml_state_endReport = false;
    sysml_state_beginRound = false;
    sysml_state_finishRound = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int io_7, bool* io_0, bool* io_1, bool* io_2, int* io_6, bool* io_8, bool* io_9, bool* io_10, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, int* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_arithmetic_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_arithmetic_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    int sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    bool sysml_lazy_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    int sysml_conditional_28 = 0;
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
    sysml_state_readyReport = (!sysml_state_started);
    sysml_state_started = true;
    sysml_state_wrongReport = false;
    sysml_state_timeoutReport = false;
    sysml_state_successReport = false;
    sysml_state_endReport = false;
    sysml_state_beginRound = false;
    sysml_state_finishRound = false;
    if ((sysml_state_phase == 0)) {
        if ((io_7 >= sysml_state_remainingMs)) {
            sysml_state_remainingMs = 0;
            sysml_state_beginRound = true;
        }
        else {
            sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
        }
    }
    else {
        if ((sysml_state_phase == 1)) {
            if ((io_7 >= sysml_state_remainingMs)) {
                sysml_state_timeoutReport = true;
                sysml_state_finishRound = true;
            }
            else {
                sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
                sysml_lazy_0 = sysml_state_selectedColor == 0;
                if (sysml_lazy_0) {
                    sysml_lazy_1 = io_3;
                    if (sysml_lazy_1) {
                        sysml_lazy_1 = !sysml_state_previousStop;
                    }
                    sysml_lazy_0 = sysml_lazy_1;
                }
                sysml_lazy_2 = sysml_lazy_0;
                if (!sysml_lazy_2) {
                    sysml_lazy_3 = sysml_state_selectedColor == 1;
                    if (sysml_lazy_3) {
                        sysml_lazy_4 = io_4;
                        if (sysml_lazy_4) {
                            sysml_lazy_4 = !sysml_state_previousBack;
                        }
                        sysml_lazy_3 = sysml_lazy_4;
                    }
                    sysml_lazy_2 = sysml_lazy_3;
                }
                sysml_lazy_5 = sysml_lazy_2;
                if (!sysml_lazy_5) {
                    sysml_lazy_6 = sysml_state_selectedColor == 2;
                    if (sysml_lazy_6) {
                        sysml_lazy_7 = io_5;
                        if (sysml_lazy_7) {
                            sysml_lazy_7 = !sysml_state_previousForward;
                        }
                        sysml_lazy_6 = sysml_lazy_7;
                    }
                    sysml_lazy_5 = sysml_lazy_6;
                }
                if (sysml_lazy_5) {
                    sysml_state_score = (sysml_state_score + 1);
                    sysml_state_successReport = true;
                    sysml_state_finishRound = true;
                }
                else {
                    sysml_lazy_8 = io_3;
                    if (sysml_lazy_8) {
                        sysml_lazy_8 = !sysml_state_previousStop;
                    }
                    sysml_lazy_9 = sysml_lazy_8;
                    if (!sysml_lazy_9) {
                        sysml_lazy_10 = io_4;
                        if (sysml_lazy_10) {
                            sysml_lazy_10 = !sysml_state_previousBack;
                        }
                        sysml_lazy_9 = sysml_lazy_10;
                    }
                    sysml_lazy_11 = sysml_lazy_9;
                    if (!sysml_lazy_11) {
                        sysml_lazy_12 = io_5;
                        if (sysml_lazy_12) {
                            sysml_lazy_12 = !sysml_state_previousForward;
                        }
                        sysml_lazy_11 = sysml_lazy_12;
                    }
                    if (sysml_lazy_11) {
                        sysml_state_wrongReport = true;
                    }
                }
            }
        }
        else {
            if ((sysml_state_phase == 2)) {
                if ((io_7 >= sysml_state_remainingMs)) {
                    sysml_state_remainingMs = 0;
                    sysml_state_beginRound = true;
                }
                else {
                    sysml_state_remainingMs = (sysml_state_remainingMs - io_7);
                }
            }
        }
    }
    if (sysml_state_finishRound) {
        if ((sysml_state_roundNumber == 5)) {
            sysml_state_phase = 3;
            sysml_state_remainingMs = 0;
            sysml_state_endReport = true;
        }
        else {
            sysml_state_phase = 2;
            sysml_state_remainingMs = 2000;
        }
    }
    if (sysml_state_beginRound) {
        sysml_state_randomState = ((sysml_state_randomState * 25173) + 13849);
        if ((((double)sysml_state_randomState) / 65536) < ((double)((int)(((double)sysml_state_randomState) / 65536)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_arithmetic_14 = ((int)(((double)sysml_state_randomState) / 65536)) - sysml_conditional_13;
        sysml_state_randomState = sysml_state_randomState - (65536 * sysml_arithmetic_14);
        if ((((double)sysml_state_randomState) / 3) < ((double)((int)(((double)sysml_state_randomState) / 3)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_arithmetic_16 = ((int)(((double)sysml_state_randomState) / 3)) - sysml_conditional_15;
        sysml_state_selectedColor = sysml_state_randomState - (3 * sysml_arithmetic_16);
        if ((sysml_state_roundNumber < 3)) {
            sysml_lazy_17 = sysml_state_selectedColor == 0;
            if (sysml_lazy_17) {
                sysml_lazy_17 = sysml_state_seenRed;
            }
            sysml_lazy_18 = sysml_lazy_17;
            if (!sysml_lazy_18) {
                sysml_lazy_19 = sysml_state_selectedColor == 1;
                if (sysml_lazy_19) {
                    sysml_lazy_19 = sysml_state_seenYellow;
                }
                sysml_lazy_18 = sysml_lazy_19;
            }
            sysml_lazy_20 = sysml_lazy_18;
            if (!sysml_lazy_20) {
                sysml_lazy_21 = sysml_state_selectedColor == 2;
                if (sysml_lazy_21) {
                    sysml_lazy_21 = sysml_state_seenGreen;
                }
                sysml_lazy_20 = sysml_lazy_21;
            }
            if (sysml_lazy_20) {
                if (sysml_state_selectedColor == 2) {
                    sysml_conditional_22 = 0;
                }
                else {
                    sysml_conditional_22 = sysml_state_selectedColor + 1;
                }
                sysml_state_selectedColor = sysml_conditional_22;
            }
            sysml_lazy_23 = sysml_state_selectedColor == 0;
            if (sysml_lazy_23) {
                sysml_lazy_23 = sysml_state_seenRed;
            }
            sysml_lazy_24 = sysml_lazy_23;
            if (!sysml_lazy_24) {
                sysml_lazy_25 = sysml_state_selectedColor == 1;
                if (sysml_lazy_25) {
                    sysml_lazy_25 = sysml_state_seenYellow;
                }
                sysml_lazy_24 = sysml_lazy_25;
            }
            sysml_lazy_26 = sysml_lazy_24;
            if (!sysml_lazy_26) {
                sysml_lazy_27 = sysml_state_selectedColor == 2;
                if (sysml_lazy_27) {
                    sysml_lazy_27 = sysml_state_seenGreen;
                }
                sysml_lazy_26 = sysml_lazy_27;
            }
            if (sysml_lazy_26) {
                if (sysml_state_selectedColor == 2) {
                    sysml_conditional_28 = 0;
                }
                else {
                    sysml_conditional_28 = sysml_state_selectedColor + 1;
                }
                sysml_state_selectedColor = sysml_conditional_28;
            }
        }
        if ((sysml_state_selectedColor == 0)) {
            sysml_state_seenRed = true;
        }
        else {
            if ((sysml_state_selectedColor == 1)) {
                sysml_state_seenYellow = true;
            }
            else {
                sysml_state_seenGreen = true;
            }
        }
        sysml_state_roundNumber = (sysml_state_roundNumber + 1);
        sysml_state_remainingMs = 5000;
        sysml_state_phase = 1;
    }
    sysml_state_previousStop = io_3;
    sysml_state_previousBack = io_4;
    sysml_state_previousForward = io_5;
    bool sysml_value_0 = 0;
    sysml_lazy_29 = io_3;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_previousStop;
    }
    sysml_value_0 = sysml_lazy_29;
    bool sysml_value_1 = 0;
    sysml_lazy_30 = io_4;
    if (sysml_lazy_30) {
        sysml_lazy_30 = !sysml_state_previousBack;
    }
    sysml_value_1 = sysml_lazy_30;
    bool sysml_value_2 = 0;
    sysml_lazy_31 = io_5;
    if (sysml_lazy_31) {
        sysml_lazy_31 = !sysml_state_previousForward;
    }
    sysml_value_2 = sysml_lazy_31;
    bool sysml_value_3 = 0;
    sysml_lazy_32 = sysml_state_selectedColor == 0;
    if (sysml_lazy_32) {
        sysml_lazy_32 = sysml_state_seenRed;
    }
    sysml_lazy_33 = sysml_lazy_32;
    if (!sysml_lazy_33) {
        sysml_lazy_34 = sysml_state_selectedColor == 1;
        if (sysml_lazy_34) {
            sysml_lazy_34 = sysml_state_seenYellow;
        }
        sysml_lazy_33 = sysml_lazy_34;
    }
    sysml_lazy_35 = sysml_lazy_33;
    if (!sysml_lazy_35) {
        sysml_lazy_36 = sysml_state_selectedColor == 2;
        if (sysml_lazy_36) {
            sysml_lazy_36 = sysml_state_seenGreen;
        }
        sysml_lazy_35 = sysml_lazy_36;
    }
    sysml_value_3 = sysml_lazy_35;
    bool sysml_value_4 = 0;
    sysml_lazy_37 = sysml_state_phase == 1;
    if (sysml_lazy_37) {
        sysml_lazy_37 = sysml_state_selectedColor == 0;
    }
    sysml_value_4 = sysml_lazy_37;
    bool sysml_value_5 = 0;
    sysml_lazy_38 = sysml_state_phase == 1;
    if (sysml_lazy_38) {
        sysml_lazy_38 = sysml_state_selectedColor == 1;
    }
    sysml_value_5 = sysml_lazy_38;
    bool sysml_value_6 = 0;
    sysml_lazy_39 = sysml_state_phase == 1;
    if (sysml_lazy_39) {
        sysml_lazy_39 = sysml_state_selectedColor == 2;
    }
    sysml_value_6 = sysml_lazy_39;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_score;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_readyReport;
    bool sysml_value_9 = 0;
    sysml_value_9 = sysml_state_wrongReport;
    bool sysml_value_10 = 0;
    sysml_value_10 = sysml_state_timeoutReport;
    bool sysml_value_11 = 0;
    sysml_value_11 = sysml_state_successReport;
    int sysml_value_12 = 0;
    sysml_value_12 = sysml_state_score;
    bool sysml_value_13 = 0;
    sysml_value_13 = sysml_state_endReport;
    int sysml_value_14 = 0;
    sysml_value_14 = sysml_state_score;
    bool sysml_value_15 = 0;
    sysml_lazy_40 = sysml_state_selectedColor == 0;
    if (sysml_lazy_40) {
        sysml_lazy_40 = sysml_value_0;
    }
    sysml_lazy_41 = sysml_lazy_40;
    if (!sysml_lazy_41) {
        sysml_lazy_42 = sysml_state_selectedColor == 1;
        if (sysml_lazy_42) {
            sysml_lazy_42 = sysml_value_1;
        }
        sysml_lazy_41 = sysml_lazy_42;
    }
    sysml_lazy_43 = sysml_lazy_41;
    if (!sysml_lazy_43) {
        sysml_lazy_44 = sysml_state_selectedColor == 2;
        if (sysml_lazy_44) {
            sysml_lazy_44 = sysml_value_2;
        }
        sysml_lazy_43 = sysml_lazy_44;
    }
    sysml_value_15 = sysml_lazy_43;
    bool sysml_value_16 = 0;
    sysml_lazy_45 = sysml_value_0;
    if (!sysml_lazy_45) {
        sysml_lazy_45 = sysml_value_1;
    }
    sysml_lazy_46 = sysml_lazy_45;
    if (!sysml_lazy_46) {
        sysml_lazy_46 = sysml_value_2;
    }
    sysml_value_16 = sysml_lazy_46;
    *io_0 = sysml_value_4;
    *io_1 = sysml_value_5;
    *io_2 = sysml_value_6;
    *io_6 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *serial_0_trigger = sysml_value_11;
    *serial_0_value_0 = sysml_value_12;
    *serial_1_trigger = sysml_value_13;
    *serial_1_value_0 = sysml_value_14;
}