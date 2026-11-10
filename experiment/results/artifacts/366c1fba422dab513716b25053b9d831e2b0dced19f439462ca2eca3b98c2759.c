#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_framePhase = 0;
/* Global Variable define of  */
int sysml_state_pot0 = 0;
/* Global Variable define of  */
int sysml_state_pot1 = 0;
/* Global Variable define of  */
int sysml_state_pot2 = 0;
/* Global Variable define of  */
int sysml_state_pot3 = 0;
/* Global Variable define of  */
int sysml_state_pot4 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_framePhase = 0;
    sysml_state_pot0 = 0;
    sysml_state_pot1 = 0;
    sysml_state_pot2 = 0;
    sysml_state_pot3 = 0;
    sysml_state_pot4 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_4, bool* io_0, bool* io_1, bool* io_2, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, int* serial_0_value_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    if ((sysml_state_framePhase == 0)) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        sysml_state_pot0 = ((int)io_4) - sysml_conditional_0;
        sysml_state_framePhase = 1;
    }
    if ((sysml_state_framePhase == 1)) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_pot1 = ((int)io_4) - sysml_conditional_1;
        sysml_state_framePhase = 2;
    }
    if ((sysml_state_framePhase == 2)) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        sysml_state_pot2 = ((int)io_4) - sysml_conditional_2;
        sysml_state_framePhase = 3;
    }
    if ((sysml_state_framePhase == 3)) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_state_pot3 = ((int)io_4) - sysml_conditional_3;
        sysml_state_framePhase = 4;
    }
    if ((sysml_state_framePhase == 4)) {
        if (io_4 < ((double)((int)io_4))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_state_pot4 = ((int)io_4) - sysml_conditional_4;
        sysml_state_framePhase = 0;
    }
    bool sysml_value_0 = 0;
    sysml_lazy_5 = sysml_state_framePhase == 0;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_framePhase == 2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_framePhase == 4;
    }
    sysml_value_0 = sysml_lazy_6;
    bool sysml_value_1 = 0;
    sysml_lazy_7 = sysml_state_framePhase == 1;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_framePhase == 2;
    }
    sysml_value_1 = sysml_lazy_7;
    bool sysml_value_2 = 0;
    sysml_lazy_8 = sysml_state_framePhase == 3;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_framePhase == 4;
    }
    sysml_value_2 = sysml_lazy_8;
    bool sysml_value_3 = 0;
    sysml_value_3 = false;
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_framePhase == 4);
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_pot0;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_pot1;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_pot2;
    int sysml_value_8 = 0;
    sysml_value_8 = sysml_state_pot3;
    int sysml_value_9 = 0;
    sysml_value_9 = sysml_state_pot4;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *serial_0_trigger = sysml_value_4;
    *serial_0_value_0 = sysml_value_5;
    *serial_0_value_1 = sysml_value_6;
    *serial_0_value_2 = sysml_value_7;
    *serial_0_value_3 = sysml_value_8;
    *serial_0_value_4 = sysml_value_9;
}