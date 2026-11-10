#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_component_repairRun0_value;
/* Global Variable define of  */
extern bool sysml_state_component_repairRun0_previous;
/* Global Variable define of  */
extern int sysml_state_component_repairPulse0_phase;
/* Global Variable define of  */
extern bool sysml_state_component_repairPulse0_started;
/* Global Variable define of  */
extern bool sysml_state_component_repairPulse0_enabled;
/* Global Variable define of  */
extern int sysml_state_component_repairPulse0_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_repairPulse0_value;
/* Global Variable define of  */
extern bool sysml_state_paused;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_2, int* io_1);
#endif
