#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_cachedEpoch = 0;
/* Global Variable define of  */
bool sysml_state_hasReading = false;
/* Global Variable define of  */
int sysml_state_dayOffset = 0;
/* Global Variable define of  */
bool sysml_state_previousBackPressed = false;
/* Global Variable define of  */
bool sysml_state_previousForwardPressed = false;
/* Global Variable define of  */
bool sysml_state_backPressEvent = false;
/* Global Variable define of  */
bool sysml_state_forwardPressEvent = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_cachedEpoch = 0;
    sysml_state_hasReading = false;
    sysml_state_dayOffset = 0;
    sysml_state_previousBackPressed = false;
    sysml_state_previousForwardPressed = false;
    sysml_state_backPressEvent = false;
    sysml_state_forwardPressEvent = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool io_2, bool io_3, int* io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBackPressed;
    }
    sysml_state_backPressEvent = sysml_lazy_0;
    sysml_lazy_1 = io_3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousForwardPressed;
    }
    sysml_state_forwardPressEvent = sysml_lazy_1;
    if (io_1) {
        sysml_state_cachedEpoch = io_0;
        sysml_state_hasReading = true;
    }
    sysml_lazy_2 = sysml_state_backPressEvent;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_forwardPressEvent;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0)) >= (-2147397248.0);
    }
    if (sysml_lazy_3) {
        sysml_state_dayOffset = (sysml_state_dayOffset - 1);
    }
    else {
        sysml_lazy_4 = sysml_state_forwardPressEvent;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_backPressEvent;
        }
        sysml_lazy_5 = sysml_lazy_4;
        if (sysml_lazy_5) {
            sysml_lazy_5 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0)) <= 2147397247.0;
        }
        if (sysml_lazy_5) {
            sysml_state_dayOffset = (sysml_state_dayOffset + 1);
        }
    }
    sysml_state_previousBackPressed = io_2;
    sysml_state_previousForwardPressed = io_3;
    double sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_cachedEpoch + (sysml_state_dayOffset * 86400.0));
    int sysml_value_1 = 0;
    if (sysml_value_0 < (-2147483648.0)) {
        sysml_conditional_6 = (-2147483647) - 1;
    }
    else {
        if (sysml_value_0 > 2147483647.0) {
            sysml_conditional_7 = 2147483647;
        }
        else {
            if (sysml_value_0 < ((double)((int)sysml_value_0))) {
                sysml_conditional_8 = 1;
            }
            else {
                sysml_conditional_8 = 0;
            }
            sysml_conditional_7 = ((int)sysml_value_0) - sysml_conditional_8;
        }
        sysml_conditional_6 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    if (sysml_state_hasReading) {
        sysml_conditional_9 = sysml_value_1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_2 = sysml_conditional_9;
    int sysml_value_3 = 0;
    if (sysml_state_hasReading) {
        sysml_conditional_10 = sysml_value_1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_3 = sysml_conditional_10;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
}