#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_prevBed1 = false;
/* Global Variable define of  */
bool sysml_state_prevBed2 = false;
/* Global Variable define of  */
bool sysml_state_prevBed3 = false;
/* Global Variable define of  */
bool sysml_state_prevAck = false;
/* Global Variable define of  */
bool sysml_state_pending = false;
/* Global Variable define of  */
int sysml_state_activeBed = 0;
/* Global Variable define of  */
int sysml_state_queueSeq = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_prevBed1 = false;
    sysml_state_prevBed2 = false;
    sysml_state_prevBed3 = false;
    sysml_state_prevAck = false;
    sysml_state_pending = false;
    sysml_state_activeBed = 0;
    sysml_state_queueSeq = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBed1;
    }
    if (sysml_lazy_0) {
        sysml_state_pending = true;
        sysml_state_activeBed = 1;
        sysml_state_queueSeq = (sysml_state_queueSeq + 1);
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevBed2;
    }
    if (sysml_lazy_1) {
        sysml_state_pending = true;
        sysml_state_activeBed = 2;
        sysml_state_queueSeq = (sysml_state_queueSeq + 1);
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBed3;
    }
    if (sysml_lazy_2) {
        sysml_state_pending = true;
        sysml_state_activeBed = 3;
        sysml_state_queueSeq = (sysml_state_queueSeq + 1);
    }
    sysml_lazy_3 = io_3;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevAck;
    }
    if (sysml_lazy_3) {
        sysml_state_pending = false;
        sysml_state_activeBed = 0;
    }
    sysml_state_prevBed1 = io_0;
    sysml_state_prevBed2 = io_1;
    sysml_state_prevBed3 = io_2;
    sysml_state_prevAck = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_pending;
    int sysml_value_1 = 0;
    if (sysml_state_pending) {
        sysml_conditional_4 = 1000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_1 = sysml_conditional_4;
    int sysml_value_2 = 0;
    if (sysml_state_pending) {
        sysml_conditional_5 = 1000;
    }
    else {
        sysml_conditional_5 = 0;
    }
    sysml_value_2 = sysml_conditional_5;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
}