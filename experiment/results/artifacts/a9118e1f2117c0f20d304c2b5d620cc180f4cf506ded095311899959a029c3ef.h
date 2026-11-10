#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_mode;
/* Global Variable define of  */
extern int sysml_state_occupancyCount;
/* Global Variable define of  */
extern bool sysml_state_prevParkingValid;
/* Global Variable define of  */
extern bool sysml_state_prevEntranceApproach;
/* Global Variable define of  */
extern bool sysml_state_prevReverseAlarmCond;
/* Global Variable define of  */
extern bool sysml_state_lastAlarmOn;
/* Global Variable define of  */
extern bool sysml_state_prevPressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, double io_4, bool io_5, double io_6, bool io_7, bool* io_1, bool* io_2, int* io_3, int* io_8);
#endif
