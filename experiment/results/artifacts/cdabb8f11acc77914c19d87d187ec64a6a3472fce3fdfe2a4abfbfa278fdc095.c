#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn1 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn2 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayNumber = 0;
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_stableBtn1 = false;
    sysml_state_stableBtn2 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_conditional_11 = 0;
    sysml_lazy_0 = io_0;
    if (!sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        sysml_state_stableBtn1 = io_0;
    }
    sysml_lazy_1 = io_1;
    if (!sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_1) {
        sysml_state_stableBtn2 = io_1;
    }
    sysml_lazy_2 = sysml_state_stableBtn1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_2) {
        if (sysml_state_displayNumber == 99) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_3;
    }
    sysml_lazy_4 = sysml_state_stableBtn2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_displayNumber == 0) {
            sysml_conditional_5 = 99;
        }
        else {
            sysml_conditional_5 = sysml_state_displayNumber - 1;
        }
        sysml_state_displayNumber = sysml_conditional_5;
    }
    sysml_state_previousBtn1 = sysml_state_stableBtn1;
    sysml_state_previousBtn2 = sysml_state_stableBtn2;
    bool sysml_value_0 = 0;
    sysml_lazy_6 = sysml_state_displayNumber >= 0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_displayNumber <= 33;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_0 = sysml_conditional_7;
    bool sysml_value_1 = 0;
    sysml_lazy_8 = sysml_state_displayNumber >= 34;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_displayNumber <= 66;
    }
    if (sysml_lazy_8) {
        sysml_conditional_9 = true;
    }
    else {
        sysml_conditional_9 = false;
    }
    sysml_value_1 = sysml_conditional_9;
    bool sysml_value_2 = 0;
    sysml_lazy_10 = sysml_state_displayNumber >= 67;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_displayNumber <= 99;
    }
    if (sysml_lazy_10) {
        sysml_conditional_11 = true;
    }
    else {
        sysml_conditional_11 = false;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    sysml_value_3 = sysml_state_displayNumber;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
}