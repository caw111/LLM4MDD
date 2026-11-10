#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_warningLevel;
/* Global Variable define of  */
extern bool sysml_state_prevFlame;
/* Global Variable define of  */
extern bool sysml_state_toneRunning;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, int* io_3, bool* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1);
#endif
