#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousAnyPIR;
/* Global Variable define of  */
extern bool sysml_state_isLighting;
/* Global Variable define of  */
extern int sysml_state_todayCount;
/* Global Variable define of  */
extern int sysml_state_illuminatedOnTimeMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, int* io_4, double* io_5, bool* serial_0_trigger, int* serial_0_value_0);
#endif
