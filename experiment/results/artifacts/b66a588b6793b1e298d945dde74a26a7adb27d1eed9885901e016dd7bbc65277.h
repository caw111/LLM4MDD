#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_counterValue;
/* Global Variable define of  */
extern int sysml_state_repeatAge;
/* Global Variable define of  */
extern bool sysml_state_previousIncrement;
/* Global Variable define of  */
extern bool sysml_state_previousDecrement;
/* Global Variable define of  */
extern bool sysml_state_previousReset;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int io_5, int* io_4);
#endif
