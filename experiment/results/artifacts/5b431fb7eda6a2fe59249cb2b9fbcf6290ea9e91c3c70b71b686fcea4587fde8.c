#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_progress = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_debounceActive = false;
/* Global Variable define of  */
int sysml_state_debounceElapsedMs = 0;
/* Global Variable define of  */
bool sysml_state_pendingPress = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_progress = 0;
    sysml_state_previousPressed = false;
    sysml_state_debounceActive = false;
    sysml_state_debounceElapsedMs = 0;
    sysml_state_pendingPress = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
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
    sysml_lazy_0 = sysml_state_debounceActive;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_debounceElapsedMs >= 150;
    }
    if (sysml_lazy_0) {
        sysml_state_debounceActive = false;
        sysml_state_debounceElapsedMs = 0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_debounceActive;
    }
    if (sysml_lazy_2) {
        sysml_state_pendingPress = true;
        sysml_state_debounceActive = true;
        sysml_state_debounceElapsedMs = 0;
    }
    sysml_lazy_3 = sysml_state_debounceActive;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_3) {
        sysml_state_debounceElapsedMs = (sysml_state_debounceElapsedMs + 5);
    }
    sysml_lazy_4 = sysml_state_debounceActive;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_previousPressed;
    }
    if (sysml_lazy_4) {
        sysml_state_debounceElapsedMs = (sysml_state_debounceElapsedMs + 5);
    }
    if (sysml_state_pendingPress) {
        if (sysml_state_progress >= 10) {
            sysml_conditional_5 = 0;
        }
        else {
            sysml_conditional_5 = sysml_state_progress + 1;
        }
        sysml_state_progress = sysml_conditional_5;
        sysml_state_pendingPress = false;
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_progress >= 1) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    if (sysml_state_progress >= 2) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    if (sysml_state_progress >= 3) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    if (sysml_state_progress >= 4) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_3 = sysml_conditional_9;
    bool sysml_value_4 = 0;
    if (sysml_state_progress >= 5) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_4 = sysml_conditional_10;
    bool sysml_value_5 = 0;
    if (sysml_state_progress >= 6) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_5 = sysml_conditional_11;
    bool sysml_value_6 = 0;
    if (sysml_state_progress >= 7) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if (sysml_state_progress >= 8) {
        sysml_conditional_13 = true;
    }
    else {
        sysml_conditional_13 = false;
    }
    sysml_value_7 = sysml_conditional_13;
    bool sysml_value_8 = 0;
    if (sysml_state_progress >= 9) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_8 = sysml_conditional_14;
    bool sysml_value_9 = 0;
    if (sysml_state_progress >= 10) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_9 = sysml_conditional_15;
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