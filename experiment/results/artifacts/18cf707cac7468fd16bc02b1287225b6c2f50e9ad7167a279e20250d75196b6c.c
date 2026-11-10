#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    sysml_value_0 = (io_0 >= 1.0);
    bool sysml_value_1 = 0;
    sysml_value_1 = (io_0 >= 2.0);
    bool sysml_value_2 = 0;
    sysml_value_2 = (io_0 >= 3.0);
    bool sysml_value_3 = 0;
    sysml_value_3 = (io_1 >= 1.0);
    bool sysml_value_4 = 0;
    sysml_value_4 = (io_1 >= 2.0);
    bool sysml_value_5 = 0;
    sysml_value_5 = (io_1 >= 3.0);
    bool sysml_value_6 = 0;
    sysml_value_6 = (io_2 >= 1.0);
    bool sysml_value_7 = 0;
    sysml_value_7 = (io_2 >= 2.0);
    bool sysml_value_8 = 0;
    sysml_value_8 = (io_2 >= 3.0);
    bool sysml_value_9 = 0;
    sysml_value_9 = (io_2 >= 4.0);
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
}