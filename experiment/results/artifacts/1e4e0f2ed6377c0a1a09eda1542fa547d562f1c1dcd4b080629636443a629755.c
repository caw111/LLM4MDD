#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mode = 2;
/* Global Variable define of  */
double sysml_state_brightness = 0.0;
/* Global Variable define of  */
int sysml_state_phaseMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mode = 2;
    sysml_state_brightness = 0.0;
    sysml_state_phaseMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, bool io_1_valid, int io_2, double* io_0, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    double sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    if (io_1_valid) {
        sysml_state_mode = io_1;
    }
    sysml_state_phaseMs = (sysml_state_phaseMs + io_2);
    if ((sysml_state_mode == 1)) {
        if (sysml_state_phaseMs < 1000) {
            sysml_conditional_0 = (((double)(sysml_state_phaseMs * 1.0)) / 1000.0) * 255.0;
        }
        else {
            if (sysml_state_phaseMs < 2000) {
                sysml_conditional_1 = (((double)((2000 - sysml_state_phaseMs) * 1.0)) / 1000.0) * 255.0;
            }
            else {
                if (sysml_state_phaseMs < 3000) {
                    sysml_conditional_2 = (((double)((sysml_state_phaseMs - 2000) * 1.0)) / 1000.0) * 255.0;
                }
                else {
                    if (sysml_state_phaseMs < 4000) {
                        sysml_conditional_3 = (((double)((4000 - sysml_state_phaseMs) * 1.0)) / 1000.0) * 255.0;
                    }
                    else {
                        if (sysml_state_phaseMs < 5000) {
                            sysml_conditional_4 = (((double)((sysml_state_phaseMs - 4000) * 1.0)) / 1000.0) * 255.0;
                        }
                        else {
                            if (sysml_state_phaseMs < 6000) {
                                sysml_conditional_5 = (((double)((6000 - sysml_state_phaseMs) * 1.0)) / 1000.0) * 255.0;
                            }
                            else {
                                sysml_conditional_5 = 0.0;
                            }
                            sysml_conditional_4 = sysml_conditional_5;
                        }
                        sysml_conditional_3 = sysml_conditional_4;
                    }
                    sysml_conditional_2 = sysml_conditional_3;
                }
                sysml_conditional_1 = sysml_conditional_2;
            }
            sysml_conditional_0 = sysml_conditional_1;
        }
        sysml_state_brightness = sysml_conditional_0;
    }
    else {
        sysml_state_brightness = 255.0;
    }
    sysml_lazy_6 = sysml_state_mode == 2;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_brightness != 255.0;
    }
    if (sysml_lazy_6) {
        sysml_state_brightness = 255.0;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_brightness;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_mode;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
}