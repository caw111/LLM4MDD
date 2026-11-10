#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_heaterEnabled;
/* Global Variable define of  */
extern bool sysml_state_aeratorEnabled;
/* Global Variable define of  */
extern bool sysml_state_alarmEnabled;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_6, double* io_3, double* io_4, double* io_5, int* io_7, int* io_8);
#endif
