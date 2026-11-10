#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_timeSinceTempSample;
/* Global Variable define of  */
extern int sysml_state_timeSinceDistSample;
/* Global Variable define of  */
extern int sysml_state_displayPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_displayPageIsTemp;
/* Global Variable define of  */
extern int sysml_state_buzzerPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_buzzerWasArmed;
/* Global Variable define of  */
extern bool sysml_state_buzzerPrevAlarm;
/* Global Variable define of  */
extern bool sysml_state_buzzerHasTempAlarm;
/* Global Variable define of  */
extern bool sysml_state_buzzerHasDistAlarm;
/* Global Variable define of  */
extern bool sysml_state_lastReportedTempAlarm;
/* Global Variable define of  */
extern bool sysml_state_lastReportedDistAlarm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int io_7, int* io_5, int* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1);
#endif
