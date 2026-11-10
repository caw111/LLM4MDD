#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
int sysml_state_elapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_elapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int* io_6, bool* io_7_segment_0, bool* io_7_segment_1, bool* io_7_segment_2, bool* io_7_segment_3, bool* io_7_segment_4, bool* io_7_segment_5, bool* io_7_segment_6, bool* io_7_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_count = 0;
    sysml_state_elapsedMs = 0;
    double sysml_value_0 = 0;
    sysml_value_0 = 0.0;
    double sysml_value_1 = 0;
    sysml_value_1 = 0.0;
    double sysml_value_2 = 0;
    sysml_value_2 = 0.0;
    double sysml_value_3 = 0;
    sysml_value_3 = 0.0;
    double sysml_value_4 = 0;
    sysml_value_4 = 0.0;
    double sysml_value_5 = 0;
    sysml_value_5 = 0.0;
    int sysml_value_6 = 0;
    sysml_value_6 = 0;
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
    sysml_value_12 = false;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    bool sysml_value_14 = 0;
    sysml_value_14 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7_segment_0 = sysml_value_7;
    *io_7_segment_1 = sysml_value_8;
    *io_7_segment_2 = sysml_value_9;
    *io_7_segment_3 = sysml_value_10;
    *io_7_segment_4 = sysml_value_11;
    *io_7_segment_5 = sysml_value_12;
    *io_7_segment_6 = sysml_value_13;
    *io_7_segment_7 = sysml_value_14;
}