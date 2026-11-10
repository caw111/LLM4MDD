#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    if ((((double)((1023.0 - io_0) * 10.0)) / 1023.0) < ((double)((int)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = ((int)(((double)((1023.0 - io_0) * 10.0)) / 1023.0)) - sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_value_0 > 0);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_value_0 > 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_value_0 > 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_value_0 > 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_value_0 > 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_value_0 > 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_value_0 > 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_value_0 > 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_value_0 > 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_value_0 > 9);
    *io_1_segment_0 = sysml_value_1;
    *io_1_segment_1 = sysml_value_2;
    *io_1_segment_2 = sysml_value_3;
    *io_1_segment_3 = sysml_value_4;
    *io_1_segment_4 = sysml_value_5;
    *io_1_segment_5 = sysml_value_6;
    *io_1_segment_6 = sysml_value_7;
    *io_1_segment_7 = sysml_value_8;
    *io_1_segment_8 = sysml_value_9;
    *io_1_segment_9 = sysml_value_10;
}