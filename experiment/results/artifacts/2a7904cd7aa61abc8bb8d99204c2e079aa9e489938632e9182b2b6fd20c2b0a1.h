#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_servo1Deg;
/* Global Variable define of  */
extern double sysml_state_servo2Deg;
/* Global Variable define of  */
extern double sysml_state_servo3Deg;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, bool* io_5_segment_0, bool* io_5_segment_1, bool* io_5_segment_2, bool* io_5_segment_3, bool* io_5_segment_4, bool* io_5_segment_5, bool* io_5_segment_6, bool* io_5_segment_7, bool* io_5_segment_8, bool* io_5_segment_9);
#endif
