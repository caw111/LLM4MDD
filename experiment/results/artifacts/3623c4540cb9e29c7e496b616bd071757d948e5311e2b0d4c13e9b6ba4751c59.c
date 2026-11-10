#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_lastValidDistanceCm = 0.0;
/* Global Variable define of  */
int sysml_state_displayedDistanceIntCm = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lastValidDistanceCm = 0.0;
    sysml_state_displayedDistanceIntCm = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, bool io_2, bool* io_0, int* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    if (io_2) {
        sysml_state_lastValidDistanceCm = io_1;
        if (io_1 >= 2.0) {
            if (io_1 < ((double)((int)io_1))) {
                sysml_conditional_1 = 1;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_conditional_0 = ((int)io_1) - sysml_conditional_1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_displayedDistanceIntCm = sysml_conditional_0;
    }
    else {
        sysml_state_displayedDistanceIntCm = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_1 < 10.0;
    }
    sysml_value_0 = sysml_lazy_2;
    int sysml_value_1 = 0;
    if (io_2) {
        sysml_conditional_3 = sysml_state_displayedDistanceIntCm;
    }
    else {
        sysml_conditional_3 = 0;
    }
    sysml_value_1 = sysml_conditional_3;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
}