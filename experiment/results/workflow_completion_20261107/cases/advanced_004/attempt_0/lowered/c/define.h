#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern bool sysml_state_clockReady;
/* Global Variable define of  */
extern int sysml_state_clockMs;
/* Global Variable define of  */
extern int sysml_state_oldClockMs;
/* Global Variable define of  */
extern int sysml_state_alarmOne;
/* Global Variable define of  */
extern int sysml_state_alarmTwo;
/* Global Variable define of  */
extern int sysml_state_alarmThree;
/* Global Variable define of  */
extern bool sysml_state_firedOne;
/* Global Variable define of  */
extern bool sysml_state_firedTwo;
/* Global Variable define of  */
extern bool sysml_state_firedThree;
/* Global Variable define of  */
extern bool sysml_state_activeTodayOne;
/* Global Variable define of  */
extern bool sysml_state_activeTodayTwo;
/* Global Variable define of  */
extern bool sysml_state_activeTodayThree;
/* Global Variable define of  */
extern int sysml_state_statusOne;
/* Global Variable define of  */
extern int sysml_state_statusTwo;
/* Global Variable define of  */
extern int sysml_state_statusThree;
/* Global Variable define of  */
extern int sysml_state_ageOne;
/* Global Variable define of  */
extern int sysml_state_ageTwo;
/* Global Variable define of  */
extern int sysml_state_ageThree;
/* Global Variable define of  */
extern int sysml_state_occurrencesOne;
/* Global Variable define of  */
extern int sysml_state_occurrencesTwo;
/* Global Variable define of  */
extern int sysml_state_occurrencesThree;
/* Global Variable define of  */
extern int sysml_state_batchMisses;
/* Global Variable define of  */
extern int sysml_state_missedCount;
/* Global Variable define of  */
extern int sysml_state_pendingWarnings;
/* Global Variable define of  */
extern bool sysml_state_warningStep;
/* Global Variable define of  */
extern int sysml_state_displayPage;
/* Global Variable define of  */
extern bool sysml_state_confirmArmed;
/* Global Variable define of  */
extern bool sysml_state_previousArmed;
/* Global Variable define of  */
extern bool sysml_state_nextArmed;
/* Global Variable define of  */
extern int sysml_state_confirmReleaseMs;
/* Global Variable define of  */
extern int sysml_state_previousReleaseMs;
/* Global Variable define of  */
extern int sysml_state_nextReleaseMs;
/* Global Variable define of  */
extern bool sysml_state_confirmPress;
/* Global Variable define of  */
extern bool sysml_state_previousPress;
/* Global Variable define of  */
extern bool sysml_state_nextPress;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_0_valid, bool io_1, bool io_2, bool io_3, int io_8, int* io_4, bool* io_5, int* io_6, bool* io_7);
#endif
