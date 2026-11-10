#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stage = 1;
/* Global Variable define of  */
int sysml_state_remainingMs = 0;
/* Global Variable define of  */
double sysml_state_targetTempC = 60.0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Global Variable define of  */
bool sysml_state_running = false;
/* Global Variable define of  */
bool sysml_state_beepActive = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stage = 1;
    sysml_state_remainingMs = 0;
    sysml_state_targetTempC = 60.0;
    sysml_state_previousPressed = false;
    sysml_state_running = false;
    sysml_state_beepActive = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, double* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    if (true) {
        sysml_lazy_0 = io_1;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousPressed;
        }
        if (sysml_lazy_0) {
            sysml_state_running = true;
        }
        sysml_state_previousPressed = io_1;
    }
    if (sysml_state_running) {
        sysml_state_remainingMs = sysml_state_remainingMs;
        if ((io_0 >= sysml_state_targetTempC)) {
            sysml_state_beepActive = true;
        }
        else {
            sysml_state_beepActive = false;
        }
        if ((sysml_state_remainingMs <= 0)) {
            if (sysml_state_stage == 1) {
                sysml_conditional_1 = 2;
            }
            else {
                if (sysml_state_stage == 2) {
                    sysml_conditional_2 = 3;
                }
                else {
                    sysml_conditional_2 = 3;
                }
                sysml_conditional_1 = sysml_conditional_2;
            }
            sysml_state_stage = sysml_conditional_1;
            if (sysml_state_stage == 1) {
                sysml_conditional_3 = 80.0;
            }
            else {
                if (sysml_state_stage == 2) {
                    sysml_conditional_4 = 60.0;
                }
                else {
                    sysml_conditional_4 = 55.0;
                }
                sysml_conditional_3 = sysml_conditional_4;
            }
            sysml_state_targetTempC = sysml_conditional_3;
            sysml_state_remainingMs = 30000;
        }
    }
    int sysml_value_0 = 0;
    if (sysml_state_beepActive) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_0 = sysml_conditional_5;
    bool sysml_value_1 = 0;
    if (sysml_state_running) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_1 = sysml_conditional_6;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_stage;
    bool sysml_value_3 = 0;
    if (sysml_state_running) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_3 = sysml_conditional_7;
    int sysml_value_4 = 0;
    sysml_value_4 = 0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_stage;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
}