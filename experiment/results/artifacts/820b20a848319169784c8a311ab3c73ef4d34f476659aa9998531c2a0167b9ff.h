#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_pollTimerMs;
/* Global Variable define of  */
extern int sysml_state_distanceBelowCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, bool* serial_1_trigger, bool* serial_1_value_0);
#endif
