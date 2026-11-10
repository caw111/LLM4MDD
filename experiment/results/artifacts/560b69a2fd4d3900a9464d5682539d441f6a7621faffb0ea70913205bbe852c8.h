#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousCounterInput;
/* Global Variable define of  */
extern int sysml_state_accumulatedNoProductMs;
/* Global Variable define of  */
extern int sysml_state_cumulativeCount;
/* Global Variable define of  */
extern int sysml_state_stopCount;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern bool sysml_state_previousStopButton;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_0_value_3);
#endif
