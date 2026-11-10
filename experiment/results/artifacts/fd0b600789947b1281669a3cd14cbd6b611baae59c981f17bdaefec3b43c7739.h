#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_processComplete;
/* Global Variable define of  */
extern int sysml_state_elapsed_stage1_ms;
/* Global Variable define of  */
extern int sysml_state_elapsed_stage2_ms;
/* Global Variable define of  */
extern int sysml_state_elapsed_stage3_ms;
/* Global Variable define of  */
extern int sysml_state_countdown_display;
/* Global Variable define of  */
extern int sysml_state_buzzer_hz;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool io_3, int* io_0, int* io_4, bool* io_5, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2);
#endif
