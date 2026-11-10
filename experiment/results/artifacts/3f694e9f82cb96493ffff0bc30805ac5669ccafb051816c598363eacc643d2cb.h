#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousAllFull;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, bool* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2, bool* serial_0_value_3, int* serial_0_value_4, bool* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7);
#endif
