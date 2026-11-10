#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_currentPositionDeg;
/* Global Variable define of  */
extern bool sysml_state_directionForward;
/* Global Variable define of  */
extern bool sysml_state_moving;
/* Global Variable define of  */
extern bool sysml_state_reportPending;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, bool* serial_0_trigger, int* serial_0_value_0);
#endif
