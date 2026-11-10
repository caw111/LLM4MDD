#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_peopleCount = 0;
/* Global Variable define of  */
bool sysml_state_previousEntryPressed = false;
/* Global Variable define of  */
bool sysml_state_displayTemperature = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_peopleCount = 0;
    sysml_state_previousEntryPressed = false;
    sysml_state_displayTemperature = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, double io_4, double io_5, bool io_6, bool io_9, double* io_7, int* io_8, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    double sysml_conditional_10 = 0;
    sysml_lazy_0 = io_9;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousEntryPressed;
    }
    if (sysml_lazy_0) {
        sysml_state_displayTemperature = (!sysml_state_displayTemperature);
        sysml_state_previousEntryPressed = io_9;
    }
    else {
        sysml_state_previousEntryPressed = io_9;
    }
    if (io_1) {
        if (io_3) {
            if (io_0 <= 0.0) {
                sysml_conditional_1 = 0;
            }
            else {
                sysml_conditional_1 = 0;
            }
            sysml_state_peopleCount = sysml_conditional_1;
        }
        else {
            if (io_0 <= 0.0) {
                sysml_conditional_2 = 0;
            }
            else {
                sysml_conditional_2 = 0;
            }
            sysml_state_peopleCount = sysml_conditional_2;
        }
    }
    if ((!io_1)) {
        if (io_3) {
            sysml_state_peopleCount = 0;
        }
        else {
            sysml_state_peopleCount = 0;
        }
    }
    if ((!io_6)) {
    }
    double sysml_value_0 = 0;
    if (sysml_state_peopleCount == 0) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (io_4 > 30.0) {
            sysml_conditional_4 = 255.0;
        }
        else {
            if (io_4 > 25.0) {
                sysml_conditional_5 = 180.0;
            }
            else {
                if (io_4 > 20.0) {
                    sysml_conditional_6 = 120.0;
                }
                else {
                    sysml_conditional_6 = 60.0;
                }
                sysml_conditional_5 = sysml_conditional_6;
            }
            sysml_conditional_4 = sysml_conditional_5;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_3;
    int sysml_value_1 = 0;
    if (sysml_state_displayTemperature) {
        if (io_6) {
            if (io_4 < ((double)((int)io_4))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_conditional_8 = ((int)io_4) - sysml_conditional_9;
        }
        else {
            sysml_conditional_8 = 0;
        }
        sysml_conditional_7 = sysml_conditional_8;
    }
    else {
        sysml_conditional_7 = sysml_state_peopleCount;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    sysml_value_2 = true;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_peopleCount;
    double sysml_value_4 = 0;
    if (io_6) {
        sysml_conditional_10 = io_4;
    }
    else {
        sysml_conditional_10 = 0.0;
    }
    sysml_value_4 = sysml_conditional_10;
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_peopleCount == 0);
    *io_7 = sysml_value_0;
    *io_8 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
    *serial_0_value_1 = sysml_value_4;
    *serial_0_value_2 = sysml_value_5;
}