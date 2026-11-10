#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_count;
/* Global Variable define of  */
extern bool sysml_state_prevBtn0;
/* Global Variable define of  */
extern bool sysml_state_prevBtn1;
/* Global Variable define of  */
extern bool sysml_state_prevBtn2;
/* Global Variable define of  */
extern bool sysml_state_debounceStable0;
/* Global Variable define of  */
extern bool sysml_state_debounceStable1;
/* Global Variable define of  */
extern bool sysml_state_debounceStable2;
/* Global Variable define of  */
extern bool sysml_state_debounceCand0;
/* Global Variable define of  */
extern bool sysml_state_debounceCand1;
/* Global Variable define of  */
extern bool sysml_state_debounceCand2;
/* Global Variable define of  */
extern int sysml_state_debounceCnt0;
/* Global Variable define of  */
extern int sysml_state_debounceCnt1;
/* Global Variable define of  */
extern int sysml_state_debounceCnt2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, int* io_9);
#endif
