#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_targetPerMinute;
/* Global Variable define of  */
extern int sysml_state_productCountTotal;
/* Global Variable define of  */
extern int sysml_state_productCountActual;
/* Global Variable define of  */
extern int sysml_state_productCountAtLastToggle;
/* Global Variable define of  */
extern int sysml_state_stopCount;
/* Global Variable define of  */
extern int sysml_state_missedInactivityMs;
/* Global Variable define of  */
extern bool sysml_state_alarmLatched;
/* Global Variable define of  */
extern bool sysml_state_prevProductPulse;
/* Global Variable define of  */
extern bool sysml_state_prevStopButtonPressed;
/* Global Variable define of  */
extern int sysml_state_displayMode;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool io_2, bool io_3, bool* io_4, int* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3, bool* serial_0_value_4);
#endif
