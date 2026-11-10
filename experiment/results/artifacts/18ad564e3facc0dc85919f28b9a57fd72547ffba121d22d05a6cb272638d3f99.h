#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_totalCount;
/* Global Variable define of  */
extern int sysml_state_stopCount;
/* Global Variable define of  */
extern bool sysml_state_prevD2;
/* Global Variable define of  */
extern bool sysml_state_prevD3;
/* Global Variable define of  */
extern bool sysml_state_prevD7;
/* Global Variable define of  */
extern int sysml_state_noProductAccumMs;
/* Global Variable define of  */
extern bool sysml_state_stopActive;
/* Global Variable define of  */
extern bool sysml_state_alarmActive;
/* Global Variable define of  */
extern bool sysml_state_alarmReported;
/* Global Variable define of  */
extern int sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_serialGate;
/* Global Variable define of  */
extern int sysml_state_serialTotalCountI;
/* Global Variable define of  */
extern int sysml_state_serialStopCountI;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2);
#endif
