#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_activeAngleDeg;
/* Global Variable define of  */
extern int sysml_state_activeLedLevel;
/* Global Variable define of  */
extern int sysml_state_holdElapsedMs;
/* Global Variable define of  */
extern bool sysml_state_btn1PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn2PrevPressed;
/* Global Variable define of  */
extern bool sysml_state_btn3PrevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9, double* io_4);
#endif
