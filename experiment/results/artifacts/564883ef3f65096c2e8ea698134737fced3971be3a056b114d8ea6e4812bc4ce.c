#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_blinkAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_led1On = false;
/* Global Variable define of  */
int sysml_state_countAccumMs = 0;
/* Global Variable define of  */
int sysml_state_countValue = 9;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_blinkAccumMs = 0;
    sysml_state_led1On = false;
    sysml_state_countAccumMs = 0;
    sysml_state_countValue = 9;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_3, bool* io_0, bool* io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs + io_3);
    if ((sysml_state_blinkAccumMs >= 2000)) {
        sysml_state_blinkAccumMs = (sysml_state_blinkAccumMs - 2000);
        sysml_state_led1On = (!sysml_state_led1On);
    }
    sysml_state_countAccumMs = (sysml_state_countAccumMs + io_3);
    if ((sysml_state_countAccumMs >= 4000)) {
        sysml_state_countAccumMs = (sysml_state_countAccumMs - 4000);
        if ((sysml_state_countValue == 0)) {
            sysml_state_countValue = 9;
        }
        else {
            sysml_state_countValue = (sysml_state_countValue - 1);
            if (((sysml_state_countValue - 1) == 0)) {
                sysml_state_countValue = 9;
            }
        }
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1On;
    bool sysml_value_1 = 0;
    sysml_value_1 = (!sysml_state_led1On);
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_countValue;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}