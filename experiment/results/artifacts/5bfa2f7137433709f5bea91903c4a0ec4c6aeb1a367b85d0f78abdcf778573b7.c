#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stage = 1;
/* Global Variable define of  */
bool sysml_state_activeTemperatureReached = false;
/* Global Variable define of  */
int sysml_state_remainingSeconds = 0;
/* Global Variable define of  */
int sysml_state_totalSeconds = 0;
/* Global Variable define of  */
bool sysml_state_heatingActive = false;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stage = 1;
    sysml_state_activeTemperatureReached = false;
    sysml_state_remainingSeconds = 0;
    sysml_state_totalSeconds = 0;
    sysml_state_heatingActive = false;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, double* serial_0_value_2, int* serial_0_value_3, bool* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if (false) {
    }
    else {
        sysml_state_previousPressed = io_1;
    }
    int sysml_value_0 = 0;
    sysml_value_0 = 0;
    bool sysml_value_1 = 0;
    sysml_value_1 = false;
    int sysml_value_2 = 0;
    sysml_value_2 = 0;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    int sysml_value_4 = 0;
    sysml_value_4 = 0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_stage;
    double sysml_value_6 = 0;
    sysml_value_6 = io_0;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_remainingSeconds;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_activeTemperatureReached;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
    *serial_0_value_4 = sysml_value_8;
}