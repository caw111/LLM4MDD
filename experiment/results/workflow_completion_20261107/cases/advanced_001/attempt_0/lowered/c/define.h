#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_bootComplete;
/* Global Variable define of  */
extern bool sysml_state_operational;
/* Global Variable define of  */
extern bool sysml_state_automaticMode;
/* Global Variable define of  */
extern int sysml_state_selectedActuator;
/* Global Variable define of  */
extern double sysml_state_fanCommand;
/* Global Variable define of  */
extern bool sysml_state_lightCommand;
/* Global Variable define of  */
extern bool sysml_state_pumpCommand;
/* Global Variable define of  */
extern double sysml_state_savedTemperature;
/* Global Variable define of  */
extern bool sysml_state_temperatureAvailable;
/* Global Variable define of  */
extern bool sysml_state_modeCandidate;
/* Global Variable define of  */
extern bool sysml_state_modeStable;
/* Global Variable define of  */
extern int sysml_state_modeStableMs;
/* Global Variable define of  */
extern bool sysml_state_modePressEvent;
/* Global Variable define of  */
extern bool sysml_state_selectCandidate;
/* Global Variable define of  */
extern bool sysml_state_selectStable;
/* Global Variable define of  */
extern int sysml_state_selectStableMs;
/* Global Variable define of  */
extern bool sysml_state_selectPressEvent;
/* Global Variable define of  */
extern int sysml_state_displayPhase;
/* Global Variable define of  */
extern int sysml_state_displayDelta;
/* Global Variable define of  */
extern bool sysml_state_previousDhtFault;
/* Global Variable define of  */
extern bool sysml_state_previousLightFault;
/* Global Variable define of  */
extern bool sysml_state_previousSoilFault;
/* Global Variable define of  */
extern bool sysml_state_reportEvent;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, double io_3, double io_4, bool io_8, bool io_9, int io_12, double* io_5, bool* io_6, bool* io_7, int* io_10, int* io_11, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2);
#endif
