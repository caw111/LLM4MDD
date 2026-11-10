#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_component_run_value = false;
/* Global Variable define of  */
bool sysml_state_component_run_previous = false;
/* Global Variable define of  */
int sysml_state_component_blink_phase = 500;
/* Global Variable define of  */
bool sysml_state_component_blink_started = false;
/* Global Variable define of  */
bool sysml_state_component_blink_enabled = false;
/* Global Variable define of  */
int sysml_state_component_blink_remainder = 0;
/* Global Variable define of  */
bool sysml_state_component_blink_value = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_component_run_value = false;
    sysml_state_component_run_previous = false;
    sysml_state_component_blink_phase = 500;
    sysml_state_component_blink_started = false;
    sysml_state_component_blink_enabled = false;
    sysml_state_component_blink_remainder = 0;
    sysml_state_component_blink_value = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_2, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_component_run_previous;
    }
    if (sysml_lazy_0) {
        sysml_state_component_run_value = (!sysml_state_component_run_value);
    }
    sysml_state_component_run_previous = io_0;
    sysml_lazy_1 = sysml_state_component_blink_enabled;
    if (sysml_lazy_1) {
        sysml_lazy_1 = io_2 > 0;
    }
    if (sysml_lazy_1) {
        if ((io_2 >= (1000 - sysml_state_component_blink_phase))) {
            sysml_state_component_blink_started = true;
        }
        sysml_state_component_blink_remainder = io_2;
        if ((sysml_state_component_blink_remainder >= 2097152000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 2097152000);
        }
        if ((sysml_state_component_blink_remainder >= 1048576000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 1048576000);
        }
        if ((sysml_state_component_blink_remainder >= 524288000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 524288000);
        }
        if ((sysml_state_component_blink_remainder >= 262144000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 262144000);
        }
        if ((sysml_state_component_blink_remainder >= 131072000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 131072000);
        }
        if ((sysml_state_component_blink_remainder >= 65536000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 65536000);
        }
        if ((sysml_state_component_blink_remainder >= 32768000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 32768000);
        }
        if ((sysml_state_component_blink_remainder >= 16384000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 16384000);
        }
        if ((sysml_state_component_blink_remainder >= 8192000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 8192000);
        }
        if ((sysml_state_component_blink_remainder >= 4096000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 4096000);
        }
        if ((sysml_state_component_blink_remainder >= 2048000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 2048000);
        }
        if ((sysml_state_component_blink_remainder >= 1024000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 1024000);
        }
        if ((sysml_state_component_blink_remainder >= 512000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 512000);
        }
        if ((sysml_state_component_blink_remainder >= 256000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 256000);
        }
        if ((sysml_state_component_blink_remainder >= 128000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 128000);
        }
        if ((sysml_state_component_blink_remainder >= 64000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 64000);
        }
        if ((sysml_state_component_blink_remainder >= 32000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 32000);
        }
        if ((sysml_state_component_blink_remainder >= 16000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 16000);
        }
        if ((sysml_state_component_blink_remainder >= 8000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 8000);
        }
        if ((sysml_state_component_blink_remainder >= 4000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 4000);
        }
        if ((sysml_state_component_blink_remainder >= 2000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 2000);
        }
        if ((sysml_state_component_blink_remainder >= 1000)) {
            sysml_state_component_blink_remainder = (sysml_state_component_blink_remainder - 1000);
        }
        if ((sysml_state_component_blink_remainder >= (1000 - sysml_state_component_blink_phase))) {
            sysml_state_component_blink_phase = (sysml_state_component_blink_remainder - (1000 - sysml_state_component_blink_phase));
        }
        else {
            sysml_state_component_blink_phase = (sysml_state_component_blink_phase + sysml_state_component_blink_remainder);
        }
    }
    if ((!sysml_state_component_run_value)) {
        sysml_state_component_blink_phase = 500;
        sysml_state_component_blink_started = false;
    }
    sysml_lazy_2 = sysml_state_component_run_value;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_component_blink_started;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_component_blink_phase < 500;
    }
    sysml_state_component_blink_value = sysml_lazy_3;
    sysml_state_component_blink_enabled = sysml_state_component_run_value;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_component_blink_value;
    *io_1 = sysml_value_0;
}