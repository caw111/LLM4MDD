#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_dhtValidLatched;
/* Global Variable define of  */
extern double sysml_state_tempLatched;
/* Global Variable define of  */
extern double sysml_state_distLatched;
/* Global Variable define of  */
extern bool sysml_state_alarmOn;
/* Global Variable define of  */
extern int sysml_state_alarmPhaseMs;
/* Global Variable define of  */
extern bool sysml_state_prevAlarmOn;
/* Global Variable define of  */
extern bool sysml_state_lastReasonAny;
/* Global Variable define of  */
extern int sysml_state_displaySwitchAccumMs;
/* Global Variable define of  */
extern bool sysml_state_displayIsTemp;
/* Global Variable define of  */
extern int sysml_state_reportGateHoldMs;
/* Global Variable define of  */
extern int sysml_state_reportEveryAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, bool io_4, int io_7, int* io_5, int* io_6, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1);
#endif
