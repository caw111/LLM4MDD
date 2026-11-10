#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistance = 0.0;
/* Global Variable define of  */
bool sysml_state_hasEverValidDistance = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistance = 0.0;
    sysml_state_hasEverValidDistance = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_2, bool io_3, bool* io_0, int* io_1, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    if (io_3) {
        sysml_state_lastValidDistance = io_2;
        sysml_state_hasEverValidDistance = true;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_0 = sysml_state_hasEverValidDistance;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_lastValidDistance < 30.0;
    }
    sysml_value_0 = sysml_lazy_0;
    int sysml_value_1 = 0;
    if (!sysml_state_hasEverValidDistance) {
        sysml_conditional_1 = 0;
    }
    else {
        if (sysml_state_lastValidDistance < 8.0) {
            sysml_conditional_2 = 1000;
        }
        else {
            if (sysml_state_lastValidDistance < 15.0) {
                sysml_conditional_3 = 600;
            }
            else {
                sysml_conditional_3 = 0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_1 = sysml_conditional_1;
    int sysml_value_2 = 0;
    if (sysml_state_hasEverValidDistance) {
        if (sysml_state_lastValidDistance < ((double)((int)sysml_state_lastValidDistance))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int)sysml_state_lastValidDistance) - sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_2 = sysml_conditional_4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_4 = sysml_value_2;
}