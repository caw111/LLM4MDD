#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_value_0 = 0;
    bool sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    bool sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    bool sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    bool sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    bool sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    bool sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_0 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_1) >= 1;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_2 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_3) >= 2;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_4 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_5) >= 3;
    }
    sysml_value_2 = sysml_conditional_4;
    bool sysml_value_3 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_6 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_7 = 1;
        }
        else {
            sysml_conditional_7 = 0;
        }
        sysml_conditional_6 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_7) >= 4;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_8 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_9 = 1;
        }
        else {
            sysml_conditional_9 = 0;
        }
        sysml_conditional_8 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_9) >= 5;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_10 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_11 = 1;
        }
        else {
            sysml_conditional_11 = 0;
        }
        sysml_conditional_10 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_11) >= 6;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_12 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_conditional_12 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_13) >= 7;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_14 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_conditional_14 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_15) >= 8;
    }
    sysml_value_7 = sysml_conditional_14;
    bool sysml_value_8 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_16 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_17 = 1;
        }
        else {
            sysml_conditional_17 = 0;
        }
        sysml_conditional_16 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_17) >= 9;
    }
    sysml_value_8 = sysml_conditional_16;
    bool sysml_value_9 = 0;
    if (io_0 < 0.0) {
        sysml_conditional_18 = false;
    }
    else {
        if ((((double)(io_0 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_0 * 10.0)) / 1023.0)))) {
            sysml_conditional_19 = 1;
        }
        else {
            sysml_conditional_19 = 0;
        }
        sysml_conditional_18 = (((int)(((double)(io_0 * 10.0)) / 1023.0)) - sysml_conditional_19) >= 10;
    }
    sysml_value_9 = sysml_conditional_18;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
}