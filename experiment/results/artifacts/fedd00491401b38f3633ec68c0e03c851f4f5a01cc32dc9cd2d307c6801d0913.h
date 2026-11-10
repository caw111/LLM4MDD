#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_stageIndex;
/* Global Variable define of  */
extern bool sysml_state_paused;
/* Global Variable define of  */
extern bool sysml_state_completed;
/* Global Variable define of  */
extern bool sysml_state_prevIo0;
/* Global Variable define of  */
extern bool sysml_state_prevIo1;
/* Global Variable define of  */
extern bool sysml_state_prevIo2;
/* Global Variable define of  */
extern int sysml_state_stage1RemainingMs;
/* Global Variable define of  */
extern int sysml_state_stage2RemainingMs;
/* Global Variable define of  */
extern int sysml_state_stage3RemainingMs;
/* Global Variable define of  */
extern int sysml_state_stage1ElapsedMs;
/* Global Variable define of  */
extern int sysml_state_stage2ElapsedMs;
/* Global Variable define of  */
extern int sysml_state_stage3ElapsedMs;
/* Global Variable define of  */
extern int sysml_state_totalElapsedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3);
#endif
