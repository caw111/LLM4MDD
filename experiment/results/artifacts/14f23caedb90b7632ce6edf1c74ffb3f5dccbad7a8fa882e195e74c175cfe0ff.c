#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_yellowRemainingMs = 0;
/* Global Variable define of  */
int sysml_state_redRemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed = false;
    sysml_state_mode = 0;
    sysml_state_yellowRemainingMs = 0;
    sysml_state_redRemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool* io_1, bool* io_2, bool* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_mode = 1;
        sysml_state_yellowRemainingMs = 1000;
        sysml_state_redRemainingMs = 2000;
    }
    if ((sysml_state_mode == 1)) {
        sysml_state_yellowRemainingMs = (sysml_state_yellowRemainingMs - 10);
        if (((sysml_state_yellowRemainingMs - 10) <= 0)) {
            sysml_state_mode = 2;
        }
    }
    if ((sysml_state_mode == 2)) {
        sysml_state_redRemainingMs = (sysml_state_redRemainingMs - 10);
        if (((sysml_state_redRemainingMs - 10) <= 0)) {
            sysml_state_mode = 0;
        }
    }
    sysml_state_previousPressed = io_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_mode == 2);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mode == 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_mode == 0);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
}