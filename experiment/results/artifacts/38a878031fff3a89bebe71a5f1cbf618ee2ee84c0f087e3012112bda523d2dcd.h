#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_vacantTimeOne;
/* Global Variable define of  */
extern int sysml_state_vacantTimeTwo;
/* Global Variable define of  */
extern int sysml_state_vacantTimeThree;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedOne;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedTwo;
/* Global Variable define of  */
extern bool sysml_state_previousOccupiedThree;
/* Global Variable define of  */
extern bool sysml_state_reportedOne;
/* Global Variable define of  */
extern bool sysml_state_reportedTwo;
/* Global Variable define of  */
extern bool sysml_state_reportedThree;
/* Global Variable define of  */
extern bool sysml_state_restockReport;
/* Global Variable define of  */
extern bool sysml_state_recordsLocked;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int io_6, int io_7, bool io_7_valid, int* io_3, int* io_4, bool* io_5, bool* io_8, bool* serial_0_trigger, bool* serial_0_value_0, bool* serial_0_value_1, bool* serial_0_value_2);
#endif
