#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_pollAccumMs = 0;
/* Global Variable define of  */
int sysml_state_distancePersistCount = 0;
/* Global Variable define of  */
bool sysml_state_distanceDetected = false;
/* Global Variable define of  */
bool sysml_state_lastTemperatureValid = false;
/* Global Variable define of  */
bool sysml_state_lastDistanceValid = false;
/* Global Variable define of  */
bool sysml_state_prevDhtValid = false;
/* Global Variable define of  */
bool sysml_state_prevUltrasonicValid = false;
/* Global Variable define of  */
double sysml_state_lastTempSnapshot = 0.0;
/* Global Variable define of  */
double sysml_state_lastDistSnapshot = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_pollAccumMs = 0;
    sysml_state_distancePersistCount = 0;
    sysml_state_distanceDetected = false;
    sysml_state_lastTemperatureValid = false;
    sysml_state_lastDistanceValid = false;
    sysml_state_prevDhtValid = false;
    sysml_state_prevUltrasonicValid = false;
    sysml_state_lastTempSnapshot = 0.0;
    sysml_state_lastDistSnapshot = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_conditional_10 = 0;
    sysml_state_pollAccumMs = (sysml_state_pollAccumMs + io_6);
    if (io_3) {
        sysml_state_lastTempSnapshot = io_1;
        sysml_state_lastTemperatureValid = true;
    }
    else {
        sysml_state_lastTemperatureValid = false;
    }
    if (io_5) {
        sysml_state_lastDistSnapshot = io_4;
        sysml_state_lastDistanceValid = true;
    }
    else {
        sysml_state_lastDistanceValid = false;
    }
    if ((sysml_state_pollAccumMs >= 3000)) {
        sysml_state_pollAccumMs = (sysml_state_pollAccumMs - 3000);
        sysml_lazy_0 = sysml_state_lastDistanceValid;
        if (sysml_lazy_0) {
            sysml_lazy_0 = sysml_state_lastDistSnapshot < 20.0;
        }
        if (sysml_lazy_0) {
            sysml_state_distancePersistCount = (sysml_state_distancePersistCount + 1);
        }
        else {
            sysml_state_distancePersistCount = 0;
        }
        sysml_lazy_1 = sysml_state_lastDistanceValid;
        if (sysml_lazy_1) {
            sysml_lazy_1 = sysml_state_lastDistSnapshot < 20.0;
        }
        if (sysml_lazy_1) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((sysml_state_distancePersistCount + sysml_conditional_2) >= 3) {
            sysml_state_distanceDetected = true;
        }
        else {
            sysml_lazy_3 = sysml_state_lastDistanceValid;
            if (sysml_lazy_3) {
                sysml_lazy_3 = sysml_state_lastDistSnapshot < 20.0;
            }
            if (!sysml_lazy_3) {
                sysml_state_distanceDetected = false;
            }
        }
    }
    sysml_state_prevDhtValid = io_3;
    sysml_state_prevUltrasonicValid = io_5;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_distanceDetected;
    bool sysml_value_1 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_lazy_5 = sysml_state_lastTemperatureValid;
        if (sysml_lazy_5) {
            sysml_lazy_5 = sysml_state_lastDistanceValid;
        }
        sysml_conditional_4 = sysml_lazy_5;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_1 = sysml_conditional_4;
    double sysml_value_2 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_6 = sysml_state_lastTempSnapshot;
    }
    else {
        sysml_conditional_6 = 0.0;
    }
    sysml_value_2 = sysml_conditional_6;
    double sysml_value_3 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_7 = sysml_state_lastDistSnapshot;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_3 = sysml_conditional_7;
    int sysml_value_4 = 0;
    if ((sysml_state_pollAccumMs + io_6) >= 3000) {
        sysml_conditional_8 = sysml_state_distancePersistCount;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    sysml_lazy_9 = (sysml_state_pollAccumMs + io_6) >= 3000;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_distanceDetected;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    sysml_value_6 = sysml_state_distanceDetected;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_1_trigger = sysml_value_5;
    *serial_1_value_0 = sysml_value_6;
}