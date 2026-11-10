#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_distance_cm_last_valid = 0.0;
/* Global Variable define of  */
double sysml_state_distance_cm_effective = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_distance_cm_last_valid = 0.0;
    sysml_state_distance_cm_effective = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2, bool* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    if (io_1) {
        sysml_state_distance_cm_last_valid = io_0;
    }
    if (io_1) {
        sysml_state_distance_cm_effective = io_0;
    }
    else {
        sysml_state_distance_cm_effective = sysml_state_distance_cm_last_valid;
    }
    int sysml_value_0 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_distance_cm_effective >= 2.0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_distance_cm_effective <= 400.0;
    }
    if (sysml_lazy_1) {
        if (sysml_state_distance_cm_effective < ((double)((int)sysml_state_distance_cm_effective))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int)sysml_state_distance_cm_effective) - sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_0 = sysml_conditional_2;
    bool sysml_value_1 = 0;
    sysml_lazy_4 = io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_distance_cm_effective < 20.0;
    }
    sysml_value_1 = sysml_lazy_4;
    double sysml_value_2 = 0;
    if (io_1) {
        if (sysml_state_distance_cm_effective <= 10.0) {
            sysml_conditional_6 = 1023.0;
        }
        else {
            if (sysml_state_distance_cm_effective >= 100.0) {
                sysml_conditional_7 = 0.0;
            }
            else {
                if (sysml_state_distance_cm_effective > 10.0) {
                    if ((((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0) < ((double)((int)(((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0)))) {
                        sysml_conditional_9 = 1;
                    }
                    else {
                        sysml_conditional_9 = 0;
                    }
                    sysml_conditional_8 = ((int)(((double)((100.0 - sysml_state_distance_cm_effective) * 1023.0)) / 90.0)) - sysml_conditional_9;
                }
                else {
                    sysml_conditional_8 = 1023.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
}