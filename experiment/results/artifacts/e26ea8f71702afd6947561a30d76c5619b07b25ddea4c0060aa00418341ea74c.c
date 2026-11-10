#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_activePot = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_activePot = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool io_3, double io_4, double io_5, double io_6, int io_7, double* io_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    sysml_lazy_0 = io_1;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !io_2;
    }
    if (sysml_lazy_0) {
        sysml_state_activePot = 1;
    }
    if (io_2) {
        sysml_state_activePot = 2;
    }
    if (io_3) {
        sysml_state_activePot = 3;
    }
    double sysml_value_0 = 0;
    if (sysml_state_activePot == 1) {
        sysml_conditional_1 = (io_4 * 180.0) + 0.0;
    }
    else {
        if (sysml_state_activePot == 2) {
            sysml_conditional_2 = (io_5 * 180.0) + 0.0;
        }
        else {
            if (sysml_state_activePot == 3) {
                sysml_conditional_3 = (io_6 * 180.0) + 0.0;
            }
            else {
                sysml_conditional_3 = 0.0;
            }
            sysml_conditional_2 = sysml_conditional_3;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    *io_0 = sysml_value_0;
}