#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern double sysml_state_led1Brightness;
/* Global Variable define of  */
extern double sysml_state_led2Brightness;
/* Global Variable define of  */
extern double sysml_state_led3Brightness;
/* Global Variable define of  */
extern int sysml_state_btn1StableUntil;
/* Global Variable define of  */
extern int sysml_state_btn2StableUntil;
/* Global Variable define of  */
extern int sysml_state_btn3StableUntil;
/* Global Variable define of  */
extern int sysml_state_stableTimer;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn3Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2);
#endif
