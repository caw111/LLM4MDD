#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_initialized;
/* Global Variable define of  */
extern int sysml_state_displayNumber;
/* Global Variable define of  */
extern int sysml_state_previousTensDigit;
/* Global Variable define of  */
extern int sysml_state_previousUnitsDigit;
/* Global Variable define of  */
extern bool sysml_state_previousIncrementPressed;
/* Global Variable define of  */
extern bool sysml_state_previousDecrementPressed;
/* Global Variable define of  */
extern double sysml_state_incrementElapsed;
/* Global Variable define of  */
extern double sysml_state_decrementElapsed;
/* Global Variable define of  */
extern int sysml_state_incrementSteps;
/* Global Variable define of  */
extern int sysml_state_decrementSteps;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int io_5, int* io_4);
#endif
