#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_lastDisplayedValidCm = 0;
/* Global Variable define of  */
bool sysml_state_hasEverDisplayedValid = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastDisplayedValidCm = 0;
    sysml_state_hasEverDisplayedValid = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    if (io_1) {
        if (io_0 < ((double)((int)io_0))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_lastDisplayedValidCm = ((int)io_0) - sysml_conditional_0;
        sysml_state_hasEverDisplayedValid = true;
    }
    if ((!io_1)) {
    }
    int sysml_value_0 = 0;
    if (io_1) {
        sysml_lazy_2 = io_0 >= 2.0;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_0 <= 400.0;
        }
        if (sysml_lazy_2) {
            if (io_0 < ((double)((int)io_0))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_conditional_3 = ((int)io_0) - sysml_conditional_4;
        }
        else {
            if (sysml_state_hasEverDisplayedValid) {
                sysml_conditional_5 = 10000;
            }
            else {
                sysml_conditional_5 = 10000;
            }
            sysml_conditional_3 = sysml_conditional_5;
        }
        sysml_conditional_1 = sysml_conditional_3;
    }
    else {
        sysml_conditional_1 = 10000;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_2 = sysml_value_0;
}