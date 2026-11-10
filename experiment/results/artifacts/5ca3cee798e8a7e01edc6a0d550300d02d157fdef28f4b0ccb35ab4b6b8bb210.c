#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_activeAngleDeg = 0.0;
/* Global Variable define of  */
int sysml_state_activeLedLevel = 0;
/* Global Variable define of  */
int sysml_state_holdElapsedMs = 0;
/* Global Variable define of  */
bool sysml_state_btn1PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn2PrevPressed = false;
/* Global Variable define of  */
bool sysml_state_btn3PrevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_activeAngleDeg = 0.0;
    sysml_state_activeLedLevel = 0;
    sysml_state_holdElapsedMs = 0;
    sysml_state_btn1PrevPressed = false;
    sysml_state_btn2PrevPressed = false;
    sysml_state_btn3PrevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_btn1PrevPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_activeAngleDeg = 30.0;
        sysml_state_activeLedLevel = 3;
        sysml_state_holdElapsedMs = 0;
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_btn2PrevPressed;
    }
    if (sysml_lazy_1) {
        sysml_state_activeAngleDeg = 60.0;
        sysml_state_activeLedLevel = 6;
        sysml_state_holdElapsedMs = 0;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_btn3PrevPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_activeAngleDeg = 90.0;
        sysml_state_activeLedLevel = 10;
        sysml_state_holdElapsedMs = 0;
    }
    if ((sysml_state_holdElapsedMs < 2000)) {
        sysml_state_holdElapsedMs = sysml_state_holdElapsedMs;
    }
    sysml_state_btn1PrevPressed = io_0;
    sysml_state_btn2PrevPressed = io_1;
    sysml_state_btn3PrevPressed = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_activeLedLevel >= 1);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_activeLedLevel >= 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_activeLedLevel >= 3);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_activeLedLevel >= 4);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_activeLedLevel >= 5);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_activeLedLevel >= 6);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_activeLedLevel >= 7);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_activeLedLevel >= 8);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_activeLedLevel >= 9);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_activeLedLevel >= 10);
    double sysml_value_10 = 0;
    sysml_value_10 = sysml_state_activeAngleDeg;
    *io_3_segment_0 = sysml_value_0;
    *io_3_segment_1 = sysml_value_1;
    *io_3_segment_2 = sysml_value_2;
    *io_3_segment_3 = sysml_value_3;
    *io_3_segment_4 = sysml_value_4;
    *io_3_segment_5 = sysml_value_5;
    *io_3_segment_6 = sysml_value_6;
    *io_3_segment_7 = sysml_value_7;
    *io_3_segment_8 = sysml_value_8;
    *io_3_segment_9 = sysml_value_9;
    *io_4 = sysml_value_10;
}