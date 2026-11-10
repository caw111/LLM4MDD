#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_patternStarted = false;
/* Global Variable define of  */
int sysml_state_cycleMilliseconds = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_patternStarted = false;
    sysml_state_cycleMilliseconds = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_11, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_arithmetic_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    if ((((double)io_11) / 36000) < ((double)((int)(((double)io_11) / 36000)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_arithmetic_1 = ((int)(((double)io_11) / 36000)) - sysml_conditional_0;
    sysml_state_cycleMilliseconds = sysml_state_cycleMilliseconds + (io_11 - (36000 * sysml_arithmetic_1));
    if ((((double)sysml_state_cycleMilliseconds) / 36000) < ((double)((int)(((double)sysml_state_cycleMilliseconds) / 36000)))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_arithmetic_3 = ((int)(((double)sysml_state_cycleMilliseconds) / 36000)) - sysml_conditional_2;
    sysml_state_cycleMilliseconds = sysml_state_cycleMilliseconds - (36000 * sysml_arithmetic_3);
    sysml_state_patternStarted = true;
    int sysml_value_0 = 0;
    if ((((double)sysml_state_cycleMilliseconds) / 2000) < ((double)((int)(((double)sysml_state_cycleMilliseconds) / 2000)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_0 = ((int)(((double)sysml_state_cycleMilliseconds) / 2000)) - sysml_conditional_4;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    int sysml_value_2 = 0;
    if (sysml_value_0 <= 9) {
        sysml_conditional_5 = sysml_value_0 + 1;
    }
    else {
        sysml_conditional_5 = 19 - sysml_value_0;
    }
    sysml_value_2 = sysml_conditional_5;
    bool sysml_value_3 = 0;
    sysml_lazy_6 = sysml_state_patternStarted;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_value_2 == 1;
    }
    sysml_value_3 = sysml_lazy_6;
    bool sysml_value_4 = 0;
    sysml_lazy_7 = sysml_state_patternStarted;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_value_2 == 2;
    }
    sysml_value_4 = sysml_lazy_7;
    bool sysml_value_5 = 0;
    sysml_lazy_8 = sysml_state_patternStarted;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_value_2 == 3;
    }
    sysml_value_5 = sysml_lazy_8;
    bool sysml_value_6 = 0;
    sysml_lazy_9 = sysml_state_patternStarted;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_value_2 == 4;
    }
    sysml_value_6 = sysml_lazy_9;
    bool sysml_value_7 = 0;
    sysml_lazy_10 = sysml_state_patternStarted;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_value_2 == 5;
    }
    sysml_value_7 = sysml_lazy_10;
    bool sysml_value_8 = 0;
    sysml_lazy_11 = sysml_state_patternStarted;
    if (sysml_lazy_11) {
        sysml_lazy_11 = sysml_value_2 == 6;
    }
    sysml_value_8 = sysml_lazy_11;
    bool sysml_value_9 = 0;
    sysml_lazy_12 = sysml_state_patternStarted;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_value_2 == 7;
    }
    sysml_value_9 = sysml_lazy_12;
    bool sysml_value_10 = 0;
    sysml_lazy_13 = sysml_state_patternStarted;
    if (sysml_lazy_13) {
        sysml_lazy_13 = sysml_value_2 == 8;
    }
    sysml_value_10 = sysml_lazy_13;
    bool sysml_value_11 = 0;
    sysml_lazy_14 = sysml_state_patternStarted;
    if (sysml_lazy_14) {
        sysml_lazy_14 = sysml_value_2 == 9;
    }
    sysml_value_11 = sysml_lazy_14;
    bool sysml_value_12 = 0;
    sysml_lazy_15 = sysml_state_patternStarted;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_value_2 == 10;
    }
    sysml_value_12 = sysml_lazy_15;
    *io_0 = sysml_value_3;
    *io_1 = sysml_value_4;
    *io_2 = sysml_value_5;
    *io_3 = sysml_value_6;
    *io_4 = sysml_value_7;
    *io_5 = sysml_value_8;
    *io_6 = sysml_value_9;
    *io_7 = sysml_value_10;
    *io_8 = sysml_value_11;
    *io_9 = sysml_value_12;
    *io_10 = sysml_value_1;
}