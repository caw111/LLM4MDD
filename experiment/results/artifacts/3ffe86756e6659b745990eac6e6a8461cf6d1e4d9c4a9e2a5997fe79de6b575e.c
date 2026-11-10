#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_0 = 0.0;
    }
    else {
        if (io_0 > 0.0) {
            if (io_0 <= 1023.0) {
                if (io_0 == 0.0) {
                    sysml_conditional_3 = 1023.0;
                }
                else {
                    if ((1023.0 - io_0) == 1023.0) {
                        sysml_conditional_4 = 1023.0;
                    }
                    else {
                        sysml_conditional_4 = 1023.0 - io_0;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            else {
                sysml_conditional_2 = 0.0;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        else {
            sysml_conditional_1 = 1023.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    if (io_0 > 800.0) {
        sysml_conditional_6 = 1000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_2 = sysml_conditional_6;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}