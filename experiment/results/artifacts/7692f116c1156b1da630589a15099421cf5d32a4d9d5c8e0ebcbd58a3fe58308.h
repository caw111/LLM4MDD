#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_mode;
/* Global Variable define of  */
extern int sysml_state_param;
/* Global Variable define of  */
extern double sysml_state_brightnessGate;
/* Global Variable define of  */
extern int sysml_state_accumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, int io_5, double* io_2, bool* io_3, int* io_4);
#endif
