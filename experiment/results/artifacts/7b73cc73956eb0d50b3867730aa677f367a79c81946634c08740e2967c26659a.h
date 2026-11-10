#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_displayRoomIndex;
/* Global Variable define of  */
extern int sysml_state_displayModeIndex;
/* Global Variable define of  */
extern int sysml_state_reportAccumMs;
/* Global Variable define of  */
extern bool sysml_state_prevButton1;
/* Global Variable define of  */
extern bool sysml_state_prevButton2;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, double io_3, double io_4, bool io_5, double io_6, double io_7, bool io_8, bool io_9, int io_13, bool* io_10, bool* io_11, int* io_12, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3, double* serial_0_value_4, double* serial_0_value_5, bool* serial_1_trigger, bool* serial_1_value_0, bool* serial_1_value_1);
#endif
