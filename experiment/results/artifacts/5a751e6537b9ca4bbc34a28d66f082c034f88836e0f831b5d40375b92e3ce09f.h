#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_locked;
/* Global Variable define of  */
extern bool sysml_state_previousPressedAny;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP2;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP3;
/* Global Variable define of  */
extern bool sysml_state_previousPressedP4;
/* Global Variable define of  */
extern bool sysml_state_resetHeld;
/* Global Variable define of  */
extern bool sysml_state_resetLatch;
/* Global Variable define of  */
extern int sysml_state_playerId;
/* Global Variable define of  */
extern int sysml_state_buzzerTone;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int* io_6, int* io_7, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1);
#endif
