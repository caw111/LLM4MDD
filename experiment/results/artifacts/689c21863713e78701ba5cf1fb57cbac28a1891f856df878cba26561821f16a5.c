#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousConfirmPressed = false;
/* Global Variable define of  */
bool sysml_state_adminMode = false;
/* Global Variable define of  */
int sysml_state_selectedItemIndex = 1;
/* Global Variable define of  */
int sysml_state_stockItem1 = 5;
/* Global Variable define of  */
int sysml_state_stockItem2 = 5;
/* Global Variable define of  */
int sysml_state_stockItem3 = 5;
/* Global Variable define of  */
bool sysml_state_beepSoundOn = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousConfirmPressed = false;
    sysml_state_adminMode = false;
    sysml_state_selectedItemIndex = 1;
    sysml_state_stockItem1 = 5;
    sysml_state_stockItem2 = 5;
    sysml_state_stockItem3 = 5;
    sysml_state_beepSoundOn = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_conditional_8 = 0;
    bool sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    bool sysml_lazy_21 = 0;
    bool sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    bool sysml_lazy_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    int sysml_conditional_27 = 0;
    int sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_conditional_30 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_adminMode;
    }
    if (sysml_lazy_0) {
        sysml_state_selectedItemIndex = 1;
    }
    sysml_lazy_1 = io_1;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_adminMode;
    }
    if (sysml_lazy_1) {
        sysml_state_selectedItemIndex = 2;
    }
    sysml_lazy_2 = io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_adminMode;
    }
    if (sysml_lazy_2) {
        sysml_state_selectedItemIndex = 3;
    }
    if (io_3) {
        sysml_state_adminMode = true;
    }
    sysml_lazy_3 = !io_3;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_adminMode;
    }
    if (sysml_lazy_3) {
        sysml_state_adminMode = false;
    }
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_4) {
        if (sysml_state_adminMode) {
            sysml_state_beepSoundOn = true;
        }
        else {
            sysml_state_beepSoundOn = true;
        }
    }
    sysml_lazy_5 = io_3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousConfirmPressed;
    }
    if (!sysml_lazy_5) {
        sysml_state_beepSoundOn = false;
    }
    sysml_lazy_6 = !sysml_state_adminMode;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_3;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_7) {
        if ((sysml_state_selectedItemIndex == 1)) {
            if ((sysml_state_stockItem1 > 0)) {
                sysml_state_stockItem1 = (sysml_state_stockItem1 - 1);
            }
        }
        if ((sysml_state_selectedItemIndex == 2)) {
            if ((sysml_state_stockItem2 > 0)) {
                sysml_state_stockItem2 = (sysml_state_stockItem2 - 1);
            }
        }
        if ((sysml_state_selectedItemIndex == 3)) {
            if ((sysml_state_stockItem3 > 0)) {
                sysml_state_stockItem3 = (sysml_state_stockItem3 - 1);
            }
        }
    }
    sysml_state_previousConfirmPressed = io_3;
    bool sysml_value_0 = 0;
    if (sysml_state_adminMode) {
        sysml_conditional_8 = true;
    }
    else {
        if (io_3) {
            sysml_conditional_9 = true;
        }
        else {
            sysml_conditional_9 = false;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_0 = sysml_conditional_8;
    int sysml_value_1 = 0;
    if (sysml_state_beepSoundOn) {
        sysml_conditional_10 = 1000;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_1 = sysml_conditional_10;
    int sysml_value_2 = 0;
    if (sysml_state_adminMode) {
        sysml_conditional_11 = 0;
    }
    else {
        sysml_lazy_12 = !io_3;
        if (sysml_lazy_12) {
            sysml_lazy_12 = true;
        }
        if (sysml_lazy_12) {
            sysml_conditional_13 = sysml_state_selectedItemIndex;
        }
        else {
            sysml_lazy_14 = io_3;
            if (sysml_lazy_14) {
                sysml_lazy_14 = !sysml_state_previousConfirmPressed;
            }
            if (sysml_lazy_14) {
                if (sysml_state_selectedItemIndex == 1) {
                    if (sysml_state_stockItem1 > 0) {
                        sysml_conditional_17 = 1;
                    }
                    else {
                        sysml_conditional_17 = 901;
                    }
                    sysml_conditional_16 = sysml_conditional_17;
                }
                else {
                    if (sysml_state_selectedItemIndex == 2) {
                        if (sysml_state_stockItem2 > 0) {
                            sysml_conditional_19 = 2;
                        }
                        else {
                            sysml_conditional_19 = 902;
                        }
                        sysml_conditional_18 = sysml_conditional_19;
                    }
                    else {
                        if (sysml_state_stockItem3 > 0) {
                            sysml_conditional_20 = 3;
                        }
                        else {
                            sysml_conditional_20 = 903;
                        }
                        sysml_conditional_18 = sysml_conditional_20;
                    }
                    sysml_conditional_16 = sysml_conditional_18;
                }
                sysml_conditional_15 = sysml_conditional_16;
            }
            else {
                sysml_conditional_15 = sysml_state_selectedItemIndex;
            }
            sysml_conditional_13 = sysml_conditional_15;
        }
        sysml_conditional_11 = sysml_conditional_13;
    }
    sysml_value_2 = sysml_conditional_11;
    bool sysml_value_3 = 0;
    sysml_lazy_21 = io_3;
    if (sysml_lazy_21) {
        sysml_lazy_21 = !sysml_state_previousConfirmPressed;
    }
    if (sysml_lazy_21) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_3 = sysml_conditional_22;
    int sysml_value_4 = 0;
    if (sysml_state_adminMode) {
        sysml_conditional_23 = 700;
    }
    else {
        sysml_lazy_24 = io_3;
        if (sysml_lazy_24) {
            sysml_lazy_24 = !sysml_state_previousConfirmPressed;
        }
        if (sysml_lazy_24) {
            if (sysml_state_selectedItemIndex == 1) {
                if (sysml_state_stockItem1 > 0) {
                    sysml_conditional_27 = 1001;
                }
                else {
                    sysml_conditional_27 = 901;
                }
                sysml_conditional_26 = sysml_conditional_27;
            }
            else {
                if (sysml_state_selectedItemIndex == 2) {
                    if (sysml_state_stockItem2 > 0) {
                        sysml_conditional_29 = 1002;
                    }
                    else {
                        sysml_conditional_29 = 902;
                    }
                    sysml_conditional_28 = sysml_conditional_29;
                }
                else {
                    if (sysml_state_stockItem3 > 0) {
                        sysml_conditional_30 = 1003;
                    }
                    else {
                        sysml_conditional_30 = 903;
                    }
                    sysml_conditional_28 = sysml_conditional_30;
                }
                sysml_conditional_26 = sysml_conditional_28;
            }
            sysml_conditional_25 = sysml_conditional_26;
        }
        else {
            sysml_conditional_25 = 0;
        }
        sysml_conditional_23 = sysml_conditional_25;
    }
    sysml_value_4 = sysml_conditional_23;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
}