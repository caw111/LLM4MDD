#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Global Variable define of  */
extern bool sysml_state_modeIsSetting;
/* Global Variable define of  */
extern double sysml_state_targetTemp;
/* Global Variable define of  */
extern double sysml_state_tempLastValid;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern int sysml_state_abnormalHighCount;
/* Global Variable define of  */
extern int sysml_state_abnormalLowCount;
/* Global Variable define of  */
extern bool sysml_state_fanOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_5, double io_6, bool io_7, bool* io_1, bool* io_2, bool* io_3, int* io_4, int* io_8, bool* serial_0_trigger, bool* serial_0_value_0);
#endif
