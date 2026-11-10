#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_currentAngleDeg;
/* Global Variable define of  */
extern int sysml_state_stableSecondsRemaining;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn2;
/* Global Variable define of  */
extern bool sysml_state_previousPressedBtn3;
/* Global Variable define of  */
extern double sysml_state_pendingAngleDeg;
/* Global Variable define of  */
extern int sysml_state_pendingLedCount;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9);
#endif
