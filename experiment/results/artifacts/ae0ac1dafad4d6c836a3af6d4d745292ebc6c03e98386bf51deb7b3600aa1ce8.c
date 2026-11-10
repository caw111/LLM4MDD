#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_counterValue = 0;
/* Global Variable define of  */
int sysml_state_repeatAge = 0;
/* Global Variable define of  */
bool sysml_state_previousIncrement = false;
/* Global Variable define of  */
bool sysml_state_previousDecrement = false;
/* Global Variable define of  */
bool sysml_state_previousReset = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_counterValue = 0;
    sysml_state_repeatAge = 0;
    sysml_state_previousIncrement = false;
    sysml_state_previousDecrement = false;
    sysml_state_previousReset = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int io_5, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    int sysml_conditional_16 = 0;
    if (io_2) {
        sysml_state_counterValue = 0;
        sysml_state_repeatAge = 0;
    }
    else {
        if (((io_0 != io_1))) {
            sysml_lazy_0 = io_0;
            if (sysml_lazy_0) {
                sysml_lazy_0 = !sysml_state_previousIncrement;
            }
            sysml_lazy_1 = sysml_lazy_0;
            if (!sysml_lazy_1) {
                sysml_lazy_2 = io_1;
                if (sysml_lazy_2) {
                    sysml_lazy_2 = !sysml_state_previousDecrement;
                }
                sysml_lazy_1 = sysml_lazy_2;
            }
            if (sysml_lazy_1) {
                if (io_0) {
                    if (sysml_state_counterValue == 99) {
                        sysml_conditional_4 = 0;
                    }
                    else {
                        sysml_conditional_4 = sysml_state_counterValue + 1;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                else {
                    if (sysml_state_counterValue == 0) {
                        sysml_conditional_5 = 99;
                    }
                    else {
                        sysml_conditional_5 = sysml_state_counterValue - 1;
                    }
                    sysml_conditional_3 = sysml_conditional_5;
                }
                sysml_state_counterValue = sysml_conditional_3;
                sysml_state_repeatAge = 0;
            }
            else {
                sysml_lazy_6 = sysml_state_previousIncrement != sysml_state_previousDecrement;
                if (sysml_lazy_6) {
                    sysml_lazy_6 = !sysml_state_previousReset;
                }
                if (sysml_lazy_6) {
                    if (io_5 >= (1000 - sysml_state_repeatAge)) {
                        sysml_conditional_7 = 1000;
                    }
                    else {
                        sysml_conditional_7 = sysml_state_repeatAge + io_5;
                    }
                    sysml_state_repeatAge = sysml_conditional_7;
                }
                else {
                    sysml_state_repeatAge = 0;
                }
                if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                if (sysml_state_repeatAge >= (100 + (((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_8))) {
                    if (io_0) {
                        if (sysml_state_counterValue == 99) {
                            sysml_conditional_10 = 0;
                        }
                        else {
                            sysml_conditional_10 = sysml_state_counterValue + 1;
                        }
                        sysml_conditional_9 = sysml_conditional_10;
                    }
                    else {
                        if (sysml_state_counterValue == 0) {
                            sysml_conditional_11 = 99;
                        }
                        else {
                            sysml_conditional_11 = sysml_state_counterValue - 1;
                        }
                        sysml_conditional_9 = sysml_conditional_11;
                    }
                    sysml_state_counterValue = sysml_conditional_9;
                    sysml_state_repeatAge = 0;
                }
            }
        }
        else {
            sysml_state_repeatAge = 0;
        }
    }
    sysml_state_previousIncrement = io_0;
    sysml_state_previousDecrement = io_1;
    sysml_state_previousReset = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = (io_0 != io_1);
    bool sysml_value_1 = 0;
    sysml_lazy_12 = io_0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = !sysml_state_previousIncrement;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (!sysml_lazy_13) {
        sysml_lazy_14 = io_1;
        if (sysml_lazy_14) {
            sysml_lazy_14 = !sysml_state_previousDecrement;
        }
        sysml_lazy_13 = sysml_lazy_14;
    }
    sysml_value_1 = sysml_lazy_13;
    bool sysml_value_2 = 0;
    sysml_lazy_15 = sysml_state_previousIncrement != sysml_state_previousDecrement;
    if (sysml_lazy_15) {
        sysml_lazy_15 = !sysml_state_previousReset;
    }
    sysml_value_2 = sysml_lazy_15;
    int sysml_value_3 = 0;
    if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_3 = 100 + (((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_16);
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_counterValue;
    *io_4 = sysml_value_4;
}