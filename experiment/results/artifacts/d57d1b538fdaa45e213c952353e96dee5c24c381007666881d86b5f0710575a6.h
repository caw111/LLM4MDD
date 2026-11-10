#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousPressed1;
/* Global Variable define of  */
extern bool sysml_state_previousPressed2;
/* Global Variable define of  */
extern bool sysml_state_previousPressed3;
/* Global Variable define of  */
extern bool sysml_state_previousPressed4;
/* Global Variable define of  */
extern bool sysml_state_locked;
/* Global Variable define of  */
extern int sysml_state_winnerId;
/* Global Variable define of  */
extern int sysml_state_resetAccumulatedMs;
/* Global Variable define of  */
extern bool sysml_state_resetting;
/* Global Variable define of  */
extern bool sysml_state_resetRequest;
/* Global Variable define of  */
extern bool sysml_state_serialTriggerNext;
/* Global Variable define of  */
extern int sysml_state_serialTimestampMsNext;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int* io_6, int* io_7, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1);
#endif
