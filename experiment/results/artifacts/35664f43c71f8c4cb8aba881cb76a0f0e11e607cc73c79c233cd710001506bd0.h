#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_modeCycling;
/* Global Variable define of  */
extern bool sysml_state_modeOff;
/* Global Variable define of  */
extern int sysml_state_currentColorIndex;
/* Global Variable define of  */
extern bool sysml_state_previousK1;
/* Global Variable define of  */
extern int sysml_state_accumulatedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int io_6, double* io_0, double* io_1, double* io_2);
#endif
