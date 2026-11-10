#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_led1Stable = false;
/* Global Variable define of  */
bool sysml_state_led2Stable = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int sysml_state_btn1DebounceMs = 0;
/* Global Variable define of  */
int sysml_state_btn2DebounceMs = 0;
/* Global Variable define of  */
int sysml_state_btn3DebounceMs = 0;
/* Global Variable define of  */
int sysml_state_btn1HoldMs = 0;
/* Global Variable define of  */
int sysml_state_btn2HoldMs = 0;
/* Global Variable define of  */
int sysml_state_btn3HoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led1Stable = false;
    sysml_state_led2Stable = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_btn1DebounceMs = 0;
    sysml_state_btn2DebounceMs = 0;
    sysml_state_btn3DebounceMs = 0;
    sysml_state_btn1HoldMs = 0;
    sysml_state_btn2HoldMs = 0;
    sysml_state_btn3HoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, bool io_4, int io_5, bool* io_0, bool* io_1) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_conditional_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    if (io_2) {
        sysml_conditional_0 = true;
    }
    else {
        sysml_conditional_0 = false;
    }
    if (sysml_conditional_0) {
        sysml_state_btn1DebounceMs = (sysml_state_btn1DebounceMs + io_5);
    }
    else {
        sysml_state_btn1DebounceMs = 0;
    }
    if (io_3) {
        sysml_conditional_1 = true;
    }
    else {
        sysml_conditional_1 = false;
    }
    if (sysml_conditional_1) {
        sysml_state_btn2DebounceMs = (sysml_state_btn2DebounceMs + io_5);
    }
    else {
        sysml_state_btn2DebounceMs = 0;
    }
    if (io_4) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    if (sysml_conditional_2) {
        sysml_state_btn3DebounceMs = (sysml_state_btn3DebounceMs + io_5);
    }
    else {
        sysml_state_btn3DebounceMs = 0;
    }
    if ((sysml_state_btn1HoldMs >= 1)) {
        sysml_state_btn1HoldMs = (sysml_state_btn1HoldMs - io_5);
    }
    else {
        sysml_state_btn1HoldMs = 0;
    }
    if ((sysml_state_btn2HoldMs >= 1)) {
        sysml_state_btn2HoldMs = (sysml_state_btn2HoldMs - io_5);
    }
    else {
        sysml_state_btn2HoldMs = 0;
    }
    if ((sysml_state_btn3HoldMs >= 1)) {
        sysml_state_btn3HoldMs = (sysml_state_btn3HoldMs - io_5);
    }
    else {
        sysml_state_btn3HoldMs = 0;
    }
    sysml_lazy_3 = io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevBtn1;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = sysml_state_btn1DebounceMs >= 20;
    }
    if (sysml_lazy_4) {
        sysml_state_led1Stable = (!sysml_state_led1Stable);
        sysml_state_btn1HoldMs = 2000;
    }
    sysml_lazy_5 = io_3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_prevBtn2;
    }
    sysml_lazy_6 = sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_btn2DebounceMs >= 20;
    }
    if (sysml_lazy_6) {
        sysml_state_led2Stable = (!sysml_state_led2Stable);
        sysml_state_btn2HoldMs = 2000;
    }
    sysml_lazy_7 = io_4;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevBtn3;
    }
    sysml_lazy_8 = sysml_lazy_7;
    if (sysml_lazy_8) {
        sysml_lazy_8 = sysml_state_btn3DebounceMs >= 20;
    }
    if (sysml_lazy_8) {
        sysml_state_led1Stable = false;
        sysml_state_led2Stable = false;
        sysml_state_btn3HoldMs = 2000;
    }
    if ((sysml_state_btn3HoldMs >= 1)) {
        sysml_state_led1Stable = false;
        sysml_state_led2Stable = false;
    }
    sysml_lazy_9 = sysml_state_btn1HoldMs >= 1;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_btn3HoldMs >= 1;
    }
    if (sysml_lazy_9) {
        sysml_state_led1Stable = false;
    }
    sysml_lazy_10 = sysml_state_btn2HoldMs >= 1;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_btn3HoldMs >= 1;
    }
    if (sysml_lazy_10) {
        sysml_state_led2Stable = false;
    }
    sysml_state_prevBtn1 = io_2;
    sysml_state_prevBtn2 = io_3;
    sysml_state_prevBtn3 = io_4;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led1Stable;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_led2Stable;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
}