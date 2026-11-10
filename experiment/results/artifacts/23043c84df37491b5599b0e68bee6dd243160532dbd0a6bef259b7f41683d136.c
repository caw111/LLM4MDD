#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevIo7Pressed = false;
/* Global Variable define of  */
int sysml_state_countReusable = 0;
/* Global Variable define of  */
int sysml_state_countOther = 0;
/* Global Variable define of  */
bool sysml_state_alarmLatched = false;
/* Global Variable define of  */
bool sysml_state_prevFullState = false;
/* Global Variable define of  */
double sysml_state_lastValidDistance = 0.0;
/* Global Variable define of  */
bool sysml_state_hasEverValidDistance = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevIo7Pressed = false;
    sysml_state_countReusable = 0;
    sysml_state_countOther = 0;
    sysml_state_alarmLatched = false;
    sysml_state_prevFullState = false;
    sysml_state_lastValidDistance = 0.0;
    sysml_state_hasEverValidDistance = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, bool* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    if (io_3) {
        sysml_state_lastValidDistance = io_2;
        sysml_state_hasEverValidDistance = true;
    }
    sysml_state_prevIo7Pressed = io_7;
    if (sysml_state_hasEverValidDistance) {
        sysml_conditional_0 = sysml_state_lastValidDistance >= 20.0;
    }
    else {
        sysml_conditional_0 = false;
    }
    sysml_state_prevFullState = sysml_conditional_0;
    sysml_lazy_1 = !sysml_state_prevIo7Pressed;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_7;
    }
    if (sysml_lazy_1) {
        sysml_state_countReusable = 0;
        sysml_state_countOther = 0;
        sysml_state_alarmLatched = false;
        sysml_state_prevFullState = false;
    }
    sysml_lazy_2 = !sysml_state_alarmLatched;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_hasEverValidDistance;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_lastValidDistance >= 20.0;
    }
    if (sysml_lazy_3) {
        sysml_state_alarmLatched = true;
    }
    sysml_lazy_4 = !sysml_state_alarmLatched;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_0;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !(sysml_state_lastValidDistance >= 20.0);
    }
    if (sysml_lazy_5) {
        sysml_state_countReusable = (sysml_state_countReusable + 1);
    }
    sysml_lazy_6 = !sysml_state_alarmLatched;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !(sysml_state_lastValidDistance >= 20.0);
    }
    if (sysml_lazy_7) {
        sysml_state_countOther = (sysml_state_countOther + 1);
    }
    int sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_countReusable + sysml_state_countOther);
    int sysml_value_1 = 0;
    if (sysml_state_alarmLatched) {
        sysml_conditional_8 = 2000;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_1 = sysml_conditional_8;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_alarmLatched;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_alarmLatched;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_alarmLatched;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}