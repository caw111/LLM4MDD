#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_colorIndex;
/* Global Variable define of  */
extern bool sysml_state_debouncedBtn1;
/* Global Variable define of  */
extern bool sysml_state_debouncedBtn2;
/* Global Variable define of  */
extern bool sysml_state_previousDebouncedBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousDebouncedBtn2;
/* Global Variable define of  */
extern int sysml_state_stableCounter1;
/* Global Variable define of  */
extern int sysml_state_stableCounter2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool* io_2, bool* io_3, bool* io_4);
#endif
