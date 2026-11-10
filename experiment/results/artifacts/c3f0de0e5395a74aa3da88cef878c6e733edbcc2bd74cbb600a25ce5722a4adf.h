#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_doorOpenMs;
/* Global Variable define of  */
extern bool sysml_state_prevDoorOpen;
/* Global Variable define of  */
extern int sysml_state_doorOpenSeconds;
/* Global Variable define of  */
extern bool sysml_state_fanCoolRelayOn;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, bool* io_2, int* io_3, int* io_4, bool* io_5, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, int* serial_1_value_1);
#endif
