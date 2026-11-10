#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_modeManual;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2Pressed;
/* Global Variable define of  */
extern int sysml_state_debounceBtn1Remaining;
/* Global Variable define of  */
extern int sysml_state_debounceBtn2Remaining;
/* Global Variable define of  */
extern bool sysml_state_stableBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_stableBtn2Pressed;
/* Global Variable define of  */
extern int sysml_state_autoPhase;
/* Global Variable define of  */
extern int sysml_state_transitionElapsed;
/* Global Variable define of  */
extern bool sysml_state_toMax;
/* Global Variable define of  */
extern bool sysml_state_prevStableBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_prevStableBtn2Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_1, bool io_2, double io_3, double io_4, double io_5, int io_6, double* io_0);
#endif
