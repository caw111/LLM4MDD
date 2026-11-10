#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_component_setpointMode_value;
/* Global Variable define of  */
extern bool sysml_state_component_setpointMode_previous;
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
extern bool sysml_state_heaterCommand;
/* Global Variable define of  */
extern double sysml_state_cachedTemperature;
/* Global Variable define of  */
extern bool sysml_state_hasReading;
/* Global Variable define of  */
extern bool sysml_state_previousOutside;
/* Global Variable define of  */
extern int sysml_state_outsideDuration;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern bool sysml_state_reportRaised;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool io_7, int io_9, bool* io_2, bool* io_3, int* io_4, int* io_5, bool* io_6, bool* io_8);
#endif
