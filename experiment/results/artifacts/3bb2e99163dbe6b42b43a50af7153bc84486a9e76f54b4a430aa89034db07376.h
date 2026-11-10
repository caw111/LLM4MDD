#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_servo1_angle_deg;
/* Global Variable define of  */
extern double sysml_state_servo2_angle_deg;
/* Global Variable define of  */
extern int sysml_state_display_value_i32;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, int io_5, int* io_0, double* io_3, double* io_4);
#endif
