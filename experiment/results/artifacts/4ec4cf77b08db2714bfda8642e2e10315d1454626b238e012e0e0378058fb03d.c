#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_run_value = true;
/* Global Variable define of  */
bool sysml_state_component_run_previous = false;
/* Global Variable define of  */
int sysml_state_component_beep_phase = 0;
/* Global Variable define of  */
bool sysml_state_component_beep_started = false;
/* Global Variable define of  */
bool sysml_state_component_beep_enabled = false;
/* Global Variable define of  */
int sysml_state_component_beep_remainder = 0;
/* Global Variable define of  */
bool sysml_state_component_beep_value = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_run_value = true;
    sysml_state_component_run_previous = false;
    sysml_state_component_beep_phase = 0;
    sysml_state_component_beep_started = false;
    sysml_state_component_beep_enabled = false;
    sysml_state_component_beep_remainder = 0;
    sysml_state_component_beep_value = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_2, int* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_run_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_run_value = (!sysml_state_component_run_value);
    }
    sysml_state_component_run_previous = io_0;
    sysml_lazy_1 = sysml_state_component_beep_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2 > 0;
    }
    if (sysml_lazy_1) {
        if ((io_2 >= (3000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_started = true;
        }
        sysml_state_component_beep_remainder = io_2;
        if ((sysml_state_component_beep_remainder >= 1572864000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1572864000);
        }
        if ((sysml_state_component_beep_remainder >= 786432000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 786432000);
        }
        if ((sysml_state_component_beep_remainder >= 393216000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 393216000);
        }
        if ((sysml_state_component_beep_remainder >= 196608000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 196608000);
        }
        if ((sysml_state_component_beep_remainder >= 98304000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 98304000);
        }
        if ((sysml_state_component_beep_remainder >= 49152000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 49152000);
        }
        if ((sysml_state_component_beep_remainder >= 24576000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 24576000);
        }
        if ((sysml_state_component_beep_remainder >= 12288000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 12288000);
        }
        if ((sysml_state_component_beep_remainder >= 6144000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 6144000);
        }
        if ((sysml_state_component_beep_remainder >= 3072000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 3072000);
        }
        if ((sysml_state_component_beep_remainder >= 1536000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 1536000);
        }
        if ((sysml_state_component_beep_remainder >= 768000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 768000);
        }
        if ((sysml_state_component_beep_remainder >= 384000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 384000);
        }
        if ((sysml_state_component_beep_remainder >= 192000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 192000);
        }
        if ((sysml_state_component_beep_remainder >= 96000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 96000);
        }
        if ((sysml_state_component_beep_remainder >= 48000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 48000);
        }
        if ((sysml_state_component_beep_remainder >= 24000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 24000);
        }
        if ((sysml_state_component_beep_remainder >= 12000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 12000);
        }
        if ((sysml_state_component_beep_remainder >= 6000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 6000);
        }
        if ((sysml_state_component_beep_remainder >= 3000)) {
            sysml_state_component_beep_remainder = (sysml_state_component_beep_remainder - 3000);
        }
        if ((sysml_state_component_beep_remainder >= (3000 - sysml_state_component_beep_phase))) {
            sysml_state_component_beep_phase = (sysml_state_component_beep_remainder - (3000 - sysml_state_component_beep_phase));
        }
        else {
            sysml_state_component_beep_phase = (sysml_state_component_beep_phase + sysml_state_component_beep_remainder);
        }
    }
    sysml_lazy_2 = sysml_state_component_run_value;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_beep_started;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_component_beep_phase < 100;
    }
    sysml_state_component_beep_value = sysml_lazy_3;
    sysml_state_component_beep_enabled = sysml_state_component_run_value;
    int sysml_value_0 = 0;
    if (sysml_state_component_beep_value) {
        sysml_conditional_4 = 1000;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_value_0 = sysml_conditional_4;
    *io_1 = sysml_value_0;
}