#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_segmentIndex = 0;
/* Global Variable define of  */
int sysml_state_accumulated = 0;
/* Global Variable define of  */
bool sysml_state_ledToggle = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_segmentIndex = 0;
    sysml_state_accumulated = 0;
    sysml_state_ledToggle = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_3, bool* io_0, bool* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    sysml_state_accumulated = (sysml_state_accumulated + io_3);
    if ((sysml_state_accumulated >= 2000)) {
        sysml_state_accumulated = (sysml_state_accumulated - 2000);
        sysml_state_ledToggle = (!sysml_state_ledToggle);
        if ((sysml_state_segmentIndex < 9)) {
            sysml_state_segmentIndex = (sysml_state_segmentIndex + 1);
        }
        else {
            sysml_state_segmentIndex = 0;
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_ledToggle;
    bool sysml_value_1 = 0;
    if (sysml_state_ledToggle) {
        sysml_conditional_0 = false;
    }
    else {
        sysml_conditional_0 = true;
    }
    sysml_value_1 = sysml_conditional_0;
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_segmentIndex >= 0);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_segmentIndex >= 1);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_segmentIndex >= 2);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_segmentIndex >= 3);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_segmentIndex >= 4);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_segmentIndex >= 5);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_segmentIndex >= 6);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_segmentIndex >= 7);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_segmentIndex >= 8);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_segmentIndex >= 9);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2_segment_0 = sysml_value_2;
    *io_2_segment_1 = sysml_value_3;
    *io_2_segment_2 = sysml_value_4;
    *io_2_segment_3 = sysml_value_5;
    *io_2_segment_4 = sysml_value_6;
    *io_2_segment_5 = sysml_value_7;
    *io_2_segment_6 = sysml_value_8;
    *io_2_segment_7 = sysml_value_9;
    *io_2_segment_8 = sysml_value_10;
    *io_2_segment_9 = sysml_value_11;
}