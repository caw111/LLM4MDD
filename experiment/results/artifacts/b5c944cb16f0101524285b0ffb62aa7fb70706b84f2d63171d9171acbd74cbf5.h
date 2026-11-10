#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_phase;
/* Global Variable define of  */
extern int sysml_state_remainingMs;
/* Global Variable define of  */
extern int sysml_state_roundNumber;
/* Global Variable define of  */
extern int sysml_state_score;
/* Global Variable define of  */
extern int sysml_state_selectedColor;
/* Global Variable define of  */
extern int sysml_state_randomState;
/* Global Variable define of  */
extern bool sysml_state_seenRed;
/* Global Variable define of  */
extern bool sysml_state_seenYellow;
/* Global Variable define of  */
extern bool sysml_state_seenGreen;
/* Global Variable define of  */
extern bool sysml_state_previousStop;
/* Global Variable define of  */
extern bool sysml_state_previousBack;
/* Global Variable define of  */
extern bool sysml_state_previousForward;
/* Global Variable define of  */
extern bool sysml_state_started;
/* Global Variable define of  */
extern bool sysml_state_readyReport;
/* Global Variable define of  */
extern bool sysml_state_wrongReport;
/* Global Variable define of  */
extern bool sysml_state_timeoutReport;
/* Global Variable define of  */
extern bool sysml_state_successReport;
/* Global Variable define of  */
extern bool sysml_state_endReport;
/* Global Variable define of  */
extern bool sysml_state_beginRound;
/* Global Variable define of  */
extern bool sysml_state_finishRound;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, bool io_5, int io_7, bool* io_0, bool* io_1, bool* io_2, int* io_6, bool* io_8, bool* io_9, bool* io_10, bool* serial_0_trigger, int* serial_0_value_0, bool* serial_1_trigger, int* serial_1_value_0);
#endif
