#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_dummyInitOff;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, bool* io_8_segment_0, bool* io_8_segment_1, bool* io_8_segment_2, bool* io_8_segment_3, bool* io_8_segment_4, bool* io_8_segment_5, bool* io_8_segment_6, bool* io_8_segment_7);
#endif
