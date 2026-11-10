#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_digit = 0;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Global Variable define of  */
int sysml_state_pulseIndex = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
    sysml_state_phaseMs = 0;
    sysml_state_pulseIndex = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, double io_3, double io_4, int* io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_phaseMs = 0;
    sysml_state_pulseIndex = 0;
    sysml_state_digit = 0;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_digit;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = false;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
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
    *io_0 = sysml_value_0;
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