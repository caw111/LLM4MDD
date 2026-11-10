#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_redIntensity = 0;
/* Global Variable define of  */
int sysml_state_greenIntensity = 0;
/* Global Variable define of  */
int sysml_state_blueIntensity = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_redIntensity = 0;
    sysml_state_greenIntensity = 0;
    sysml_state_blueIntensity = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, double io_4, double io_5, double* io_6, double* io_7, double* io_8) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_0) {
        sysml_lazy_0 = io_0;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !false;
        }
        if (sysml_lazy_0) {
            if ((((double)(io_3 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 255.0)) / 1023.0)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_conditional_1 = ((int)(((double)(io_3 * 255.0)) / 1023.0)) - sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_redIntensity = sysml_conditional_1;
    }
    if (io_1) {
        if (io_1) {
            if ((((double)(io_4 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_4 * 255.0)) / 1023.0)))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)(((double)(io_4 * 255.0)) / 1023.0)) - sysml_conditional_4;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_state_greenIntensity = sysml_conditional_3;
    }
    if (io_2) {
        if (io_2) {
            if ((((double)(io_5 * 255.0)) / 1023.0) < ((double)((int)(((double)(io_5 * 255.0)) / 1023.0)))) {
                sysml_conditional_6 = 1;
            }
            else {
                sysml_conditional_6 = 0;
            }
            sysml_conditional_5 = ((int)(((double)(io_5 * 255.0)) / 1023.0)) - sysml_conditional_6;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_state_blueIntensity = sysml_conditional_5;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_redIntensity;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_greenIntensity;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_blueIntensity;
    *io_6 = sysml_value_0;
    *io_7 = sysml_value_1;
    *io_8 = sysml_value_2;
}