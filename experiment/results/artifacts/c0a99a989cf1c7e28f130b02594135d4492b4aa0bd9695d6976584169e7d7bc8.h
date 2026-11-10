#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_component_run_value;
/* Global Variable define of  */
extern bool sysml_state_component_run_previous;
/* Global Variable define of  */
extern int sysml_state_component_blink_phase;
/* Global Variable define of  */
extern bool sysml_state_component_blink_started;
/* Global Variable define of  */
extern bool sysml_state_component_blink_enabled;
/* Global Variable define of  */
extern int sysml_state_component_blink_remainder;
/* Global Variable define of  */
extern bool sysml_state_component_blink_value;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int io_2, bool* io_1);
#endif
