#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_litCount = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debounceArmed = true;
/* Global Variable define of  */
int sysml_state_debounceRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_litCount = 0;
    sysml_state_previousPressed = false;
    sysml_state_debounceArmed = true;
    sysml_state_debounceRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_debounceArmed;
    }
    if (sysml_lazy_1) {
        if (sysml_state_litCount == 10) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_litCount + 1;
        }
        sysml_state_litCount = sysml_conditional_2;
        sysml_state_debounceArmed = false;
        sysml_state_debounceRemainingMs = 150;
    }
    sysml_lazy_3 = sysml_state_debounceRemainingMs >= 5;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_debounceArmed;
    }
    if (sysml_lazy_3) {
        sysml_state_debounceRemainingMs = (sysml_state_debounceRemainingMs - 5);
        if (((sysml_state_debounceRemainingMs - 5) <= 0)) {
            sysml_state_debounceArmed = true;
        }
    }
    sysml_lazy_4 = sysml_state_debounceRemainingMs < 5;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_debounceArmed;
    }
    if (sysml_lazy_4) {
        sysml_state_debounceArmed = true;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_litCount >= 1);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_litCount >= 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_litCount >= 3);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_litCount >= 4);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_litCount >= 5);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_litCount >= 6);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_litCount >= 7);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_litCount >= 8);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_litCount >= 9);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_litCount >= 10);
    *io_1_segment_0 = sysml_value_0;
    *io_1_segment_1 = sysml_value_1;
    *io_1_segment_2 = sysml_value_2;
    *io_1_segment_3 = sysml_value_3;
    *io_1_segment_4 = sysml_value_4;
    *io_1_segment_5 = sysml_value_5;
    *io_1_segment_6 = sysml_value_6;
    *io_1_segment_7 = sysml_value_7;
    *io_1_segment_8 = sysml_value_8;
    *io_1_segment_9 = sysml_value_9;
}