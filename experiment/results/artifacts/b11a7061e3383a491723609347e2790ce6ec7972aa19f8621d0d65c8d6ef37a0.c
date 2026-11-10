#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_menuIndex = 0;
/* Global Variable define of  */
int sysml_state_confirmedIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousMenuPressed = true;
/* Global Variable define of  */
bool sysml_state_previousConfirmPressed = true;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_menuIndex = 0;
    sysml_state_confirmedIndex = 0;
    sysml_state_previousMenuPressed = true;
    sysml_state_previousConfirmPressed = true;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, bool io_4, double io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousMenuPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_menuIndex == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_menuIndex + 1;
        }
        sysml_state_menuIndex = sysml_conditional_1;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_2) {
        sysml_state_confirmedIndex = sysml_state_menuIndex;
    }
    sysml_state_previousMenuPressed = io_0;
    sysml_state_previousConfirmPressed = io_1;
    int sysml_value_0 = 0;
    if (sysml_state_confirmedIndex == 0) {
        if (io_4) {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_5 = 1;
            }
            else {
                sysml_conditional_5 = 0;
            }
            sysml_conditional_4 = ((int)io_2) - sysml_conditional_5;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    else {
        if (sysml_state_confirmedIndex == 1) {
            if (io_4) {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int)io_3) - sysml_conditional_8;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        else {
            if (io_5 < ((double)((int)io_5))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_conditional_6 = ((int)io_5) - sysml_conditional_9;
        }
        sysml_conditional_3 = sysml_conditional_6;
    }
    sysml_value_0 = sysml_conditional_3;
    *io_6 = sysml_value_0;
}