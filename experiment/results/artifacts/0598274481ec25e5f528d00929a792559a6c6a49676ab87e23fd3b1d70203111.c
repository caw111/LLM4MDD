#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_alertLevel = 0;
/* Global Variable define of  */
double sysml_state_keyValueTemp = 0.0;
/* Global Variable define of  */
double sysml_state_keyValueSmoke = 0.0;
/* Global Variable define of  */
bool sysml_state_flameDetectedLatched = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_alertLevel = 0;
    sysml_state_keyValueTemp = 0.0;
    sysml_state_keyValueSmoke = 0.0;
    sysml_state_flameDetectedLatched = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int* io_3, bool* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_0_value_2, int* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_state_flameDetectedLatched = io_2;
    sysml_lazy_0 = sysml_state_flameDetectedLatched;
    if (!sysml_lazy_0) {
        sysml_lazy_1 = io_0 >= 600.0;
        if (sysml_lazy_1) {
            sysml_lazy_1 = io_1 >= 600.0;
        }
        sysml_lazy_2 = sysml_lazy_1;
        if (!sysml_lazy_2) {
            sysml_lazy_3 = io_0 >= 700.0;
            if (sysml_lazy_3) {
                sysml_lazy_3 = !(io_0 >= 900.0);
            }
            sysml_lazy_2 = sysml_lazy_3;
        }
        sysml_lazy_4 = sysml_lazy_2;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = io_1 >= 900.0;
        }
        sysml_lazy_0 = sysml_lazy_4;
    }
    if (sysml_lazy_0) {
        sysml_state_alertLevel = 2;
        sysml_state_keyValueTemp = io_0;
        sysml_state_keyValueSmoke = io_1;
    }
    else {
        sysml_lazy_5 = io_0 >= 650.0;
        if (!sysml_lazy_5) {
            sysml_lazy_5 = io_1 >= 650.0;
        }
        if (sysml_lazy_5) {
            sysml_state_alertLevel = 1;
            sysml_state_keyValueTemp = io_0;
            sysml_state_keyValueSmoke = io_1;
        }
        else {
            sysml_state_alertLevel = 0;
            sysml_state_keyValueTemp = 0.0;
            sysml_state_keyValueSmoke = 0.0;
        }
    }
    int sysml_value_0 = 0;
    if (sysml_state_alertLevel == 2) {
        sysml_conditional_6 = 2000;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_0 = sysml_conditional_6;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_alertLevel == 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_alertLevel == 1);
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alertLevel;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_alertLevel == 2);
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_keyValueTemp;
    double sysml_value_6 = 0;
    sysml_value_6 = sysml_state_keyValueSmoke;
    bool sysml_value_7 = 0;
    sysml_value_7 = sysml_state_flameDetectedLatched;
    int sysml_value_8 = 0;
    sysml_value_8 = sysml_state_alertLevel;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
}