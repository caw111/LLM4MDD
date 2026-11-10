#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_modeIsAuto = false;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_modeIsAuto = false;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, double io_3, double io_4, double io_5, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    double sysml_arithmetic_4 = 0;
    double sysml_arithmetic_5 = 0;
    double sysml_arithmetic_6 = 0;
    int sysml_conditional_7 = 0;
    double sysml_arithmetic_8 = 0;
    double sysml_arithmetic_9 = 0;
    double sysml_arithmetic_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_modeIsAuto = true;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_1) {
        sysml_state_modeIsAuto = false;
    }
    sysml_state_previousBtn1 = io_1;
    sysml_state_previousBtn2 = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_modeIsAuto) {
        if (io_5 > io_4) {
            sysml_arithmetic_4 = ((double)1.0) / 1.0;
            sysml_arithmetic_5 = ((double)1.0) / 1.0;
            sysml_arithmetic_6 = ((double)1.0) / 1.0;
            if ((io_4 + ((io_5 - io_4) * sysml_arithmetic_5)) < ((double)((int)(io_4 + ((io_5 - io_4) * sysml_arithmetic_6))))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_3 = ((int)(io_4 + ((io_5 - io_4) * sysml_arithmetic_4))) - sysml_conditional_7;
        }
        else {
            sysml_arithmetic_8 = ((double)1.0) / 1.0;
            sysml_arithmetic_9 = ((double)1.0) / 1.0;
            sysml_arithmetic_10 = ((double)1.0) / 1.0;
            if ((io_5 + ((io_4 - io_5) * sysml_arithmetic_9)) < ((double)((int)(io_5 + ((io_4 - io_5) * sysml_arithmetic_10))))) {
                sysml_conditional_11 = 1;
            }
            else {
                sysml_conditional_11 = 0;
            }
            sysml_conditional_3 = ((int)(io_5 + ((io_4 - io_5) * sysml_arithmetic_8))) - sysml_conditional_11;
        }
        sysml_conditional_2 = sysml_conditional_3;
    }
    else {
        if (io_3 < ((double)((int)io_3))) {
            sysml_conditional_12 = 1;
        }
        else {
            sysml_conditional_12 = 0;
        }
        sysml_conditional_2 = ((int)io_3) - sysml_conditional_12;
    }
    sysml_value_0 = sysml_conditional_2;
    *io_0 = sysml_value_0;
}