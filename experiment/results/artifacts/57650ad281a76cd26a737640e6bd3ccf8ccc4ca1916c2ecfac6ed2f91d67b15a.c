#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 1;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 1;
    sysml_state_previousBtn1 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, bool io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    double sysml_conditional_9 = 0;
    if (io_4) {
        sysml_state_mode = 1;
    }
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (sysml_state_mode == 1) {
            sysml_conditional_1 = 2;
        }
        else {
            if (sysml_state_mode == 2) {
                sysml_conditional_2 = 3;
            }
            else {
                if (sysml_state_mode == 3) {
                    sysml_conditional_3 = 4;
                }
                else {
                    sysml_conditional_3 = 1;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_mode = sysml_conditional_1;
    }
    sysml_state_previousBtn1 = io_3;
    double sysml_value_0 = 0;
    sysml_lazy_4 = sysml_state_mode == 1;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_mode == 4;
    }
    if (sysml_lazy_4) {
        sysml_conditional_5 = io_0;
    }
    else {
        sysml_conditional_5 = 0.0;
    }
    sysml_value_0 = sysml_conditional_5;
    double sysml_value_1 = 0;
    sysml_lazy_6 = sysml_state_mode == 2;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_mode == 4;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = io_1;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_1 = sysml_conditional_7;
    double sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_mode == 3;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_mode == 4;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = io_2;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
}