#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_inWashStage = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Global Variable define of  */
bool sysml_state_faultActive = false;
/* Global Variable define of  */
bool sysml_state_endConfirmed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_inWashStage = 0;
    sysml_state_prevPressed = false;
    sysml_state_faultActive = false;
    sysml_state_endConfirmed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, double io_5, bool* io_1, int* io_2, int* io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    if ((!io_4)) {
        sysml_state_faultActive = false;
        sysml_state_endConfirmed = false;
        sysml_state_inWashStage = 0;
        sysml_state_prevPressed = false;
    }
    else {
        if (sysml_state_faultActive) {
            sysml_state_inWashStage = 0;
            sysml_state_endConfirmed = sysml_state_endConfirmed;
        }
        sysml_lazy_0 = io_0;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_prevPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_endConfirmed = true;
        }
        sysml_lazy_1 = !sysml_state_faultActive;
        if (sysml_lazy_1) {
            sysml_lazy_1 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_1) {
            sysml_state_inWashStage = 3;
        }
        else {
            sysml_state_inWashStage = sysml_state_inWashStage;
        }
        sysml_state_prevPressed = io_4;
    }
    bool sysml_value_0 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_lazy_3 = sysml_state_inWashStage == 3;
        if (sysml_lazy_3) {
            sysml_lazy_3 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_3) {
            sysml_conditional_4 = true;
        }
        else {
            sysml_conditional_4 = false;
        }
        sysml_conditional_2 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_5 = 0;
    }
    else {
        sysml_lazy_6 = sysml_state_inWashStage == 3;
        if (sysml_lazy_6) {
            sysml_lazy_6 = !sysml_state_endConfirmed;
        }
        if (sysml_lazy_6) {
            sysml_conditional_7 = 1000;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_5 = sysml_conditional_7;
    }
    sysml_value_1 = sysml_conditional_5;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_inWashStage;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_faultActive;
    int sysml_value_4 = 0;
    if (sysml_state_faultActive) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_4 = sysml_conditional_8;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}