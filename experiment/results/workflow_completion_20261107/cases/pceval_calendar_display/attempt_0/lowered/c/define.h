#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_cachedEpoch;
/* Global Variable define of  */
extern bool sysml_state_hasReading;
/* Global Variable define of  */
extern int sysml_state_dayOffset;
/* Global Variable define of  */
extern bool sysml_state_previousBackPressed;
/* Global Variable define of  */
extern bool sysml_state_previousForwardPressed;
/* Global Variable define of  */
extern bool sysml_state_backPressEvent;
/* Global Variable define of  */
extern bool sysml_state_forwardPressEvent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, bool io_2, bool io_3, int* io_4, int* io_5);
#endif
