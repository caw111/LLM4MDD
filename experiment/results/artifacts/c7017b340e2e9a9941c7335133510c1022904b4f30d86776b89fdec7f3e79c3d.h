#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_prevPot;
/* Global Variable define of  */
extern bool sysml_state_servoHold1;
/* Global Variable define of  */
extern bool sysml_state_servoHold2;
/* Global Variable define of  */
extern bool sysml_state_servoHold3;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_7, double* io_8, double* io_9);
#endif
