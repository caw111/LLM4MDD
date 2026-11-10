#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_bootComplete = false;
/* Global Variable define of  */
bool sysml_state_operational = false;
/* Global Variable define of  */
bool sysml_state_automaticMode = true;
/* Global Variable define of  */
int sysml_state_selectedActuator = 0;
/* Global Variable define of  */
double sysml_state_fanCommand = 0.0;
/* Global Variable define of  */
bool sysml_state_lightCommand = false;
/* Global Variable define of  */
bool sysml_state_pumpCommand = false;
/* Global Variable define of  */
double sysml_state_savedTemperature = 0.0;
/* Global Variable define of  */
bool sysml_state_temperatureAvailable = false;
/* Global Variable define of  */
bool sysml_state_modeCandidate = false;
/* Global Variable define of  */
bool sysml_state_modeStable = false;
/* Global Variable define of  */
int sysml_state_modeStableMs = 0;
/* Global Variable define of  */
bool sysml_state_modePressEvent = false;
/* Global Variable define of  */
bool sysml_state_selectCandidate = false;
/* Global Variable define of  */
bool sysml_state_selectStable = false;
/* Global Variable define of  */
int sysml_state_selectStableMs = 0;
/* Global Variable define of  */
bool sysml_state_selectPressEvent = false;
/* Global Variable define of  */
int sysml_state_displayPhase = 0;
/* Global Variable define of  */
int sysml_state_displayDelta = 0;
/* Global Variable define of  */
bool sysml_state_previousDhtFault = false;
/* Global Variable define of  */
bool sysml_state_previousLightFault = false;
/* Global Variable define of  */
bool sysml_state_previousSoilFault = false;
/* Global Variable define of  */
bool sysml_state_reportEvent = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_bootComplete = false;
    sysml_state_operational = false;
    sysml_state_automaticMode = true;
    sysml_state_selectedActuator = 0;
    sysml_state_fanCommand = 0.0;
    sysml_state_lightCommand = false;
    sysml_state_pumpCommand = false;
    sysml_state_savedTemperature = 0.0;
    sysml_state_temperatureAvailable = false;
    sysml_state_modeCandidate = false;
    sysml_state_modeStable = false;
    sysml_state_modeStableMs = 0;
    sysml_state_modePressEvent = false;
    sysml_state_selectCandidate = false;
    sysml_state_selectStable = false;
    sysml_state_selectStableMs = 0;
    sysml_state_selectPressEvent = false;
    sysml_state_displayPhase = 0;
    sysml_state_displayDelta = 0;
    sysml_state_previousDhtFault = false;
    sysml_state_previousLightFault = false;
    sysml_state_previousSoilFault = false;
    sysml_state_reportEvent = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, double io_4, bool io_8, bool io_9, int io_12, double* io_5, bool* io_6, bool* io_7, int* io_10, int* io_11, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    bool sysml_lazy_6 = 0;
    double sysml_conditional_7 = 0;
    double sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    bool sysml_lazy_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    int sysml_conditional_32 = 0;
    sysml_state_operational = sysml_state_bootComplete;
    sysml_state_bootComplete = true;
    sysml_state_modePressEvent = false;
    sysml_state_selectPressEvent = false;
    sysml_state_reportEvent = false;
    if (io_1) {
        sysml_state_savedTemperature = io_0;
        sysml_state_temperatureAvailable = true;
    }
    if ((io_8 != sysml_state_modeCandidate)) {
        sysml_state_modeCandidate = io_8;
        sysml_state_modeStableMs = 0;
    }
    else {
        if ((io_12 >= (20 - sysml_state_modeStableMs))) {
            sysml_state_modeStableMs = 20;
        }
        else {
            if ((io_12 > 0)) {
                sysml_state_modeStableMs = (sysml_state_modeStableMs + io_12);
            }
        }
    }
    sysml_lazy_0 = sysml_state_modeStableMs >= 20;
    if (sysml_lazy_0) {
        sysml_lazy_0 = sysml_state_modeStable != sysml_state_modeCandidate;
    }
    if (sysml_lazy_0) {
        sysml_state_modePressEvent = sysml_state_modeCandidate;
        sysml_state_modeStable = sysml_state_modeCandidate;
    }
    if ((io_9 != sysml_state_selectCandidate)) {
        sysml_state_selectCandidate = io_9;
        sysml_state_selectStableMs = 0;
    }
    else {
        if ((io_12 >= (20 - sysml_state_selectStableMs))) {
            sysml_state_selectStableMs = 20;
        }
        else {
            if ((io_12 > 0)) {
                sysml_state_selectStableMs = (sysml_state_selectStableMs + io_12);
            }
        }
    }
    sysml_lazy_1 = sysml_state_selectStableMs >= 20;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_selectStable != sysml_state_selectCandidate;
    }
    if (sysml_lazy_1) {
        sysml_state_selectPressEvent = sysml_state_selectCandidate;
        sysml_state_selectStable = sysml_state_selectCandidate;
    }
    if (sysml_state_modePressEvent) {
        sysml_state_automaticMode = (!sysml_state_automaticMode);
    }
    sysml_lazy_2 = sysml_state_selectPressEvent;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_automaticMode;
    }
    if (sysml_lazy_2) {
        if (sysml_state_selectedActuator == 2) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_selectedActuator + 1;
        }
        sysml_state_selectedActuator = sysml_conditional_3;
    }
    if ((io_12 > 0)) {
        if ((((double)io_12) / 6000) < ((double)((int)(((double)io_12) / 6000)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_arithmetic_5 = ((int)(((double)io_12) / 6000)) - sysml_conditional_4;
        sysml_state_displayDelta = io_12 - (6000 * sysml_arithmetic_5);
        sysml_state_displayPhase = (sysml_state_displayPhase + sysml_state_displayDelta);
        if ((sysml_state_displayPhase >= 6000)) {
            sysml_state_displayPhase = (sysml_state_displayPhase - 6000);
        }
    }
    if (sysml_state_operational) {
        if (sysml_state_automaticMode) {
            sysml_lazy_6 = !sysml_state_temperatureAvailable;
            if (!sysml_lazy_6) {
                sysml_lazy_6 = sysml_state_savedTemperature <= 25.0;
            }
            if (sysml_lazy_6) {
                sysml_conditional_7 = 0.0;
            }
            else {
                if (sysml_state_savedTemperature >= 35.0) {
                    sysml_conditional_8 = 255.0;
                }
                else {
                    sysml_conditional_8 = ((double)((sysml_state_savedTemperature - 25.0) * 255.0)) / 10.0;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_state_fanCommand = sysml_conditional_7;
            sysml_state_lightCommand = (io_2 < 400.0);
            sysml_state_pumpCommand = (io_3 < 400.0);
        }
        else {
            if ((sysml_state_selectedActuator == 0)) {
                sysml_state_fanCommand = (((double)((io_4 * 255.0)) / 1023.0));
            }
            else {
                if ((sysml_state_selectedActuator == 1)) {
                    sysml_state_lightCommand = (io_4 >= 512.0);
                }
                else {
                    sysml_state_pumpCommand = (io_4 >= 512.0);
                }
            }
        }
        sysml_lazy_9 = !io_1;
        if (!sysml_lazy_9) {
            sysml_lazy_10 = io_2 == 0.0;
            if (!sysml_lazy_10) {
                sysml_lazy_10 = io_2 == 1023.0;
            }
            sysml_lazy_9 = sysml_lazy_10;
        }
        sysml_lazy_11 = sysml_lazy_9;
        if (!sysml_lazy_11) {
            sysml_lazy_12 = io_3 == 0.0;
            if (!sysml_lazy_12) {
                sysml_lazy_12 = io_3 == 1023.0;
            }
            sysml_lazy_11 = sysml_lazy_12;
        }
        sysml_lazy_13 = sysml_lazy_11;
        if (sysml_lazy_13) {
            sysml_lazy_14 = (!io_1) != sysml_state_previousDhtFault;
            if (!sysml_lazy_14) {
                sysml_lazy_15 = io_2 == 0.0;
                if (!sysml_lazy_15) {
                    sysml_lazy_15 = io_2 == 1023.0;
                }
                sysml_lazy_14 = sysml_lazy_15 != sysml_state_previousLightFault;
            }
            sysml_lazy_16 = sysml_lazy_14;
            if (!sysml_lazy_16) {
                sysml_lazy_17 = io_3 == 0.0;
                if (!sysml_lazy_17) {
                    sysml_lazy_17 = io_3 == 1023.0;
                }
                sysml_lazy_16 = sysml_lazy_17 != sysml_state_previousSoilFault;
            }
            sysml_lazy_13 = sysml_lazy_16;
        }
        sysml_state_reportEvent = sysml_lazy_13;
        sysml_state_previousDhtFault = ((!io_1));
        sysml_lazy_18 = io_2 == 0.0;
        if (!sysml_lazy_18) {
            sysml_lazy_18 = io_2 == 1023.0;
        }
        sysml_state_previousLightFault = sysml_lazy_18;
        sysml_lazy_19 = io_3 == 0.0;
        if (!sysml_lazy_19) {
            sysml_lazy_19 = io_3 == 1023.0;
        }
        sysml_state_previousSoilFault = sysml_lazy_19;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = ((double)((io_4 * 255.0)) / 1023.0);
    bool sysml_value_1 = 0;
    sysml_value_1 = (!io_1);
    bool sysml_value_2 = 0;
    sysml_lazy_20 = io_2 == 0.0;
    if (!sysml_lazy_20) {
        sysml_lazy_20 = io_2 == 1023.0;
    }
    sysml_value_2 = sysml_lazy_20;
    bool sysml_value_3 = 0;
    sysml_lazy_21 = io_3 == 0.0;
    if (!sysml_lazy_21) {
        sysml_lazy_21 = io_3 == 1023.0;
    }
    sysml_value_3 = sysml_lazy_21;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_fanCommand;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_lightCommand;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_pumpCommand;
    int sysml_value_7 = 0;
    if (sysml_state_displayPhase < 2000) {
        if ((sysml_state_savedTemperature + 0.5) < ((double)((int)(sysml_state_savedTemperature + 0.5)))) {
            sysml_conditional_23 = 1;
        }
        else {
            sysml_conditional_23 = 0;
        }
        sysml_conditional_22 = ((int)(sysml_state_savedTemperature + 0.5)) - sysml_conditional_23;
    }
    else {
        if (sysml_state_displayPhase < 4000) {
            if (io_2 < ((double)((int)io_2))) {
                sysml_conditional_25 = 1;
            }
            else {
                sysml_conditional_25 = 0;
            }
            sysml_conditional_24 = ((int)io_2) - sysml_conditional_25;
        }
        else {
            if (io_3 < ((double)((int)io_3))) {
                sysml_conditional_26 = 1;
            }
            else {
                sysml_conditional_26 = 0;
            }
            sysml_conditional_24 = ((int)io_3) - sysml_conditional_26;
        }
        sysml_conditional_22 = sysml_conditional_24;
    }
    sysml_value_7 = sysml_conditional_22;
    bool sysml_value_8 = 0;
    sysml_value_8 = sysml_state_reportEvent;
    bool sysml_value_9 = 0;
    sysml_value_9 = sysml_value_1;
    bool sysml_value_10 = 0;
    sysml_value_10 = sysml_value_2;
    bool sysml_value_11 = 0;
    sysml_lazy_27 = sysml_value_1;
    if (!sysml_lazy_27) {
        sysml_lazy_27 = sysml_value_2;
    }
    sysml_lazy_28 = sysml_lazy_27;
    if (!sysml_lazy_28) {
        sysml_lazy_28 = sysml_value_3;
    }
    sysml_value_11 = sysml_lazy_28;
    bool sysml_value_12 = 0;
    sysml_lazy_29 = sysml_value_1 != sysml_state_previousDhtFault;
    if (!sysml_lazy_29) {
        sysml_lazy_29 = sysml_value_2 != sysml_state_previousLightFault;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (!sysml_lazy_30) {
        sysml_lazy_30 = sysml_value_3 != sysml_state_previousSoilFault;
    }
    sysml_value_12 = sysml_lazy_30;
    bool sysml_value_13 = 0;
    sysml_value_13 = sysml_value_3;
    int sysml_value_14 = 0;
    sysml_lazy_31 = sysml_state_operational;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_value_11;
    }
    if (sysml_lazy_31) {
        sysml_conditional_32 = 2000;
    }
    else {
        sysml_conditional_32 = 0;
    }
    sysml_value_14 = sysml_conditional_32;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_10 = sysml_value_7;
    *io_11 = sysml_value_14;
    *serial_0_trigger = sysml_value_8;
    *serial_0_value_0 = sysml_value_9;
    *serial_0_value_1 = sysml_value_10;
    *serial_0_value_2 = sysml_value_13;
}