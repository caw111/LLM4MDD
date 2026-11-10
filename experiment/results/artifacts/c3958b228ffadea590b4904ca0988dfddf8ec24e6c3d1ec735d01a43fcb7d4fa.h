#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_pageIndex;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern double sysml_state_dhtTempLastValid;
/* Global Variable define of  */
extern double sysml_state_dhtHumLastValid;
/* Global Variable define of  */
extern int sysml_state_reportAccumMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, bool io_4, int io_6, int* io_5, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif
