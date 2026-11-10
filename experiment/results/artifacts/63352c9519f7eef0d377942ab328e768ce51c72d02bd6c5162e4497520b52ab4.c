#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_directionReversed = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_directionReversed = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0_segment_0, bool* io_0_segment_1, bool* io_0_segment_2, bool* io_0_segment_3, bool* io_0_segment_4, bool* io_0_segment_5, bool* io_0_segment_6, bool* io_0_segment_7, bool* io_0_segment_8, bool* io_0_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_directionReversed = (!sysml_state_directionReversed);
    }
    sysml_state_previousPressed = io_2;
    bool sysml_value_0 = 0;
    if (sysml_state_directionReversed) {
        sysml_lazy_2 = io_1 >= 0.0;
        if (!sysml_lazy_2) {
            sysml_lazy_2 = false;
        }
        sysml_conditional_1 = sysml_lazy_2;
    }
    else {
        if (io_1 < ((double)((int)io_1))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_lazy_4 = (((int)io_1) - sysml_conditional_3) >= 1;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = false;
        }
        sysml_conditional_1 = sysml_lazy_4;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    sysml_lazy_5 = sysml_state_directionReversed;
    if (sysml_lazy_5) {
        if (io_1 < ((double)((int)io_1))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_lazy_5 = (((int)io_1) - sysml_conditional_6) <= 9;
    }
    sysml_lazy_7 = sysml_lazy_5;
    if (sysml_lazy_7) {
        if (io_1 < ((double)((int)io_1))) {
            sysml_conditional_8 = 1;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_lazy_7 = (((int)io_1) - sysml_conditional_8) >= 1;
    }
    sysml_value_1 = sysml_lazy_7;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    *io_0_segment_0 = sysml_value_0;
    *io_0_segment_1 = sysml_value_1;
    *io_0_segment_2 = sysml_value_2;
    *io_0_segment_3 = sysml_value_3;
    *io_0_segment_4 = sysml_value_4;
    *io_0_segment_5 = sysml_value_5;
    *io_0_segment_6 = sysml_value_6;
    *io_0_segment_7 = sysml_value_7;
    *io_0_segment_8 = sysml_value_8;
    *io_0_segment_9 = sysml_value_9;
}