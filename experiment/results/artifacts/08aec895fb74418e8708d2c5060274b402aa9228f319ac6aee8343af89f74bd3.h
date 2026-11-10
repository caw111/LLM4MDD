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
extern int sysml_state_elapsedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int* io_6, bool* io_7_segment_0, bool* io_7_segment_1, bool* io_7_segment_2, bool* io_7_segment_3, bool* io_7_segment_4, bool* io_7_segment_5, bool* io_7_segment_6, bool* io_7_segment_7);
#endif
