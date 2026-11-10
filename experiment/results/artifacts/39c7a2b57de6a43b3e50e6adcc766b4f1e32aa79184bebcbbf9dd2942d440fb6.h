#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_previousPressed;
/* Global Variable define of  */
extern int sysml_state_displayMode;
/* Global Variable define of  */
extern int sysml_state_occupiedCount;
/* Global Variable define of  */
extern bool sysml_state_prevInPositionOccupied;
/* Global Variable define of  */
extern bool sysml_state_prevOccupied;
/* Global Variable define of  */
extern double sysml_state_entryDistanceCm;
/* Global Variable define of  */
extern double sysml_state_parkingDistanceCm;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool io_1, double io_2, bool io_3, bool io_8, int io_9, int* io_4, bool* io_5, bool* io_6, int* io_7);
#endif
