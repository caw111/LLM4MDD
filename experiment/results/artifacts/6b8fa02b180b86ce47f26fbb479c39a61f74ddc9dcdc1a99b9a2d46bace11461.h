#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_dhtValid;
/* Global Variable define of  */
extern bool sysml_state_lastTempValid;
/* Global Variable define of  */
extern bool sysml_state_lastHumValid;
/* Global Variable define of  */
extern bool sysml_state_showTemp;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool* io_0, int* io_3, bool* serial_0_trigger, int* serial_0_value_0);
#endif
