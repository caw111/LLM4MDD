#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_autoMode = false;
/* Global Variable define of  */
bool sysml_state_prevIo2 = false;
/* Global Variable define of  */
bool sysml_state_prevIo3 = false;
/* Global Variable define of  */
int sysml_state_curAnglePct = 50;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_autoMode = false;
    sysml_state_prevIo2 = false;
    sysml_state_prevIo3 = false;
    sysml_state_curAnglePct = 50;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, bool io_2, bool io_3, bool* io_4, double* io_5, int* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    bool sysml_lazy_11 = 0;
    int sysml_conditional_12 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevIo2;
    }
    if (sysml_lazy_0) {
        if (sysml_state_autoMode) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = true;
        }
        sysml_state_autoMode = sysml_conditional_1;
    }
    sysml_lazy_2 = !sysml_state_autoMode;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_3;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevIo3;
    }
    if (sysml_lazy_3) {
        if (sysml_state_curAnglePct >= 100) {
            sysml_conditional_4 = 0;
        }
        else {
            sysml_conditional_4 = sysml_state_curAnglePct + 10;
        }
        sysml_state_curAnglePct = sysml_conditional_4;
    }
    if (sysml_state_autoMode) {
        sysml_lazy_5 = io_0 >= 700.0;
        if (sysml_lazy_5) {
            sysml_lazy_6 = io_1 > 30.0;
            if (sysml_lazy_6) {
                sysml_lazy_6 = true;
            }
            sysml_lazy_5 = sysml_lazy_6;
        }
        if (sysml_lazy_5) {
            sysml_conditional_7 = 0;
        }
        else {
            if (io_1 > 30.0) {
                sysml_conditional_8 = 0;
            }
            else {
                if (io_0 <= 300.0) {
                    sysml_conditional_9 = 100;
                }
                else {
                    sysml_conditional_9 = 0;
                }
                sysml_conditional_8 = sysml_conditional_9;
            }
            sysml_conditional_7 = sysml_conditional_8;
        }
        sysml_state_curAnglePct = sysml_conditional_7;
    }
    sysml_state_prevIo2 = io_2;
    sysml_state_prevIo3 = io_3;
    bool sysml_value_0 = 0;
    if (sysml_state_autoMode) {
        sysml_lazy_11 = io_0 <= 300.0;
        if (sysml_lazy_11) {
            sysml_lazy_11 = io_1 <= 30.0;
        }
        sysml_conditional_10 = sysml_lazy_11;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_0 = sysml_conditional_10;
    double sysml_value_1 = 0;
    if ((sysml_state_curAnglePct * 1.0) < ((double)((int)(sysml_state_curAnglePct * 1.0)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_1 = ((int)(sysml_state_curAnglePct * 1.0)) - sysml_conditional_12;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_curAnglePct;
    *io_4 = sysml_value_0;
    *io_5 = sysml_value_1;
    *io_6 = sysml_value_2;
}