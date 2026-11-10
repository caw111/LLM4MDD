#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_dhtValid = false;
/* Global Variable define of  */
bool sysml_state_lastTempValid = false;
/* Global Variable define of  */
bool sysml_state_lastHumValid = false;
/* Global Variable define of  */
bool sysml_state_showTemp = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_dhtValid = false;
    sysml_state_lastTempValid = false;
    sysml_state_lastHumValid = false;
    sysml_state_showTemp = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool* io_0, int* io_3, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    sysml_lazy_0 = sysml_state_lastTempValid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_lastHumValid;
    }
    if (sysml_lazy_0) {
        sysml_state_showTemp = (!sysml_state_showTemp);
    }
    else {
        sysml_state_showTemp = true;
    }
    sysml_state_lastTempValid = true;
    sysml_state_lastHumValid = true;
    sysml_lazy_1 = sysml_state_lastTempValid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_lastHumValid;
    }
    sysml_state_dhtValid = sysml_lazy_1;
    bool sysml_value_0 = 0;
    if (sysml_state_dhtValid) {
        sysml_conditional_2 = false;
    }
    else {
        sysml_conditional_2 = true;
    }
    sysml_value_0 = sysml_conditional_2;
    int sysml_value_1 = 0;
    sysml_lazy_3 = sysml_state_dhtValid;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_showTemp;
    }
    if (sysml_lazy_3) {
        sysml_conditional_4 = 0;
    }
    else {
        sysml_lazy_5 = sysml_state_dhtValid;
        if (sysml_lazy_5) {
            sysml_lazy_5 = !sysml_state_showTemp;
        }
        if (sysml_lazy_5) {
            sysml_conditional_6 = 0;
        }
        else {
            sysml_conditional_6 = 0;
        }
        sysml_conditional_4 = sysml_conditional_6;
    }
    sysml_value_1 = sysml_conditional_4;
    bool sysml_value_2 = 0;
    sysml_value_2 = (!sysml_state_dhtValid);
    int sysml_value_3 = 0;
    sysml_value_3 = 0;
    *io_0 = sysml_value_0;
    *io_3 = sysml_value_1;
    *serial_0_trigger = sysml_value_2;
    *serial_0_value_0 = sysml_value_3;
}