#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_elapsedTotalMs;
/* Global Variable define of  */
extern int sysml_state_rainbowIndex;
/* Global Variable define of  */
extern bool sysml_state_ledOn;
/* Global Variable define of  */
extern bool sysml_state_servo1At180;
/* Global Variable define of  */
extern bool sysml_state_servo2At0;
/* Global Variable define of  */
extern bool sysml_state_servo3At135;
/* Global Variable define of  */
extern bool sysml_state_phaseStepDone;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10);
#endif
