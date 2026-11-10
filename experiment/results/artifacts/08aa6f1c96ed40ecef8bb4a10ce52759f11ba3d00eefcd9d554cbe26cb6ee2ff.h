#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_roomIndex;
/* Global Variable define of  */
extern int sysml_state_displayMode;
/* Global Variable define of  */
extern bool sysml_state_prevBtnRoom;
/* Global Variable define of  */
extern bool sysml_state_prevBtnMode;
/* Global Variable define of  */
extern int sysml_state_serialAccumMs;
/* Global Variable define of  */
extern bool sysml_state_nextSerialReady;
/* Global Variable define of  */
extern int sysml_state_tm1637NumberLatched;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, double io_3, bool io_4, double io_5, double io_6, bool io_7, bool io_8, bool io_9, int io_13, bool* io_10, bool* io_11, int* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_0_value_6, bool* serial_0_value_7);
#endif
