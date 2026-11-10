#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_savedThreshold = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_savedThreshold = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_0_valid, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_0_valid) {
        sysml_state_savedThreshold = io_0;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = (1 < sysml_state_savedThreshold);
    bool sysml_value_1 = 0;
    sysml_value_1 = (2 < sysml_state_savedThreshold);
    bool sysml_value_2 = 0;
    sysml_value_2 = (3 < sysml_state_savedThreshold);
    bool sysml_value_3 = 0;
    sysml_value_3 = (4 < sysml_state_savedThreshold);
    bool sysml_value_4 = 0;
    sysml_value_4 = (5 < sysml_state_savedThreshold);
    bool sysml_value_5 = 0;
    sysml_value_5 = (6 < sysml_state_savedThreshold);
    bool sysml_value_6 = 0;
    sysml_value_6 = (7 < sysml_state_savedThreshold);
    bool sysml_value_7 = 0;
    sysml_value_7 = (8 < sysml_state_savedThreshold);
    bool sysml_value_8 = 0;
    sysml_value_8 = (9 < sysml_state_savedThreshold);
    bool sysml_value_9 = 0;
    sysml_value_9 = (10 < sysml_state_savedThreshold);
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