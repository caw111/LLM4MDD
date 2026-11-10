#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousD2Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousD3Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousD4Pressed = false;
/* Global Variable define of  */
bool sysml_state_previousD7Pressed = false;
/* Global Variable define of  */
int sysml_state_selectedItemIndex = 0;
/* Global Variable define of  */
int sysml_state_inventory0 = 5;
/* Global Variable define of  */
int sysml_state_inventory1 = 5;
/* Global Variable define of  */
int sysml_state_inventory2 = 5;
/* Global Variable define of  */
int sysml_state_inventory3 = 5;
/* Global Variable define of  */
bool sysml_state_inAdminMode = false;
/* Global Variable define of  */
bool sysml_state_pendingDispenseTrigger = false;
/* Global Variable define of  */
bool sysml_state_pendingErrorTrigger = false;
/* Global Variable define of  */
int sysml_state_pendingErrorCode = 1;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousD2Pressed = false;
    sysml_state_previousD3Pressed = false;
    sysml_state_previousD4Pressed = false;
    sysml_state_previousD7Pressed = false;
    sysml_state_selectedItemIndex = 0;
    sysml_state_inventory0 = 5;
    sysml_state_inventory1 = 5;
    sysml_state_inventory2 = 5;
    sysml_state_inventory3 = 5;
    sysml_state_inAdminMode = false;
    sysml_state_pendingDispenseTrigger = false;
    sysml_state_pendingErrorTrigger = false;
    sysml_state_pendingErrorCode = 1;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, int* serial_1_value_0) {
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
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    sysml_state_pendingDispenseTrigger = false;
    sysml_state_pendingErrorTrigger = false;
    if (io_3) {
        sysml_lazy_0 = io_3;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_previousD7Pressed;
        }
        if (sysml_lazy_0) {
            sysml_state_inAdminMode = true;
        }
    }
    sysml_lazy_1 = !io_3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_previousD7Pressed;
    }
    if (sysml_lazy_1) {
        sysml_state_inAdminMode = false;
    }
    sysml_lazy_2 = io_0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_previousD2Pressed;
    }
    if (sysml_lazy_2) {
        sysml_state_selectedItemIndex = 0;
    }
    sysml_lazy_3 = io_1;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousD3Pressed;
    }
    if (sysml_lazy_3) {
        sysml_state_selectedItemIndex = 1;
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousD4Pressed;
    }
    if (sysml_lazy_4) {
        sysml_state_selectedItemIndex = 2;
    }
    sysml_lazy_5 = !io_3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_previousD7Pressed;
    }
    if (sysml_lazy_5) {
        if (sysml_state_inAdminMode) {
            sysml_state_pendingErrorCode = 0;
        }
        else {
            sysml_lazy_6 = sysml_state_selectedItemIndex == 0;
            if (sysml_lazy_6) {
                sysml_lazy_6 = sysml_state_inventory0 > 0;
            }
            sysml_lazy_7 = sysml_lazy_6;
            if (!sysml_lazy_7) {
                sysml_lazy_8 = sysml_state_selectedItemIndex == 1;
                if (sysml_lazy_8) {
                    sysml_lazy_8 = sysml_state_inventory1 > 0;
                }
                sysml_lazy_7 = sysml_lazy_8;
            }
            sysml_lazy_9 = sysml_lazy_7;
            if (!sysml_lazy_9) {
                sysml_lazy_10 = sysml_state_selectedItemIndex == 2;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = sysml_state_inventory2 > 0;
                }
                sysml_lazy_9 = sysml_lazy_10;
            }
            if (sysml_lazy_9) {
                if ((sysml_state_selectedItemIndex == 0)) {
                    sysml_state_inventory0 = (sysml_state_inventory0 - 1);
                }
                sysml_lazy_11 = sysml_state_selectedItemIndex == 1;
                if (sysml_lazy_11) {
                    sysml_lazy_11 = sysml_state_inventory1 > 0;
                }
                if (sysml_lazy_11) {
                    sysml_state_inventory1 = (sysml_state_inventory1 - 1);
                }
                sysml_lazy_12 = sysml_state_selectedItemIndex == 2;
                if (sysml_lazy_12) {
                    sysml_lazy_12 = sysml_state_inventory2 > 0;
                }
                if (sysml_lazy_12) {
                    sysml_state_inventory2 = (sysml_state_inventory2 - 1);
                }
                sysml_state_pendingDispenseTrigger = true;
            }
            else {
                sysml_state_pendingErrorTrigger = true;
                if ((sysml_state_selectedItemIndex == 0)) {
                    sysml_state_pendingErrorCode = 10;
                }
                else {
                    if ((sysml_state_selectedItemIndex == 1)) {
                        sysml_state_pendingErrorCode = 11;
                    }
                    else {
                        sysml_state_pendingErrorCode = 12;
                    }
                }
            }
        }
    }
    sysml_state_previousD2Pressed = io_0;
    sysml_state_previousD3Pressed = io_1;
    sysml_state_previousD4Pressed = io_2;
    sysml_state_previousD7Pressed = io_3;
    bool sysml_value_0 = 0;
    sysml_lazy_13 = !sysml_state_inAdminMode;
    if (sysml_lazy_13) {
        sysml_lazy_13 = sysml_state_pendingDispenseTrigger;
    }
    sysml_value_0 = sysml_lazy_13;
    int sysml_value_1 = 0;
    if (sysml_state_pendingDispenseTrigger) {
        sysml_conditional_14 = 2000;
    }
    else {
        if (sysml_state_pendingErrorTrigger) {
            sysml_conditional_15 = 1000;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_1 = sysml_conditional_14;
    int sysml_value_2 = 0;
    if (sysml_state_pendingErrorTrigger) {
        sysml_conditional_16 = sysml_state_pendingErrorCode;
    }
    else {
        if (sysml_state_pendingDispenseTrigger) {
            if (sysml_state_selectedItemIndex == 0) {
                sysml_conditional_18 = 101;
            }
            else {
                if (sysml_state_selectedItemIndex == 1) {
                    sysml_conditional_19 = 201;
                }
                else {
                    sysml_conditional_19 = 301;
                }
                sysml_conditional_18 = sysml_conditional_19;
            }
            sysml_conditional_17 = sysml_conditional_18;
        }
        else {
            if (sysml_state_inAdminMode) {
                sysml_conditional_20 = 900;
            }
            else {
                if (sysml_state_selectedItemIndex == 0) {
                    sysml_conditional_21 = 100;
                }
                else {
                    if (sysml_state_selectedItemIndex == 1) {
                        sysml_conditional_22 = 200;
                    }
                    else {
                        sysml_conditional_22 = 300;
                    }
                    sysml_conditional_21 = sysml_conditional_22;
                }
                sysml_conditional_20 = sysml_conditional_21;
            }
            sysml_conditional_17 = sysml_conditional_20;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_16;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_pendingDispenseTrigger;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_selectedItemIndex;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_pendingErrorTrigger;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_pendingErrorCode;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
}