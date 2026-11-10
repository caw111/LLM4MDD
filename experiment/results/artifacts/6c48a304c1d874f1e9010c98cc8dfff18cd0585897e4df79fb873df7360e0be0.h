#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_armed;
/* Global Variable define of  */
extern bool sysml_state_nightAuto;
/* Global Variable define of  */
extern int sysml_state_eventCount;
/* Global Variable define of  */
extern bool sysml_state_previousDoorOpened;
/* Global Variable define of  */
extern bool sysml_state_previousPIR;
/* Global Variable define of  */
extern bool sysml_state_serialCommandActive;
/* Global Variable define of  */
extern int sysml_state_pendingReportCmd;
/* Global Variable define of  */
extern bool sysml_state_doorRising;
/* Global Variable define of  */
extern bool sysml_state_pirRising;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, int* io_3, bool* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0);
#endif
