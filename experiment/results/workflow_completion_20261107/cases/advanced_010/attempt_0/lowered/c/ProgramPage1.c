#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stockOne = 2;
/* Global Variable define of  */
int sysml_state_stockTwo = 2;
/* Global Variable define of  */
int sysml_state_stockThree = 2;
/* Global Variable define of  */
int sysml_state_priceOne = 1;
/* Global Variable define of  */
int sysml_state_priceTwo = 2;
/* Global Variable define of  */
int sysml_state_priceThree = 3;
/* Global Variable define of  */
int sysml_state_selectedItem = 0;
/* Global Variable define of  */
int sysml_state_displayPhase = 0;
/* Global Variable define of  */
bool sysml_state_displayError = false;
/* Global Variable define of  */
bool sysml_state_adminMode = false;
/* Global Variable define of  */
bool sysml_state_previousOne = false;
/* Global Variable define of  */
bool sysml_state_previousTwo = false;
/* Global Variable define of  */
bool sysml_state_previousThree = false;
/* Global Variable define of  */
bool sysml_state_previousConfirm = false;
/* Global Variable define of  */
int sysml_state_holdDuration = 0;
/* Global Variable define of  */
bool sysml_state_holdHandled = false;
/* Global Variable define of  */
int sysml_state_beepRemaining = 0;
/* Global Variable define of  */
int sysml_state_beepFrequency = 0;
/* Global Variable define of  */
int sysml_state_saleItem = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stockOne = 2;
    sysml_state_stockTwo = 2;
    sysml_state_stockThree = 2;
    sysml_state_priceOne = 1;
    sysml_state_priceTwo = 2;
    sysml_state_priceThree = 3;
    sysml_state_selectedItem = 0;
    sysml_state_displayPhase = 0;
    sysml_state_displayError = false;
    sysml_state_adminMode = false;
    sysml_state_previousOne = false;
    sysml_state_previousTwo = false;
    sysml_state_previousThree = false;
    sysml_state_previousConfirm = false;
    sysml_state_holdDuration = 0;
    sysml_state_holdHandled = false;
    sysml_state_beepRemaining = 0;
    sysml_state_beepFrequency = 0;
    sysml_state_saleItem = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int io_7, bool io_7_valid, int io_11, bool* io_4, int* io_5, int* io_6, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_arithmetic_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    int sysml_conditional_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    int sysml_conditional_32 = 0;
    int sysml_conditional_33 = 0;
    int sysml_conditional_34 = 0;
    int sysml_arithmetic_35 = 0;
    int sysml_conditional_36 = 0;
    int sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    int sysml_conditional_39 = 0;
    int sysml_conditional_40 = 0;
    int sysml_conditional_41 = 0;
    int sysml_conditional_42 = 0;
    int sysml_conditional_43 = 0;
    int sysml_conditional_44 = 0;
    sysml_state_saleItem = 0;
    if ((sysml_state_beepRemaining > 0)) {
        if (io_11 >= sysml_state_beepRemaining) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_beepRemaining - io_11;
        }
        sysml_state_beepRemaining = sysml_conditional_0;
    }
    if ((sysml_state_selectedItem > 0)) {
        if ((((double)io_11) / 2000) < ((double)((int)(((double)io_11) / 2000)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_arithmetic_2 = ((int)(((double)io_11) / 2000)) - sysml_conditional_1;
        sysml_state_displayPhase = sysml_state_displayPhase + (io_11 - (2000 * sysml_arithmetic_2));
        if ((sysml_state_displayPhase >= 2000)) {
            sysml_state_displayPhase = (sysml_state_displayPhase - 2000);
        }
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousOne;
    }
    if (sysml_lazy_3) {
        sysml_state_selectedItem = 1;
        sysml_state_displayPhase = 0;
        sysml_state_displayError = false;
    }
    else {
        sysml_lazy_4 = io_1;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_previousTwo;
        }
        if (sysml_lazy_4) {
            sysml_state_selectedItem = 2;
            sysml_state_displayPhase = 0;
            sysml_state_displayError = false;
        }
        else {
            sysml_lazy_5 = io_2;
            if (sysml_lazy_5) {
                sysml_lazy_5 = !sysml_state_previousThree;
            }
            if (sysml_lazy_5) {
                sysml_state_selectedItem = 3;
                sysml_state_displayPhase = 0;
                sysml_state_displayError = false;
            }
        }
    }
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_previousConfirm;
    }
    if (sysml_lazy_6) {
        sysml_state_holdDuration = 0;
        sysml_state_holdHandled = false;
    }
    sysml_lazy_7 = sysml_state_previousConfirm;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_holdHandled;
    }
    if (sysml_lazy_7) {
        if (io_11 >= (2000 - sysml_state_holdDuration)) {
            sysml_conditional_8 = 2000;
        }
        else {
            sysml_conditional_8 = sysml_state_holdDuration + io_11;
        }
        sysml_state_holdDuration = sysml_conditional_8;
        if ((sysml_state_holdDuration >= 2000)) {
            sysml_state_adminMode = (!sysml_state_adminMode);
            sysml_state_holdHandled = true;
        }
    }
    sysml_lazy_9 = io_7_valid;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_adminMode;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_7 >= 10000;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_7 <= 39999;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_lazy_12 = (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_13) * 10000)) >= 0;
    }
    sysml_lazy_14 = sysml_lazy_12;
    if (sysml_lazy_14) {
        if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_lazy_14 = (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_15) * 10000)) <= 9999;
    }
    if (sysml_lazy_14) {
        if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        if ((((int)(((double)io_7) / 10000)) - sysml_conditional_16) == 1) {
            if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            if ((sysml_state_stockOne + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_17) * 10000))) > 9999) {
                sysml_conditional_18 = 9999;
            }
            else {
                if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                    sysml_conditional_19 = 1;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_18 = sysml_state_stockOne + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_19) * 10000));
            }
            sysml_state_stockOne = sysml_conditional_18;
        }
        else {
            if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                sysml_conditional_20 = 1;
            }
            else {
                sysml_conditional_20 = 0;
            }
            if ((((int)(((double)io_7) / 10000)) - sysml_conditional_20) == 2) {
                if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                    sysml_conditional_21 = 1;
                }
                else {
                    sysml_conditional_21 = 0;
                }
                if ((sysml_state_stockTwo + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_21) * 10000))) > 9999) {
                    sysml_conditional_22 = 9999;
                }
                else {
                    if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                        sysml_conditional_23 = 1;
                    }
                    else {
                        sysml_conditional_23 = 0;
                    }
                    sysml_conditional_22 = sysml_state_stockTwo + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_23) * 10000));
                }
                sysml_state_stockTwo = sysml_conditional_22;
            }
            else {
                if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                    sysml_conditional_24 = 1;
                }
                else {
                    sysml_conditional_24 = 0;
                }
                if ((((int)(((double)io_7) / 10000)) - sysml_conditional_24) == 3) {
                    if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                        sysml_conditional_25 = 1;
                    }
                    else {
                        sysml_conditional_25 = 0;
                    }
                    if ((sysml_state_stockThree + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_25) * 10000))) > 9999) {
                        sysml_conditional_26 = 9999;
                    }
                    else {
                        if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
                            sysml_conditional_27 = 1;
                        }
                        else {
                            sysml_conditional_27 = 0;
                        }
                        sysml_conditional_26 = sysml_state_stockThree + (io_7 - ((((int)(((double)io_7) / 10000)) - sysml_conditional_27) * 10000));
                    }
                    sysml_state_stockThree = sysml_conditional_26;
                }
            }
        }
    }
    sysml_lazy_28 = sysml_state_previousConfirm;
    if (sysml_lazy_28) {
        sysml_lazy_28 = !io_3;
    }
    sysml_lazy_29 = sysml_lazy_28;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_holdHandled;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (sysml_lazy_30) {
        sysml_lazy_30 = !sysml_state_adminMode;
    }
    sysml_lazy_31 = sysml_lazy_30;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_selectedItem > 0;
    }
    if (sysml_lazy_31) {
        if (sysml_state_selectedItem == 1) {
            sysml_conditional_32 = sysml_state_stockOne;
        }
        else {
            if (sysml_state_selectedItem == 2) {
                sysml_conditional_33 = sysml_state_stockTwo;
            }
            else {
                sysml_conditional_33 = sysml_state_stockThree;
            }
            sysml_conditional_32 = sysml_conditional_33;
        }
        if (sysml_conditional_32 > 0) {
            sysml_state_saleItem = sysml_state_selectedItem;
            if ((sysml_state_selectedItem == 1)) {
                sysml_state_stockOne = (sysml_state_stockOne - 1);
            }
            else {
                if ((sysml_state_selectedItem == 2)) {
                    sysml_state_stockTwo = (sysml_state_stockTwo - 1);
                }
                else {
                    sysml_state_stockThree = (sysml_state_stockThree - 1);
                }
            }
            sysml_state_displayError = false;
            sysml_state_beepFrequency = 2000;
            sysml_state_beepRemaining = 100;
        }
        else {
            sysml_state_displayError = true;
            sysml_state_beepFrequency = 1000;
            sysml_state_beepRemaining = 500;
        }
    }
    sysml_state_previousOne = io_0;
    sysml_state_previousTwo = io_1;
    sysml_state_previousThree = io_2;
    sysml_state_previousConfirm = io_3;
    int sysml_value_0 = 0;
    if ((((double)io_11) / 2000) < ((double)((int)(((double)io_11) / 2000)))) {
        sysml_conditional_34 = 1;
    }
    else {
        sysml_conditional_34 = 0;
    }
    sysml_arithmetic_35 = ((int)(((double)io_11) / 2000)) - sysml_conditional_34;
    sysml_value_0 = io_11 - (2000 * sysml_arithmetic_35);
    int sysml_value_1 = 0;
    if ((((double)io_7) / 10000) < ((double)((int)(((double)io_7) / 10000)))) {
        sysml_conditional_36 = 1;
    }
    else {
        sysml_conditional_36 = 0;
    }
    sysml_value_1 = ((int)(((double)io_7) / 10000)) - sysml_conditional_36;
    int sysml_value_2 = 0;
    if (sysml_state_selectedItem == 1) {
        sysml_conditional_37 = sysml_state_stockOne;
    }
    else {
        if (sysml_state_selectedItem == 2) {
            sysml_conditional_38 = sysml_state_stockTwo;
        }
        else {
            sysml_conditional_38 = sysml_state_stockThree;
        }
        sysml_conditional_37 = sysml_conditional_38;
    }
    sysml_value_2 = sysml_conditional_37;
    int sysml_value_3 = 0;
    if (sysml_state_selectedItem == 1) {
        sysml_conditional_39 = sysml_state_priceOne;
    }
    else {
        if (sysml_state_selectedItem == 2) {
            sysml_conditional_40 = sysml_state_priceTwo;
        }
        else {
            sysml_conditional_40 = sysml_state_priceThree;
        }
        sysml_conditional_39 = sysml_conditional_40;
    }
    sysml_value_3 = sysml_conditional_39;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_adminMode;
    int sysml_value_5 = 0;
    if (sysml_state_beepRemaining > 0) {
        sysml_conditional_41 = sysml_state_beepFrequency;
    }
    else {
        sysml_conditional_41 = 0;
    }
    sysml_value_5 = sysml_conditional_41;
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_saleItem == 1);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_saleItem == 2);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_saleItem == 3);
    int sysml_value_9 = 0;
    sysml_value_9 = (io_7 - (sysml_value_1 * 10000));
    int sysml_value_10 = 0;
    if (sysml_state_displayError) {
        sysml_conditional_42 = -1;
    }
    else {
        if (sysml_state_selectedItem == 0) {
            sysml_conditional_43 = 10000;
        }
        else {
            if (sysml_state_displayPhase < 1000) {
                sysml_conditional_44 = sysml_state_selectedItem;
            }
            else {
                sysml_conditional_44 = sysml_value_3;
            }
            sysml_conditional_43 = sysml_conditional_44;
        }
        sysml_conditional_42 = sysml_conditional_43;
    }
    sysml_value_10 = sysml_conditional_42;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_10;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
}