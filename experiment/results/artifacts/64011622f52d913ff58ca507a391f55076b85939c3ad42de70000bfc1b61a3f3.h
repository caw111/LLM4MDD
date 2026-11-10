#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_servoHoldAccumMs1;
/* Global Variable define of  */
extern int sysml_state_servoHoldAccumMs2;
/* Global Variable define of  */
extern int sysml_state_servoHoldAccumMs3;
/* Global Variable define of  */
extern double sysml_state_servoAngle1;
/* Global Variable define of  */
extern double sysml_state_servoAngle2;
/* Global Variable define of  */
extern double sysml_state_servoAngle3;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, int io_10, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9);
#endif
