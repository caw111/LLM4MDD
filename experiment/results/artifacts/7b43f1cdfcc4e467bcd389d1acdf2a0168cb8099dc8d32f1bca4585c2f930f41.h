#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_pollAccumMs;
/* Global Variable define of  */
extern int sysml_state_distancePersistCount;
/* Global Variable define of  */
extern bool sysml_state_distanceDetected;
/* Global Variable define of  */
extern bool sysml_state_lastTemperatureValid;
/* Global Variable define of  */
extern bool sysml_state_lastDistanceValid;
/* Global Variable define of  */
extern bool sysml_state_prevDhtValid;
/* Global Variable define of  */
extern bool sysml_state_prevUltrasonicValid;
/* Global Variable define of  */
extern double sysml_state_lastTempSnapshot;
/* Global Variable define of  */
extern double sysml_state_lastDistSnapshot;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, bool io_3, double io_4, bool io_5, int io_6, bool* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, int* serial_0_value_2, bool* serial_1_trigger, bool* serial_1_value_0);
#endif
