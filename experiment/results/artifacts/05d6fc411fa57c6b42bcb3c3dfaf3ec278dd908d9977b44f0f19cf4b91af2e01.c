#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_menuIndex = 0;
/* Global Variable define of  */
bool sysml_state_previousD2 = false;
/* Global Variable define of  */
int sysml_state_selectedIndex = 0;
/* Global Variable define of  */
bool sysml_state_lockedByConfirm = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_menuIndex = 0;
    sysml_state_previousD2 = false;
    sysml_state_selectedIndex = 0;
    sysml_state_lockedByConfirm = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, bool io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousD2;
    }
    if (sysml_lazy_0) {
        if (sysml_state_menuIndex == 2) {
            sysml_conditional_1 = 0;
        }
        else {
            if (sysml_state_menuIndex == 0) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 2;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_state_menuIndex = sysml_conditional_1;
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_lockedByConfirm;
    }
    if (sysml_lazy_3) {
        sysml_state_selectedIndex = sysml_state_menuIndex;
        sysml_state_lockedByConfirm = true;
    }
    sysml_lazy_4 = !io_1;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_lockedByConfirm;
    }
    if (sysml_lazy_4) {
        sysml_state_lockedByConfirm = false;
    }
    sysml_state_previousD2 = io_0;
    int sysml_value_0 = 0;
    if (sysml_state_selectedIndex == 0) {
        if (io_5) {
            if (io_3 <= 0.0) {
                sysml_conditional_7 = 0;
            }
            else {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_8 = 1;
                }
                else {
                    sysml_conditional_8 = 0;
                }
                sysml_conditional_7 = ((int)io_3) - sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        else {
            if (io_3 <= 0.0) {
                sysml_conditional_9 = 0;
            }
            else {
                if (io_3 < ((double)((int)io_3))) {
                    sysml_conditional_10 = 1;
                }
                else {
                    sysml_conditional_10 = 0;
                }
                sysml_conditional_9 = ((int)io_3) - sysml_conditional_10;
            }
            sysml_conditional_6 = sysml_conditional_9;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    else {
        if (sysml_state_selectedIndex == 1) {
            if (io_5) {
                if (io_4 <= 0.0) {
                    sysml_conditional_13 = 0;
                }
                else {
                    if (io_4 < ((double)((int)io_4))) {
                        sysml_conditional_14 = 1;
                    }
                    else {
                        sysml_conditional_14 = 0;
                    }
                    sysml_conditional_13 = ((int)io_4) - sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            else {
                if (io_4 <= 0.0) {
                    sysml_conditional_15 = 0;
                }
                else {
                    if (io_4 < ((double)((int)io_4))) {
                        sysml_conditional_16 = 1;
                    }
                    else {
                        sysml_conditional_16 = 0;
                    }
                    sysml_conditional_15 = ((int)io_4) - sysml_conditional_16;
                }
                sysml_conditional_12 = sysml_conditional_15;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        else {
            if (io_2 <= 0.0) {
                sysml_conditional_17 = 0;
            }
            else {
                if (io_2 < ((double)((int)io_2))) {
                    sysml_conditional_18 = 1;
                }
                else {
                    sysml_conditional_18 = 0;
                }
                sysml_conditional_17 = ((int)io_2) - sysml_conditional_18;
            }
            sysml_conditional_11 = sysml_conditional_17;
        }
        sysml_conditional_5 = sysml_conditional_11;
    }
    sysml_value_0 = sysml_conditional_5;
    *io_6 = sysml_value_0;
}