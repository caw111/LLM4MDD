#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double* io_0, int* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    if (io_1 > 30.0) {
        if (io_1 >= 40.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            if (io_1 >= 35.0) {
                sysml_conditional_2 = 160.0;
            }
            else {
                sysml_conditional_2 = 80.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    int sysml_value_1 = 0;
    if (io_1 >= 40.0) {
        sysml_conditional_3 = 2000;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_1 = sysml_conditional_3;
    int sysml_value_2 = 0;
    if (io_1 > 30.0) {
        if (io_1 >= 40.0) {
            sysml_conditional_5 = 4;
        }
        else {
            if (io_1 >= 35.0) {
                sysml_conditional_6 = 3;
            }
            else {
                if (io_1 >= 30.0) {
                    sysml_conditional_7 = 2;
                }
                else {
                    sysml_conditional_7 = 1;
                }
                sysml_conditional_6 = sysml_conditional_7;
            }
            sysml_conditional_5 = sysml_conditional_6;
        }
        sysml_conditional_4 = sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_0 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}