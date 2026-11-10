#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_toggle = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_toggle = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
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
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_0);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        sysml_state_toggle = (!sysml_state_toggle);
    }
    bool sysml_value_0 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_0 = false;
    }
    else {
        sysml_conditional_0 = true;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_value_1 = sysml_conditional_1;
    bool sysml_value_2 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = true;
    }
    sysml_value_2 = sysml_conditional_2;
    bool sysml_value_3 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_3 = true;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_value_3 = sysml_conditional_3;
    bool sysml_value_4 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_4 = false;
    }
    else {
        sysml_conditional_4 = true;
    }
    sysml_value_4 = sysml_conditional_4;
    bool sysml_value_5 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_5 = sysml_conditional_5;
    bool sysml_value_6 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_6 = false;
    }
    else {
        sysml_conditional_6 = true;
    }
    sysml_value_6 = sysml_conditional_6;
    bool sysml_value_7 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_7 = sysml_conditional_7;
    bool sysml_value_8 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_8 = false;
    }
    else {
        sysml_conditional_8 = true;
    }
    sysml_value_8 = sysml_conditional_8;
    bool sysml_value_9 = 0;
    if (sysml_state_toggle) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_9 = sysml_conditional_9;
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