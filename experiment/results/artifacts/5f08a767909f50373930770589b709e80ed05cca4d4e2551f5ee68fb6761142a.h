#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_modeAuto;
/* Global Variable define of  */
extern bool sysml_state_prevButton;
/* Global Variable define of  */
extern int sysml_state_manualForcedUntil;
/* Global Variable define of  */
extern int sysml_state_manualRemainingMs;
/* Global Variable define of  */
extern int sysml_state_displayPhase;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_7, bool* io_3, bool* io_4, int* io_5, int* io_6);
#endif
