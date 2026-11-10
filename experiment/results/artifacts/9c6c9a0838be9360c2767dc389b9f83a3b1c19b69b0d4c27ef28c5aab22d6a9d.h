#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_measuredCm_i32;
/* Global Variable define of  */
extern int sysml_state_targetCm_i32;
/* Global Variable define of  */
extern int sysml_state_comparisonResultCode_i32;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, int io_2, bool io_2_valid, bool* io_3, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2);
#endif
