#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_servoAngleAccumMs;
/* Global Variable define of  */
extern double sysml_state_servoAngle1;
/* Global Variable define of  */
extern double sysml_state_servoAngle2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_8, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_9, bool* io_10, bool* io_11, bool* io_12, bool* io_13, bool* io_14);
#endif
