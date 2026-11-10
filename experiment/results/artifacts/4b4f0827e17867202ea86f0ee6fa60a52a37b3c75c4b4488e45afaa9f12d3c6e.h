#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_ledPhaseMs;
/* Global Variable define of  */
extern int sysml_state_sevsegPhaseMs;
/* Global Variable define of  */
extern int sysml_state_sevsegDigit;
/* Global Variable define of  */
extern int sysml_state_servo1PhaseMs;
/* Global Variable define of  */
extern int sysml_state_servo2PhaseMs;
/* Global Variable define of  */
extern bool sysml_state_servo1AtEndpoint;
/* Global Variable define of  */
extern bool sysml_state_servo2AtEndpoint;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int elapsedMilliseconds, bool* io_0, bool* io_1, int* io_2, double* io_3, double* io_4);
#endif
