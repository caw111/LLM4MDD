#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_recyclableCount;
/* Global Variable define of  */
extern int sysml_state_otherCount;
/* Global Variable define of  */
extern int sysml_state_totalCount;
/* Global Variable define of  */
extern bool sysml_state_fullBinAlarmed;
/* Global Variable define of  */
extern bool sysml_state_previousResetPressed;
/* Global Variable define of  */
extern double sysml_state_fillHeightCmLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1);
#endif
