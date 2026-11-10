#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int sysml_state_stockOne;
/* Global Variable define of  */
extern int sysml_state_stockTwo;
/* Global Variable define of  */
extern int sysml_state_stockThree;
/* Global Variable define of  */
extern int sysml_state_priceOne;
/* Global Variable define of  */
extern int sysml_state_priceTwo;
/* Global Variable define of  */
extern int sysml_state_priceThree;
/* Global Variable define of  */
extern int sysml_state_selectedItem;
/* Global Variable define of  */
extern int sysml_state_displayPhase;
/* Global Variable define of  */
extern bool sysml_state_displayError;
/* Global Variable define of  */
extern bool sysml_state_adminMode;
/* Global Variable define of  */
extern bool sysml_state_previousOne;
/* Global Variable define of  */
extern bool sysml_state_previousTwo;
/* Global Variable define of  */
extern bool sysml_state_previousThree;
/* Global Variable define of  */
extern bool sysml_state_previousConfirm;
/* Global Variable define of  */
extern int sysml_state_holdDuration;
/* Global Variable define of  */
extern bool sysml_state_holdHandled;
/* Global Variable define of  */
extern int sysml_state_beepRemaining;
/* Global Variable define of  */
extern int sysml_state_beepFrequency;
/* Global Variable define of  */
extern int sysml_state_saleItem;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int io_7, bool io_7_valid, int io_11, bool* io_4, int* io_5, int* io_6, bool* io_8, bool* io_9, bool* io_10);
#endif
