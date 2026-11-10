#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_processComplete = false;
/* Global Variable define of  */
int sysml_state_elapsed_stage1_ms = 0;
/* Global Variable define of  */
int sysml_state_elapsed_stage2_ms = 0;
/* Global Variable define of  */
int sysml_state_elapsed_stage3_ms = 0;
/* Global Variable define of  */
int sysml_state_countdown_display = 0;
/* Global Variable define of  */
int sysml_state_buzzer_hz = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_processComplete = false;
    sysml_state_elapsed_stage1_ms = 0;
    sysml_state_elapsed_stage2_ms = 0;
    sysml_state_elapsed_stage3_ms = 0;
    sysml_state_countdown_display = 0;
    sysml_state_buzzer_hz = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool io_3, int* io_0, int* io_4, bool* io_5, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    if ((!sysml_state_processComplete)) {
        sysml_state_processComplete = false;
        sysml_state_countdown_display = 0;
        sysml_state_buzzer_hz = 0;
        sysml_state_elapsed_stage1_ms = 0;
        sysml_state_elapsed_stage2_ms = 0;
        sysml_state_elapsed_stage3_ms = 0;
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_countdown_display;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_buzzer_hz;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_processComplete;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_processComplete;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_elapsed_stage1_ms;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_elapsed_stage2_ms;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_elapsed_stage3_ms;
    *io_0 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
}