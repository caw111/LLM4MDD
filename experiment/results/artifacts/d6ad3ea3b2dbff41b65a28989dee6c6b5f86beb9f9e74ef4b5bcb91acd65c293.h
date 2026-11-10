#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_readyElapsedMs;
/* Global Variable define of  */
extern int sysml_state_gameElapsedMs;
/* Global Variable define of  */
extern int sysml_state_roundIndex;
/* Global Variable define of  */
extern int sysml_state_ledPhase;
/* Global Variable define of  */
extern int sysml_state_score;
/* Global Variable define of  */
extern bool sysml_state_previousStopPressed;
/* Global Variable define of  */
extern bool sysml_state_previousBackPressed;
/* Global Variable define of  */
extern bool sysml_state_previousForwardPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, bool* io_0, bool* io_1, bool* io_2, int* io_6, bool* serial_0_trigger, int* serial_0_value_0);
#endif
