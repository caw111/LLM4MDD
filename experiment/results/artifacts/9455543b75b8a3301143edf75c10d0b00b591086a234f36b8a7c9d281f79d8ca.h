#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_peopleCount;
/* Global Variable define of  */
extern bool sysml_state_showTempMode;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern double sysml_state_tempHold;
/* Global Variable define of  */
extern bool sysml_state_lastEntranceValid;
/* Global Variable define of  */
extern bool sysml_state_lastExitValid;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, double io_4, double io_5, bool io_6, bool io_9, double* io_7, int* io_8, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1);
#endif
