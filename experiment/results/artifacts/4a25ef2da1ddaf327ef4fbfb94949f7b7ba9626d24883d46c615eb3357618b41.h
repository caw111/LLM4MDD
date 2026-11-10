#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_prevVehicleApproach;
/* Global Variable define of  */
extern bool sysml_state_prevPedApproach;
/* Global Variable define of  */
extern bool sysml_state_prevDanger;
/* Global Variable define of  */
extern bool sysml_state_dangerActive;
/* Global Variable define of  */
extern int sysml_state_dangerElapsedMs;
/* Global Variable define of  */
extern int sysml_state_dangerMaxElapsedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool* io_4, bool* io_5, int* io_6, int* io_7, bool* io_8, bool* serial_0_trigger, int* serial_0_value_0);
#endif
