#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_running;
/* Global Variable define of  */
extern bool sysml_state_paused;
/* Global Variable define of  */
extern int sysml_state_seconds;
/* Global Variable define of  */
extern bool sysml_state_previousStartPausePressed;
/* Global Variable define of  */
extern bool sysml_state_previousResetPressed;
/* Global Variable define of  */
extern bool sysml_state_buzzerOn;
/* Global Variable define of  */
extern int sysml_state_buzzerRemainingMs;
/* Global Variable define of  */
extern int sysml_state_toneHz;
/* Global Variable define of  */
extern int sysml_state_displayValue;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int* io_2, int* io_3);
#endif
