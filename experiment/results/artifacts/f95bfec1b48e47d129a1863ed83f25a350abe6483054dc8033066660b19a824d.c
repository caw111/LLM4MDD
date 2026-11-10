#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousPressed1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressed3 = false;
/* Global Variable define of  */
double sysml_state_holdPressedSeconds = 0.0;
/* Global Variable define of  */
int sysml_state_activeState = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousPressed1 = false;
    sysml_state_previousPressed2 = false;
    sysml_state_previousPressed3 = false;
    sysml_state_holdPressedSeconds = 0.0;
    sysml_state_activeState = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    double sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    double sysml_conditional_11 = 0;
    double sysml_conditional_12 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed3;
    }
    if (sysml_lazy_0) {
        sysml_state_activeState = 3;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_1 = io_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousPressed1;
    }
    if (sysml_lazy_1) {
        sysml_state_activeState = 1;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousPressed2;
    }
    if (sysml_lazy_2) {
        sysml_state_activeState = 2;
        sysml_state_holdPressedSeconds = 0.0;
    }
    sysml_lazy_3 = io_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = io_0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = io_1;
    }
    if (!sysml_lazy_4) {
        sysml_state_holdPressedSeconds = 2.0;
    }
    sysml_lazy_5 = io_2;
    if (!sysml_lazy_5) {
        sysml_lazy_5 = io_0;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = io_1;
    }
    if (sysml_lazy_6) {
        sysml_state_holdPressedSeconds = 2.0;
    }
    sysml_state_previousPressed1 = io_0;
    sysml_state_previousPressed2 = io_1;
    sysml_state_previousPressed3 = io_2;
    double sysml_value_0 = 0;
    if (sysml_state_activeState == 1) {
        sysml_conditional_7 = 1.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_0 = sysml_conditional_7;
    double sysml_value_1 = 0;
    if (sysml_state_activeState == 2) {
        sysml_conditional_8 = 1.0;
    }
    else {
        sysml_conditional_8 = 0.0;
    }
    sysml_value_1 = sysml_conditional_8;
    double sysml_value_2 = 0;
    if (sysml_state_activeState == 3) {
        sysml_conditional_9 = 1.0;
    }
    else {
        sysml_conditional_9 = 0.0;
    }
    sysml_value_2 = sysml_conditional_9;
    double sysml_value_3 = 0;
    sysml_lazy_10 = sysml_state_activeState == 1;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_activeState == 2;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = 90.0;
    }
    else {
        sysml_conditional_11 = 0.0;
    }
    sysml_value_3 = sysml_conditional_11;
    double sysml_value_4 = 0;
    if (sysml_state_activeState == 2) {
        sysml_conditional_12 = 90.0;
    }
    else {
        sysml_conditional_12 = 0.0;
    }
    sysml_value_4 = sysml_conditional_12;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6 = sysml_value_3;
    *io_7 = sysml_value_4;
}