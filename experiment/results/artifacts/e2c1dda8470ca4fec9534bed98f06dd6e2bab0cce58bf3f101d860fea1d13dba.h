#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_modeAuto;
/* Global Variable define of  */
extern bool sysml_state_manualTargetFan;
/* Global Variable define of  */
extern bool sysml_state_prevModeBtn;
/* Global Variable define of  */
extern bool sysml_state_prevTargetBtn;
/* Global Variable define of  */
extern bool sysml_state_manualFanActive;
/* Global Variable define of  */
extern bool sysml_state_manualLightActive;
/* Global Variable define of  */
extern bool sysml_state_manualPumpActive;
/* Global Variable define of  */
extern bool sysml_state_faultAlarmed;
/* Global Variable define of  */
extern bool sysml_state_prevFault;
/* Global Variable define of  */
extern int sysml_state_displayIndex;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, double io_4, double io_9, double io_10, bool io_11, double* io_5, bool* io_6, bool* io_7, int* io_8, int* io_12, bool* serial_0_trigger, bool* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2);
#endif
