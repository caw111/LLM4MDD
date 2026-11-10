#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayValue = 0;
/* Global Variable define of  */
int sysml_state_barLitCount = 0;
/* Global Variable define of  */
int sysml_state_tensDigit = 0;
/* Global Variable define of  */
int sysml_state_unitsDigit = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayValue = 0;
    sysml_state_barLitCount = 0;
    sysml_state_tensDigit = 0;
    sysml_state_unitsDigit = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, int* io_0, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_1 <= 0.0) {
        sysml_conditional_0 = 0;
    }
    else {
        if (io_1 >= 99.0) {
            sysml_conditional_1 = 99;
        }
        else {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)io_1) - sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_displayValue = sysml_conditional_0;
    if ((((double)sysml_state_displayValue) / 10) < ((double)((int)(((double)sysml_state_displayValue) / 10)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_state_tensDigit = ((int)(((double)sysml_state_displayValue) / 10)) - sysml_conditional_3;
    if ((((double)sysml_state_displayValue) / 10) < ((double)((int)(((double)sysml_state_displayValue) / 10)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_arithmetic_5 = ((int)(((double)sysml_state_displayValue) / 10)) - sysml_conditional_4;
    sysml_state_unitsDigit = sysml_state_displayValue - (10 * sysml_arithmetic_5);
    if ((((double)sysml_state_displayValue) / 10) < ((double)((int)(((double)sysml_state_displayValue) / 10)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_state_barLitCount = ((int)(((double)sysml_state_displayValue) / 10)) - sysml_conditional_6;
    int sysml_value_0 = 0;
    sysml_value_0 = ((10 * sysml_state_tensDigit) + sysml_state_unitsDigit);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_barLitCount >= 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_barLitCount >= 2);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_barLitCount >= 3);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_barLitCount >= 4);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_barLitCount >= 5);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_barLitCount >= 6);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_barLitCount >= 7);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_barLitCount >= 8);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_barLitCount >= 9);
    bool sysml_value_10 = 0;
    sysml_value_10 = false;
    *io_0 = sysml_value_0;
    *io_2_segment_0 = sysml_value_1;
    *io_2_segment_1 = sysml_value_2;
    *io_2_segment_2 = sysml_value_3;
    *io_2_segment_3 = sysml_value_4;
    *io_2_segment_4 = sysml_value_5;
    *io_2_segment_5 = sysml_value_6;
    *io_2_segment_6 = sysml_value_7;
    *io_2_segment_7 = sysml_value_8;
    *io_2_segment_8 = sysml_value_9;
    *io_2_segment_9 = sysml_value_10;
}