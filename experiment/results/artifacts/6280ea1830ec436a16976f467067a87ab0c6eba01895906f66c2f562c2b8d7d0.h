#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_stageId;
/* Global Variable define of  */
extern int sysml_state_remainingSeconds;
/* Global Variable define of  */
extern double sysml_state_targetDegC;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD2;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD3;
/* Global Variable define of  */
extern bool sysml_state_prevButtonD4;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, double* serial_0_value_2, int* serial_0_value_3);
#endif
