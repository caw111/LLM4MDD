#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_lightBelowThreshold = false;
/* Global Variable define of  */
double sysml_state_threshold = 500.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_lightBelowThreshold = false;
    sysml_state_threshold = 500.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_lightBelowThreshold = (io_0 < sysml_state_threshold);
    sysml_state_threshold = sysml_state_threshold;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_lightBelowThreshold;
    *io_1 = sysml_value_0;
}