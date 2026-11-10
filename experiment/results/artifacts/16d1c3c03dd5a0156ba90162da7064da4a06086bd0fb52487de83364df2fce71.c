#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
bool sysml_state_nextLedPhase = false;
/* Global Variable define of  */
int sysml_state_segmentIndex = 1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_nextLedPhase = false;
    sysml_state_segmentIndex = 1;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_14, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_14);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_segmentIndex >= 10) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = sysml_state_segmentIndex + 1;
        }
        sysml_state_segmentIndex = sysml_conditional_0;
        sysml_state_nextLedPhase = (!sysml_state_nextLedPhase);
    }
    bool sysml_value_0 = 0;
    sysml_lazy_1 = sysml_state_segmentIndex >= 1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_segmentIndex <= 10;
    }
    sysml_lazy_2 = sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_nextLedPhase;
    }
    sysml_value_0 = sysml_lazy_2;
    bool sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_segmentIndex >= 1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_segmentIndex <= 10;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_nextLedPhase;
    }
    sysml_value_1 = sysml_lazy_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_segmentIndex == 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_segmentIndex == 10);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_segmentIndex == 2);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_segmentIndex == 3);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_segmentIndex == 4);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_segmentIndex == 5);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_segmentIndex == 6);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_segmentIndex == 7);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_segmentIndex == 8);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_segmentIndex == 9);
    bool sysml_value_12 = 0;
    sysml_value_12 = false;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *io_11 = sysml_value_11;
    *io_12 = sysml_value_12;
    *io_13 = sysml_value_13;
}