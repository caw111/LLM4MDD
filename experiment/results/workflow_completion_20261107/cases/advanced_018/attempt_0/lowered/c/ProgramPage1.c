#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_setpointMode_value = false;
/* Global Variable define of  */
bool sysml_state_component_setpointMode_previous = false;
/* Global Variable define of  */
int sysml_state_component_ventilation_phase = 0;
/* Global Variable define of  */
bool sysml_state_component_ventilation_started = false;
/* Global Variable define of  */
bool sysml_state_component_ventilation_enabled = false;
/* Global Variable define of  */
int sysml_state_component_ventilation_remainder = 0;
/* Global Variable define of  */
bool sysml_state_component_ventilation_value = false;
/* Global Variable define of  */
bool sysml_state_heaterCommand = false;
/* Global Variable define of  */
double sysml_state_cachedTemperature = 0.0;
/* Global Variable define of  */
bool sysml_state_hasReading = false;
/* Global Variable define of  */
bool sysml_state_previousOutside = false;
/* Global Variable define of  */
int sysml_state_outsideDuration = 0;
/* Global Variable define of  */
bool sysml_state_alarmActive = false;
/* Global Variable define of  */
bool sysml_state_reportRaised = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_setpointMode_value = false;
    sysml_state_component_setpointMode_previous = false;
    sysml_state_component_ventilation_phase = 0;
    sysml_state_component_ventilation_started = false;
    sysml_state_component_ventilation_enabled = false;
    sysml_state_component_ventilation_remainder = 0;
    sysml_state_component_ventilation_value = false;
    sysml_state_heaterCommand = false;
    sysml_state_cachedTemperature = 0.0;
    sysml_state_hasReading = false;
    sysml_state_previousOutside = false;
    sysml_state_outsideDuration = 0;
    sysml_state_alarmActive = false;
    sysml_state_reportRaised = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int io_9, bool* io_2, bool* io_3, int* io_4, int* io_5, bool* io_6, bool* io_8) {
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
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    sysml_lazy_0 = io_7;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_setpointMode_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_setpointMode_value = (!sysml_state_component_setpointMode_value);
    }
    sysml_state_component_setpointMode_previous = io_7;
    sysml_lazy_1 = sysml_state_component_ventilation_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_9 > 0;
    }
    if (sysml_lazy_1) {
        if ((io_9 >= (300000 - sysml_state_component_ventilation_phase))) {
            sysml_state_component_ventilation_started = true;
        }
        sysml_state_component_ventilation_remainder = io_9;
        if ((sysml_state_component_ventilation_remainder >= 1228800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 1228800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 614400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 614400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 307200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 307200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 153600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 153600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 76800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 76800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 38400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 38400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 19200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 19200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 9600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 9600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 4800000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 4800000);
        }
        if ((sysml_state_component_ventilation_remainder >= 2400000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 2400000);
        }
        if ((sysml_state_component_ventilation_remainder >= 1200000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 1200000);
        }
        if ((sysml_state_component_ventilation_remainder >= 600000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 600000);
        }
        if ((sysml_state_component_ventilation_remainder >= 300000)) {
            sysml_state_component_ventilation_remainder = (sysml_state_component_ventilation_remainder - 300000);
        }
        if ((sysml_state_component_ventilation_remainder >= (300000 - sysml_state_component_ventilation_phase))) {
            sysml_state_component_ventilation_phase = (sysml_state_component_ventilation_remainder - (300000 - sysml_state_component_ventilation_phase));
        }
        else {
            sysml_state_component_ventilation_phase = (sysml_state_component_ventilation_phase + sysml_state_component_ventilation_remainder);
        }
    }
    sysml_lazy_2 = true;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_ventilation_started;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_component_ventilation_phase < 10000;
    }
    sysml_state_component_ventilation_value = sysml_lazy_3;
    sysml_state_component_ventilation_enabled = true;
    sysml_state_reportRaised = false;
    if (io_1) {
        sysml_state_cachedTemperature = io_0;
        sysml_state_hasReading = true;
        if ((io_0 < 37.0)) {
            sysml_state_heaterCommand = true;
        }
        else {
            if ((io_0 > 38.0)) {
                sysml_state_heaterCommand = false;
            }
        }
        sysml_lazy_4 = io_0 < 37.0;
        if (!sysml_lazy_4) {
            sysml_lazy_4 = io_0 > 38.0;
        }
        if (sysml_lazy_4) {
            if (sysml_state_previousOutside) {
                if (io_9 >= (60000 - sysml_state_outsideDuration)) {
                    sysml_conditional_5 = 60000;
                }
                else {
                    sysml_conditional_5 = sysml_state_outsideDuration + io_9;
                }
                sysml_state_outsideDuration = sysml_conditional_5;
            }
            else {
                sysml_state_outsideDuration = 0;
            }
            sysml_lazy_6 = sysml_state_outsideDuration >= 60000;
            if (sysml_lazy_6) {
                sysml_lazy_6 = !sysml_state_alarmActive;
            }
            if (sysml_lazy_6) {
                sysml_state_reportRaised = true;
                sysml_state_alarmActive = true;
            }
        }
        else {
            sysml_state_outsideDuration = 0;
            sysml_state_alarmActive = false;
        }
    }
    else {
        sysml_state_heaterCommand = false;
        sysml_state_outsideDuration = 0;
    }
    sysml_lazy_7 = io_1;
    if (sysml_lazy_7) {
        sysml_lazy_8 = io_0 < 37.0;
        if (!sysml_lazy_8) {
            sysml_lazy_8 = io_0 > 38.0;
        }
        sysml_lazy_7 = sysml_lazy_8;
    }
    sysml_state_previousOutside = sysml_lazy_7;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_heaterCommand;
    bool sysml_value_1 = 0;
    sysml_lazy_9 = sysml_state_component_ventilation_value;
    if (!sysml_lazy_9) {
        sysml_lazy_10 = io_1;
        if (sysml_lazy_10) {
            sysml_lazy_10 = io_0 > 38.0;
        }
        sysml_lazy_9 = sysml_lazy_10;
    }
    sysml_value_1 = sysml_lazy_9;
    int sysml_value_2 = 0;
    if (sysml_state_alarmActive) {
        sysml_conditional_11 = 1000;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_2 = sysml_conditional_11;
    int sysml_value_3 = 0;
    if (sysml_state_hasReading) {
        if (sysml_state_component_setpointMode_value) {
            sysml_conditional_13 = 38;
        }
        else {
            if ((sysml_state_cachedTemperature + 0.5) < ((double)((int)(sysml_state_cachedTemperature + 0.5)))) {
                sysml_conditional_14 = 1;
            }
            else {
                sysml_conditional_14 = 0;
            }
            sysml_conditional_13 = ((int)(sysml_state_cachedTemperature + 0.5)) - sysml_conditional_14;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    else {
        sysml_conditional_12 = 10000;
    }
    sysml_value_3 = sysml_conditional_12;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_alarmActive;
    bool sysml_value_5 = 0;
    sysml_value_5 = sysml_state_reportRaised;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_8 = sysml_value_5;
}