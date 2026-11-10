#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Global Variable define of  */
int sysml_state_positionDeg = 0;
/* Global Variable define of  */
bool sysml_state_direction = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_accumulatedMs = 0;
    sysml_state_positionDeg = 0;
    sysml_state_direction = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, double* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_0);
    if ((sysml_state_accumulatedMs >= 2000)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
        if (sysml_state_direction) {
            if (((sysml_state_positionDeg + 10) <= 180)) {
                sysml_state_positionDeg = (sysml_state_positionDeg + 10);
            }
            else {
                sysml_state_positionDeg = 180;
                sysml_state_direction = false;
            }
        }
        else {
            if (((sysml_state_positionDeg - 10) >= 0)) {
                sysml_state_positionDeg = (sysml_state_positionDeg - 10);
            }
            else {
                sysml_state_positionDeg = 0;
                sysml_state_direction = true;
            }
        }
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_positionDeg;
    *io_1 = sysml_value_0;
}