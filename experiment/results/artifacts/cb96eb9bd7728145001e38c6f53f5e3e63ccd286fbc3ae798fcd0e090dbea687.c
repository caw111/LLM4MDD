#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_occupied = false;
/* Global Variable define of  */
bool sysml_state_overtimeActive = false;
/* Global Variable define of  */
int sysml_state_startMin = 0;
/* Global Variable define of  */
int sysml_state_endMin = 0;
/* Global Variable define of  */
bool sysml_state_prevPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_occupied = false;
    sysml_state_overtimeActive = false;
    sysml_state_startMin = 0;
    sysml_state_endMin = 0;
    sysml_state_prevPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_4, bool io_5, int io_8, bool io_8_valid, bool* io_1, int* io_2, int* io_3, bool* io_6, bool* io_7, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_conditional_14 = 0;
    bool sysml_conditional_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_8_valid;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_8 == 0;
    }
    if (sysml_lazy_0) {
        sysml_state_startMin = 0;
        sysml_state_endMin = 0;
        sysml_state_occupied = false;
        sysml_state_overtimeActive = false;
    }
    sysml_lazy_1 = io_8_valid;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_8 == 1;
    }
    if (sysml_lazy_1) {
        sysml_state_startMin = 10;
        sysml_state_endMin = 20;
    }
    sysml_lazy_2 = io_8_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_8 == 2;
    }
    if (sysml_lazy_2) {
        sysml_state_startMin = 30;
        sysml_state_endMin = 60;
    }
    sysml_lazy_3 = sysml_state_startMin == 0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_endMin == 0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = false;
    }
    if (sysml_lazy_4) {
    }
    if (io_4) {
        sysml_state_occupied = true;
    }
    sysml_lazy_5 = !io_5;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_occupied;
    }
    if (sysml_lazy_5) {
        sysml_state_occupied = false;
    }
    sysml_lazy_6 = io_0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_prevPressed;
    }
    if (sysml_lazy_6) {
        sysml_state_occupied = false;
        sysml_state_overtimeActive = false;
    }
    sysml_lazy_7 = sysml_state_occupied;
    if (sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_startMin > 0;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_endMin > sysml_state_startMin;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_startMin >= sysml_state_endMin;
    }
    if (sysml_lazy_9) {
        sysml_state_overtimeActive = true;
    }
    else {
        sysml_state_overtimeActive = false;
    }
    sysml_state_prevPressed = io_0;
    bool sysml_value_0 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    int sysml_value_1 = 0;
    if (sysml_state_overtimeActive) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_1 = sysml_conditional_11;
    int sysml_value_2 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    sysml_lazy_13 = !sysml_state_occupied;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_overtimeActive;
    }
    if (sysml_lazy_13) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_3 = sysml_conditional_14;
    bool sysml_value_4 = 0;
    if (sysml_state_occupied) {
        sysml_conditional_15 = true;
    }
    else {
        sysml_conditional_15 = false;
    }
    sysml_value_4 = sysml_conditional_15;
    bool sysml_value_5 = 0;
    sysml_lazy_16 = io_8_valid;
    if (sysml_lazy_16) {
        sysml_lazy_16 = sysml_state_endMin > 0;
    }
    if (sysml_lazy_16) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_5 = sysml_conditional_17;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_startMin;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_endMin;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
    *serial_0_trigger = sysml_value_5;
    *serial_0_value_0 = sysml_value_6;
    *serial_0_value_1 = sysml_value_7;
}