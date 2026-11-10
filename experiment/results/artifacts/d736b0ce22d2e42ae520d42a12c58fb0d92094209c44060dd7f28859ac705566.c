#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_vacantTimeOne = 0;
/* Global Variable define of  */
int sysml_state_vacantTimeTwo = 0;
/* Global Variable define of  */
int sysml_state_vacantTimeThree = 0;
/* Global Variable define of  */
bool sysml_state_previousOccupiedOne = false;
/* Global Variable define of  */
bool sysml_state_previousOccupiedTwo = false;
/* Global Variable define of  */
bool sysml_state_previousOccupiedThree = false;
/* Global Variable define of  */
bool sysml_state_reportedOne = false;
/* Global Variable define of  */
bool sysml_state_reportedTwo = false;
/* Global Variable define of  */
bool sysml_state_reportedThree = false;
/* Global Variable define of  */
bool sysml_state_restockReport = false;
/* Global Variable define of  */
bool sysml_state_recordsLocked = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_vacantTimeOne = 0;
    sysml_state_vacantTimeTwo = 0;
    sysml_state_vacantTimeThree = 0;
    sysml_state_previousOccupiedOne = false;
    sysml_state_previousOccupiedTwo = false;
    sysml_state_previousOccupiedThree = false;
    sysml_state_reportedOne = false;
    sysml_state_reportedTwo = false;
    sysml_state_reportedThree = false;
    sysml_state_restockReport = false;
    sysml_state_recordsLocked = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, int io_7, bool io_7_valid, int* io_3, int* io_4, bool* io_5, bool* io_8, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    bool sysml_lazy_23 = 0;
    int sysml_conditional_24 = 0;
    bool sysml_lazy_25 = 0;
    bool sysml_lazy_26 = 0;
    bool sysml_lazy_27 = 0;
    sysml_state_restockReport = false;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2;
    }
    sysml_state_recordsLocked = sysml_lazy_1;
    sysml_lazy_2 = io_7_valid;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_7 == 2;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (!sysml_lazy_3) {
        sysml_lazy_3 = io_0;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (!sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_previousOccupiedOne;
    }
    if (sysml_lazy_4) {
        sysml_state_vacantTimeOne = 0;
        sysml_state_reportedOne = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeOne)) {
            sysml_conditional_5 = 60000 + 1;
        }
        else {
            sysml_conditional_5 = sysml_state_vacantTimeOne + io_6;
        }
        sysml_state_vacantTimeOne = sysml_conditional_5;
    }
    sysml_lazy_6 = io_7_valid;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_7 == 2;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = io_1;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (!sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_previousOccupiedTwo;
    }
    if (sysml_lazy_8) {
        sysml_state_vacantTimeTwo = 0;
        sysml_state_reportedTwo = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeTwo)) {
            sysml_conditional_9 = 60000 + 1;
        }
        else {
            sysml_conditional_9 = sysml_state_vacantTimeTwo + io_6;
        }
        sysml_state_vacantTimeTwo = sysml_conditional_9;
    }
    sysml_lazy_10 = io_7_valid;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_7 == 2;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_previousOccupiedThree;
    }
    if (sysml_lazy_12) {
        sysml_state_vacantTimeThree = 0;
        sysml_state_reportedThree = false;
    }
    else {
        if (io_6 >= ((60000 + 1) - sysml_state_vacantTimeThree)) {
            sysml_conditional_13 = 60000 + 1;
        }
        else {
            sysml_conditional_13 = sysml_state_vacantTimeThree + io_6;
        }
        sysml_state_vacantTimeThree = sysml_conditional_13;
    }
    sysml_lazy_14 = sysml_state_vacantTimeOne > 60000;
    if (sysml_lazy_14) {
        sysml_lazy_14 = !sysml_state_reportedOne;
    }
    if (sysml_lazy_14) {
        sysml_state_restockReport = true;
        sysml_state_reportedOne = true;
    }
    sysml_lazy_15 = sysml_state_vacantTimeTwo > 60000;
    if (sysml_lazy_15) {
        sysml_lazy_15 = !sysml_state_reportedTwo;
    }
    if (sysml_lazy_15) {
        sysml_state_restockReport = true;
        sysml_state_reportedTwo = true;
    }
    sysml_lazy_16 = sysml_state_vacantTimeThree > 60000;
    if (sysml_lazy_16) {
        sysml_lazy_16 = !sysml_state_reportedThree;
    }
    if (sysml_lazy_16) {
        sysml_state_restockReport = true;
        sysml_state_reportedThree = true;
    }
    sysml_state_previousOccupiedOne = io_0;
    sysml_state_previousOccupiedTwo = io_1;
    sysml_state_previousOccupiedThree = io_2;
    bool sysml_value_0 = 0;
    sysml_lazy_17 = io_0;
    if (sysml_lazy_17) {
        sysml_lazy_17 = io_1;
    }
    sysml_lazy_18 = sysml_lazy_17;
    if (sysml_lazy_18) {
        sysml_lazy_18 = io_2;
    }
    sysml_value_0 = sysml_lazy_18;
    bool sysml_value_1 = 0;
    sysml_lazy_19 = io_7_valid;
    if (sysml_lazy_19) {
        sysml_lazy_19 = io_7 == 2;
    }
    sysml_value_1 = sysml_lazy_19;
    int sysml_value_2 = 0;
    sysml_value_2 = 60000;
    int sysml_value_3 = 0;
    if (io_0) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_3 = sysml_conditional_20;
    int sysml_value_4 = 0;
    if (io_1) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    sysml_value_4 = sysml_conditional_21;
    int sysml_value_5 = 0;
    if (io_2) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_value_5 = sysml_conditional_22;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_restockReport;
    bool sysml_value_7 = 0;
    sysml_lazy_23 = io_7_valid;
    if (sysml_lazy_23) {
        sysml_lazy_23 = io_7 == 1;
    }
    sysml_value_7 = sysml_lazy_23;
    bool sysml_value_8 = 0;
    sysml_value_8 = io_0;
    bool sysml_value_9 = 0;
    sysml_value_9 = io_1;
    bool sysml_value_10 = 0;
    sysml_value_10 = io_2;
    int sysml_value_11 = 0;
    if (sysml_value_0) {
        sysml_conditional_24 = 2000;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_value_11 = sysml_conditional_24;
    bool sysml_value_12 = 0;
    sysml_lazy_25 = sysml_value_0;
    if (!sysml_lazy_25) {
        sysml_lazy_25 = sysml_state_vacantTimeOne > sysml_value_2;
    }
    sysml_lazy_26 = sysml_lazy_25;
    if (!sysml_lazy_26) {
        sysml_lazy_26 = sysml_state_vacantTimeTwo > sysml_value_2;
    }
    sysml_lazy_27 = sysml_lazy_26;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_state_vacantTimeThree > sysml_value_2;
    }
    sysml_value_12 = sysml_lazy_27;
    int sysml_value_13 = 0;
    sysml_value_13 = ((sysml_value_3 + sysml_value_4) + sysml_value_5);
    *io_3 = sysml_value_13;
    *io_4 = sysml_value_11;
    *io_5 = sysml_value_12;
    *io_8 = sysml_value_6;
    *serial_0_trigger = sysml_value_7;
    *serial_0_value_0 = sysml_value_8;
    *serial_0_value_1 = sysml_value_9;
    *serial_0_value_2 = sysml_value_10;
}