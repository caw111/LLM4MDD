#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_phaseIndex = 0;
/* Global Variable define of  */
int sysml_state_accumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_phaseIndex = 0;
    sysml_state_accumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_arithmetic_1 = 0;
    bool sysml_lazy_2 = 0;
    sysml_state_accumMs = (sysml_state_accumMs + io_0);
    if ((sysml_state_accumMs >= 2000)) {
        sysml_state_accumMs = (sysml_state_accumMs - 2000);
        if ((((double)(sysml_state_phaseIndex + 1)) / 4) < ((double)((int)(((double)(sysml_state_phaseIndex + 1)) / 4)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_arithmetic_1 = ((int)(((double)(sysml_state_phaseIndex + 1)) / 4)) - sysml_conditional_0;
        sysml_state_phaseIndex = (sysml_state_phaseIndex + 1) - (4 * sysml_arithmetic_1);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_phaseIndex == 0);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_phaseIndex == 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_phaseIndex == 2);
    bool sysml_value_3 = 0;
    sysml_value_3 = (!(sysml_state_phaseIndex == 3));
    bool sysml_value_4 = 0;
    sysml_lazy_2 = sysml_state_phaseIndex == 1;
    if (!sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_phaseIndex == 2;
    }
    sysml_value_4 = sysml_lazy_2;
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_phaseIndex == 2);
    bool sysml_value_6 = 0;
    sysml_value_6 = false;
    bool sysml_value_7 = 0;
    sysml_value_7 = false;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    bool sysml_value_9 = 0;
    sysml_value_9 = false;
    bool sysml_value_10 = 0;
    sysml_value_10 = false;
    bool sysml_value_11 = 0;
    sysml_value_11 = false;
    bool sysml_value_12 = 0;
    sysml_value_12 = (sysml_state_phaseIndex == 3);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4_segment_0 = sysml_value_3;
    *io_4_segment_1 = sysml_value_4;
    *io_4_segment_2 = sysml_value_5;
    *io_4_segment_3 = sysml_value_6;
    *io_4_segment_4 = sysml_value_7;
    *io_4_segment_5 = sysml_value_8;
    *io_4_segment_6 = sysml_value_9;
    *io_4_segment_7 = sysml_value_10;
    *io_4_segment_8 = sysml_value_11;
    *io_4_segment_9 = sysml_value_12;
}