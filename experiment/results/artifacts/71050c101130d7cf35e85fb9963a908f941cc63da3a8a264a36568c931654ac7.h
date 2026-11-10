#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_led1State;
/* Global Variable define of  */
extern bool sysml_state_led2State;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_prevBtn3;
/* Global Variable define of  */
extern int sysml_state_debounceBtn1Ms;
/* Global Variable define of  */
extern int sysml_state_debounceBtn2Ms;
/* Global Variable define of  */
extern int sysml_state_debounceBtn3Ms;
/* Global Variable define of  */
extern bool sysml_state_stableBtn1;
/* Global Variable define of  */
extern bool sysml_state_stableBtn2;
/* Global Variable define of  */
extern bool sysml_state_stableBtn3;
/* Global Variable define of  */
extern int sysml_state_holdBtn1RemainingMs;
/* Global Variable define of  */
extern int sysml_state_holdBtn2RemainingMs;
/* Global Variable define of  */
extern int sysml_state_holdBtn3RemainingMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int io_5, bool* io_0, bool* io_1);
#endif
