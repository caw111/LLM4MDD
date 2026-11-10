#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_stage;
/* Global Variable define of  */
extern bool sysml_state_activeTemperatureReached;
/* Global Variable define of  */
extern int sysml_state_remainingSeconds;
/* Global Variable define of  */
extern int sysml_state_totalSeconds;
/* Global Variable define of  */
extern bool sysml_state_heatingActive;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_2, bool io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, double* serial_0_value_2, int* serial_0_value_3, bool* serial_0_value_4);
#endif
