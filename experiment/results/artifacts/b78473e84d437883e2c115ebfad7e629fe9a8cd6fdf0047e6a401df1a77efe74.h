#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_component_showSetting_value;
/* Global Variable define of  */
extern bool sysml_state_component_showSetting_previous;
/* Global Variable define of  */
extern int sysml_state_component_ventilation_phase;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_started;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_enabled;
/* Global Variable define of  */
extern int sysml_state_component_ventilation_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_ventilation_value;
/* Global Variable define of  */
extern bool sysml_state_heaterOn;
/* Global Variable define of  */
extern int sysml_state_previousSide;
/* Global Variable define of  */
extern int sysml_state_outsideDuration;
/* Global Variable define of  */
extern bool sysml_state_previousAlarm;
/* Global Variable define of  */
extern bool sysml_state_reportPulse;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int io_8, bool* io_2, bool* io_3, int* io_4, int* io_5, bool* io_6, bool* io_9);
#endif
