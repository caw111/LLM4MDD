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
extern double sysml_state_pwmValue;
/* Global Variable define of  */
extern int sysml_state_phaseMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_1, int io_2, bool io_2_valid, double* io_0, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1);
#endif
