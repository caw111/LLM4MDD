#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_acceptedFrame = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_acceptedFrame = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_0_valid, int* io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (io_0_valid) {
        sysml_state_acceptedFrame = io_0;
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_acceptedFrame;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_acceptedFrame;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}