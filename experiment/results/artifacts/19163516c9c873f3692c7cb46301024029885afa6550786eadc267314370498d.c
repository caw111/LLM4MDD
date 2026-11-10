#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_digit = 0;
/* Global Variable define of  */
int sysml_state_elapsedAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_parityEven = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_digit = 0;
    sysml_state_elapsedAccumMs = 0;
    sysml_state_parityEven = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_5, bool* io_0, bool* io_1, int* io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9, bool* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_arithmetic_4 = 0;
    sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs + io_5);
    if ((sysml_state_elapsedAccumMs >= 2000)) {
        sysml_state_elapsedAccumMs = (sysml_state_elapsedAccumMs - 2000);
        if (sysml_state_digit == 9) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_digit + 1;
        }
        sysml_state_digit = sysml_conditional_0;
        if ((((double)sysml_state_digit) / 2) < ((double)((int)(((double)sysml_state_digit) / 2)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_arithmetic_2 = ((int)(((double)sysml_state_digit) / 2)) - sysml_conditional_1;
        sysml_state_parityEven = sysml_state_digit == (2 * sysml_arithmetic_2);
    }
    if ((sysml_state_elapsedAccumMs < 2000)) {
    }
    if ((sysml_state_elapsedAccumMs >= 2000)) {
        if ((((double)sysml_state_digit) / 2) < ((double)((int)(((double)sysml_state_digit) / 2)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_arithmetic_4 = ((int)(((double)sysml_state_digit) / 2)) - sysml_conditional_3;
        sysml_state_parityEven = sysml_state_digit == (2 * sysml_arithmetic_4);
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_parityEven;
    bool sysml_value_1 = 0;
    sysml_value_1 = (!sysml_state_parityEven);
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_digit;
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_digit >= 1);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_digit >= 2);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_digit >= 3);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_digit >= 4);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_digit >= 5);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_digit >= 6);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_digit >= 7);
    bool sysml_value_10 = 0;
    sysml_value_10 = (sysml_state_digit >= 8);
    bool sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_digit >= 9);
    bool sysml_value_12 = 0;
    sysml_value_12 = false;
    bool sysml_value_13 = 0;
    sysml_value_13 = false;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3_segment_0 = sysml_value_3;
    *io_3_segment_1 = sysml_value_4;
    *io_3_segment_2 = sysml_value_5;
    *io_3_segment_3 = sysml_value_6;
    *io_3_segment_4 = sysml_value_7;
    *io_3_segment_5 = sysml_value_8;
    *io_3_segment_6 = sysml_value_9;
    *io_3_segment_7 = sysml_value_10;
    *io_3_segment_8 = sysml_value_11;
    *io_3_segment_9 = sysml_value_12;
    *io_4 = sysml_value_13;
}