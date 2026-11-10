#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_btn1PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn2PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn3PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn1StablePressed;
/* Global Variable define of  */
extern bool sysml_state_btn2StablePressed;
/* Global Variable define of  */
extern bool sysml_state_btn3StablePressed;
/* Global Variable define of  */
extern int sysml_state_btn1StableTimerMs;
/* Global Variable define of  */
extern int sysml_state_btn2StableTimerMs;
/* Global Variable define of  */
extern int sysml_state_btn3StableTimerMs;
/* Global Variable define of  */
extern bool sysml_state_valueBit0;
/* Global Variable define of  */
extern bool sysml_state_valueBit1;
/* Global Variable define of  */
extern bool sysml_state_valueBit2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif
