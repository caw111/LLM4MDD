#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_servoAngle;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2;
/* Global Variable define of  */
extern bool sysml_state_movingLed;
/* Global Variable define of  */
extern double sysml_state_movingUntilAngle;
/* Global Variable define of  */
extern bool sysml_state_movingHoldActive;
/* Global Variable define of  */
extern int sysml_state_movingHoldMsRemaining;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, bool* io_0, double* io_3);
#endif
