#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_led1Brightness;
/* Global Variable define of  */
extern int sysml_state_led2Brightness;
/* Global Variable define of  */
extern int sysml_state_led3Brightness;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int sysml_state_hold1RemainingMs;
/* Global Variable define of  */
extern int sysml_state_hold2RemainingMs;
/* Global Variable define of  */
extern int sysml_state_hold3RemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, int io_9, double* io_0, double* io_1, double* io_2);
#endif
