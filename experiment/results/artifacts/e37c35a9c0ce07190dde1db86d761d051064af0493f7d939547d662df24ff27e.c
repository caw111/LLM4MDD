#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_measuredCm_i32 = 0;
/* Global Variable define of  */
int sysml_state_targetCm_i32 = 0;
/* Global Variable define of  */
int sysml_state_comparisonResultCode_i32 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_measuredCm_i32 = 0;
    sysml_state_targetCm_i32 = 0;
    sysml_state_comparisonResultCode_i32 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool io_2_valid, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_lazy_7 = 0;
    if (io_1) {
        if (io_0 < 0.0) {
            sysml_conditional_0 = 0;
        }
        else {
            if (io_0 > 20000.0) {
                sysml_conditional_1 = 20000;
            }
            else {
                if (io_0 < ((double)((int)io_0))) {
                    sysml_conditional_2 = 1;
                }
                else {
                    sysml_conditional_2 = 0;
                }
                sysml_conditional_1 = ((int)io_0) - sysml_conditional_2;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_measuredCm_i32 = sysml_conditional_0;
    }
    if (io_2_valid) {
        if (io_2 < 0) {
            sysml_conditional_3 = 0;
        }
        else {
            if (io_2 > 20000) {
                sysml_conditional_4 = 20000;
            }
            else {
                sysml_conditional_4 = io_2;
            }
            sysml_conditional_3 = sysml_conditional_4;
        }
        sysml_state_targetCm_i32 = sysml_conditional_3;
    }
    if (sysml_state_measuredCm_i32 > sysml_state_targetCm_i32) {
        sysml_conditional_5 = 0;
    }
    else {
        if (sysml_state_measuredCm_i32 == sysml_state_targetCm_i32) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 2;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_state_comparisonResultCode_i32 = sysml_conditional_5;
    bool sysml_value_0 = 0;
    sysml_value_0 = true;
    bool sysml_value_1 = 0;
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_7 = io_2_valid;
    }
    sysml_value_1 = sysml_lazy_7;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_measuredCm_i32;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_targetCm_i32;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_comparisonResultCode_i32;
    *io_3 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
}