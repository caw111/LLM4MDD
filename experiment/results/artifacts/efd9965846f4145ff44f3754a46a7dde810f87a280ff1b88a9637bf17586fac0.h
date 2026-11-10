#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_redIntensity;
/* Global Variable define of  */
extern int sysml_state_greenIntensity;
/* Global Variable define of  */
extern int sysml_state_blueIntensity;
/* Global Variable define of  */
extern int sysml_state_selectedColor;
/* Global Variable define of  */
extern bool sysml_state_previousBtn1Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn2Pressed;
/* Global Variable define of  */
extern bool sysml_state_previousBtn3Pressed;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double* io_3, double* io_4, double* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7);
#endif
