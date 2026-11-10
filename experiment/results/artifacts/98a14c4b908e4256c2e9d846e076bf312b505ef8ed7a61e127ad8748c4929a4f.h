#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_prevIo7Pressed;
/* Global Variable define of  */
extern int sysml_state_countReusable;
/* Global Variable define of  */
extern int sysml_state_countOther;
/* Global Variable define of  */
extern bool sysml_state_alarmLatched;
/* Global Variable define of  */
extern bool sysml_state_prevFullState;
/* Global Variable define of  */
extern double sysml_state_lastValidDistance;
/* Global Variable define of  */
extern bool sysml_state_hasEverValidDistance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, bool io_3, bool io_7, int* io_4, int* io_5, bool* io_6, bool* serial_0_trigger, bool* serial_0_value_0);
#endif
