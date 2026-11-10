#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousProximity;
/* Global Variable define of  */
extern bool sysml_state_valveOn;
/* Global Variable define of  */
extern int sysml_state_valveRemainingMs;
/* Global Variable define of  */
extern int sysml_state_todayUses;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern int sysml_state_alarmRemainingMs;
/* Global Variable define of  */
extern bool sysml_state_lastDistanceValid;
/* Global Variable define of  */
extern int sysml_state_lastDisplayedPercent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_2, bool io_3, bool* io_1, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1);
#endif
