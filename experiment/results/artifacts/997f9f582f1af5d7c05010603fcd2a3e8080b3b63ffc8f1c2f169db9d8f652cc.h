#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_component_beep_phase;
/* Global Variable define of  */
extern bool sysml_state_component_beep_started;
/* Global Variable define of  */
extern bool sysml_state_component_beep_enabled;
/* Global Variable define of  */
extern int sysml_state_component_beep_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_beep_value;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, int* io_1);
#endif
