#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_automaticMode;
/* Global Variable define of  */
extern bool sysml_state_previousAutoPressed;
/* Global Variable define of  */
extern bool sysml_state_previousManualPressed;
/* Global Variable define of  */
extern double sysml_state_rampPhaseMs;
/* Global Variable define of  */
extern double sysml_state_directionDurationMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int io_4, double* io_3);
#endif
