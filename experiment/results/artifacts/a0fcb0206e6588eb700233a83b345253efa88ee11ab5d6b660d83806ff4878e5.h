#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_mode1Index;
/* Global Variable define of  */
extern int sysml_state_mode2Index;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn2;
/* Global Variable define of  */
extern bool sysml_state_isSecondMode;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double* io_3, double* io_4, double* io_5);
#endif
