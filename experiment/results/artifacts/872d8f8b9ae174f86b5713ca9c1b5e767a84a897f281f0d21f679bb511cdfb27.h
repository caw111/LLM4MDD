#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_currentFloor;
/* Global Variable define of  */
extern int sysml_state_targetFloor;
/* Global Variable define of  */
extern bool sysml_state_pendingMove;
/* Global Variable define of  */
extern int sysml_state_moveRemainingMs;
/* Global Variable define of  */
extern bool sysml_state_previousMoveRequest;
/* Global Variable define of  */
extern bool sysml_state_movementCompletePulse;
/* Global Variable define of  */
extern bool sysml_state_previousPressedAny;
/* Global Variable define of  */
extern int sysml_state_loadPercent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int* io_4, bool* io_5, int* io_6, bool* serial_0_trigger, int* serial_0_value_0);
#endif
