#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_led1State = false;
/* Global Variable define of  */
bool sysml_state_led2State = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int sysml_state_debounceBtn1Ms = 0;
/* Global Variable define of  */
int sysml_state_debounceBtn2Ms = 0;
/* Global Variable define of  */
int sysml_state_debounceBtn3Ms = 0;
/* Global Variable define of  */
bool sysml_state_stableBtn1 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn2 = false;
/* Global Variable define of  */
bool sysml_state_stableBtn3 = false;
/* Global Variable define of  */
int sysml_state_holdBtn1RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_holdBtn2RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_holdBtn3RemainingMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1State = false;
    sysml_state_led2State = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_debounceBtn1Ms = 0;
    sysml_state_debounceBtn2Ms = 0;
    sysml_state_debounceBtn3Ms = 0;
    sysml_state_stableBtn1 = false;
    sysml_state_stableBtn2 = false;
    sysml_state_stableBtn3 = false;
    sysml_state_holdBtn1RemainingMs = 0;
    sysml_state_holdBtn2RemainingMs = 0;
    sysml_state_holdBtn3RemainingMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int io_5, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    if (io_2) {
        sysml_conditional_0 = sysml_state_debounceBtn1Ms + io_5;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_state_debounceBtn1Ms = sysml_conditional_0;
    if (io_2) {
        sysml_conditional_1 = sysml_state_debounceBtn1Ms >= 20;
    }
    else {
        sysml_conditional_1 = false;
    }
    sysml_state_stableBtn1 = sysml_conditional_1;
    if (io_3) {
        sysml_conditional_2 = sysml_state_debounceBtn2Ms + io_5;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_state_debounceBtn2Ms = sysml_conditional_2;
    if (io_3) {
        sysml_conditional_3 = sysml_state_debounceBtn2Ms >= 20;
    }
    else {
        sysml_conditional_3 = false;
    }
    sysml_state_stableBtn2 = sysml_conditional_3;
    if (io_4) {
        sysml_conditional_4 = sysml_state_debounceBtn3Ms + io_5;
    }
    else {
        sysml_conditional_4 = 0;
    }
    sysml_state_debounceBtn3Ms = sysml_conditional_4;
    if (io_4) {
        sysml_conditional_5 = sysml_state_debounceBtn3Ms >= 20;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_state_stableBtn3 = sysml_conditional_5;
    if (sysml_state_holdBtn1RemainingMs > 0) {
        sysml_conditional_6 = sysml_state_holdBtn1RemainingMs - io_5;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_state_holdBtn1RemainingMs = sysml_conditional_6;
    if (sysml_state_holdBtn2RemainingMs > 0) {
        sysml_conditional_7 = sysml_state_holdBtn2RemainingMs - io_5;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_state_holdBtn2RemainingMs = sysml_conditional_7;
    if (sysml_state_holdBtn3RemainingMs > 0) {
        sysml_conditional_8 = sysml_state_holdBtn3RemainingMs - io_5;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_state_holdBtn3RemainingMs = sysml_conditional_8;
    sysml_lazy_9 = sysml_state_stableBtn3;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_prevBtn3;
    }
    if (sysml_lazy_9) {
        sysml_state_led1State = false;
        sysml_state_led2State = false;
        sysml_state_holdBtn3RemainingMs = 2000;
    }
    sysml_lazy_10 = !(sysml_state_holdBtn3RemainingMs > 0);
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_stableBtn1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_11) {
        sysml_state_led1State = (!sysml_state_led1State);
        sysml_state_holdBtn1RemainingMs = 2000;
    }
    sysml_lazy_12 = !(sysml_state_holdBtn3RemainingMs > 0);
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_stableBtn2;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_13) {
        sysml_state_led2State = (!sysml_state_led2State);
        sysml_state_holdBtn2RemainingMs = 2000;
    }
    if ((sysml_state_holdBtn3RemainingMs > 0)) {
        sysml_state_led1State = false;
        sysml_state_led2State = false;
    }
    if ((sysml_state_prevBtn1 != sysml_state_stableBtn1)) {
        sysml_state_prevBtn1 = sysml_state_stableBtn1;
    }
    else {
        sysml_state_prevBtn1 = sysml_state_stableBtn1;
    }
    if ((sysml_state_prevBtn2 != sysml_state_stableBtn2)) {
        sysml_state_prevBtn2 = sysml_state_stableBtn2;
    }
    else {
        sysml_state_prevBtn2 = sysml_state_stableBtn2;
    }
    if ((sysml_state_prevBtn3 != sysml_state_stableBtn3)) {
        sysml_state_prevBtn3 = sysml_state_stableBtn3;
    }
    else {
        sysml_state_prevBtn3 = sysml_state_stableBtn3;
    }
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1State;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2State;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}