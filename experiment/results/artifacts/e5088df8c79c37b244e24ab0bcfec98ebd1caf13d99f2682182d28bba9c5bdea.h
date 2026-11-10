#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2;
/* Global Variable define of  */
extern int sysml_state_mode;
/* Global Variable define of  */
extern int sysml_state_maintenanceElapsedMs;
/* Global Variable define of  */
extern int sysml_state_flashAccumMs;
/* Global Variable define of  */
extern bool sysml_state_ledFlashOn;
/* Global Variable define of  */
extern int sysml_state_btn1HoldMs;
/* Global Variable define of  */
extern int sysml_state_btn2HoldMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int io_5, double* io_0, double* io_1, bool* io_2);
#endif
