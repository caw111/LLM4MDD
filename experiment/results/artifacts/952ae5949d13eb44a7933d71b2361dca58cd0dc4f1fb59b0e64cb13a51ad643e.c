#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn3 = false;
/* Global Variable define of  */
int sysml_state_accumMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_prevBtn3 = false;
    sysml_state_accumMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, double io_3, int io_4, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    int sysml_conditional_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    if (io_2) {
        sysml_state_number = 0;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_0 = !io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevBtn1;
    }
    if (sysml_lazy_1) {
        if (sysml_state_number >= 99) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_2;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_3 = !io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = io_1;
    }
    sysml_lazy_4 = sysml_lazy_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    if (sysml_lazy_4) {
        if (sysml_state_number == 0) {
            sysml_conditional_5 = 99;
        }
        else {
            sysml_conditional_5 = sysml_state_number - 1;
        }
        sysml_state_number = sysml_conditional_5;
        sysml_state_accumMs = 0;
    }
    sysml_lazy_6 = !io_2;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !io_0;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !io_1;
    }
    if (sysml_lazy_7) {
        sysml_state_accumMs = 0;
    }
    sysml_lazy_8 = !io_2;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_0;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !io_1;
    }
    if (sysml_lazy_9) {
        sysml_state_accumMs = (sysml_state_accumMs + io_4);
        if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
            sysml_conditional_10 = 1;
        }
        else {
            sysml_conditional_10 = 0;
        }
        if (sysml_state_accumMs >= (100 + (((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_10))) {
            if (sysml_state_number >= 99) {
                sysml_conditional_11 = 0;
            }
            else {
                sysml_conditional_11 = sysml_state_number + 1;
            }
            sysml_state_number = sysml_conditional_11;
            sysml_state_accumMs = 0;
        }
    }
    sysml_lazy_12 = !io_2;
    if (sysml_lazy_12) {
        sysml_lazy_12 = io_1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !io_0;
    }
    if (sysml_lazy_13) {
        sysml_state_accumMs = (sysml_state_accumMs + io_4);
        if ((((double)(io_3 * 900.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 900.0)) / 1023.0)))) {
            sysml_conditional_14 = 1;
        }
        else {
            sysml_conditional_14 = 0;
        }
        if (sysml_state_accumMs >= (100 + (((int)(((double)(io_3 * 900.0)) / 1023.0)) - sysml_conditional_14))) {
            if (sysml_state_number == 0) {
                sysml_conditional_15 = 99;
            }
            else {
                sysml_conditional_15 = sysml_state_number - 1;
            }
            sysml_state_number = sysml_conditional_15;
            sysml_state_accumMs = 0;
        }
    }
    sysml_state_prevBtn1 = io_0;
    sysml_state_prevBtn2 = io_1;
    sysml_state_prevBtn3 = io_2;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    *io_5 = sysml_value_0;
}