#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_modeIsAuto;
/* Global Variable define of  */
extern bool sysml_state_manualTargetIsFan;
/* Global Variable define of  */
extern bool sysml_state_previousD2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousD3Pressed;
/* Global Variable define of  */
extern double sysml_state_fanPwmManual;
/* Global Variable define of  */
extern bool sysml_state_ledOnManual;
/* Global Variable define of  */
extern bool sysml_state_pumpOnManual;
/* Global Variable define of  */
extern int sysml_state_tmItemIndex;
/* Global Variable define of  */
extern int sysml_state_tmElapsedMs;
/* Global Variable define of  */
extern double sysml_state_lastTempValid;
/* Global Variable define of  */
extern double sysml_state_lastHumValid;
/* Global Variable define of  */
extern double sysml_state_lastIo3;
/* Global Variable define of  */
extern double sysml_state_prevSoilHumValid;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, double io_5, bool io_11, bool io_12, double* io_6, bool* io_7, bool* io_8, int* io_9, int* io_10, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_1_trigger, double* serial_1_value_0, double* serial_1_value_1, double* serial_1_value_2);
#endif
