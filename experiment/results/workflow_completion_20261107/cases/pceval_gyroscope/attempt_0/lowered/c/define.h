#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_readySent;
/* Global Variable define of  */
extern bool sysml_state_readyPulse;
/* Global Variable define of  */
extern bool sysml_state_reportPulse;
/* Global Variable define of  */
extern double sysml_state_reportPhase;
/* Global Variable define of  */
extern double sysml_state_cachedX;
/* Global Variable define of  */
extern double sysml_state_cachedY;
/* Global Variable define of  */
extern double sysml_state_cachedZ;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool io_3, int io_5, bool* io_4, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif
