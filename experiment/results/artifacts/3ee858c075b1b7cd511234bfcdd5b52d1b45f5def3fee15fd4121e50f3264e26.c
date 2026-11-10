#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_ledCountSec1 = 0;
/* Global Variable define of  */
int sysml_state_ledCountSec2 = 0;
/* Global Variable define of  */
int sysml_state_ledCountSec3 = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_ledCountSec1 = 0;
    sysml_state_ledCountSec2 = 0;
    sysml_state_ledCountSec3 = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double io_2, bool* io_3_segment_0, bool* io_3_segment_1, bool* io_3_segment_2, bool* io_3_segment_3, bool* io_3_segment_4, bool* io_3_segment_5, bool* io_3_segment_6, bool* io_3_segment_7, bool* io_3_segment_8, bool* io_3_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 0;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_1 = 3;
        }
        else {
            if (io_0 < 256.0) {
                sysml_conditional_2 = 0;
            }
            else {
                if (io_0 < 512.0) {
                    sysml_conditional_3 = 1;
                }
                else {
                    if (io_0 < 768.0) {
                        sysml_conditional_4 = 2;
                    }
                    else {
                        sysml_conditional_4 = 3;
                    }
                    sysml_conditional_3 = sysml_conditional_4;
                }
                sysml_conditional_2 = sysml_conditional_3;
            }
            sysml_conditional_1 = sysml_conditional_2;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_ledCountSec1 = sysml_conditional_0;
    if (io_1 <= 0.0) {
        sysml_conditional_5 = 0;
    }
    else {
        if (io_1 >= 1023.0) {
            sysml_conditional_6 = 3;
        }
        else {
            if (io_1 < 256.0) {
                sysml_conditional_7 = 0;
            }
            else {
                if (io_1 < 512.0) {
                    sysml_conditional_8 = 1;
                }
                else {
                    if (io_1 < 768.0) {
                        sysml_conditional_9 = 2;
                    }
                    else {
                        sysml_conditional_9 = 3;
                    }
                    sysml_conditional_8 = sysml_conditional_9;
                }
                sysml_conditional_7 = sysml_conditional_8;
            }
            sysml_conditional_6 = sysml_conditional_7;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_state_ledCountSec2 = sysml_conditional_5;
    if (io_2 <= 0.0) {
        sysml_conditional_10 = 0;
    }
    else {
        if (io_2 >= 1023.0) {
            sysml_conditional_11 = 4;
        }
        else {
            if (io_2 < 256.0) {
                sysml_conditional_12 = 0;
            }
            else {
                if (io_2 < 512.0) {
                    sysml_conditional_13 = 1;
                }
                else {
                    if (io_2 < 768.0) {
                        sysml_conditional_14 = 2;
                    }
                    else {
                        sysml_conditional_14 = 3;
                    }
                    sysml_conditional_13 = sysml_conditional_14;
                }
                sysml_conditional_12 = sysml_conditional_13;
            }
            sysml_conditional_11 = sysml_conditional_12;
        }
        sysml_conditional_10 = sysml_conditional_11;
    }
    sysml_state_ledCountSec3 = sysml_conditional_10;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_ledCountSec1 >= 1);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_ledCountSec1 >= 2);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_ledCountSec1 >= 3);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_ledCountSec2 >= 1);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_ledCountSec2 >= 2);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_ledCountSec2 >= 3);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_ledCountSec3 >= 1);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_ledCountSec3 >= 2);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_ledCountSec3 >= 3);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_ledCountSec3 >= 4);
    *io_3_segment_0 = sysml_value_0;
    *io_3_segment_1 = sysml_value_1;
    *io_3_segment_2 = sysml_value_2;
    *io_3_segment_3 = sysml_value_3;
    *io_3_segment_4 = sysml_value_4;
    *io_3_segment_5 = sysml_value_5;
    *io_3_segment_6 = sysml_value_6;
    *io_3_segment_7 = sysml_value_7;
    *io_3_segment_8 = sysml_value_8;
    *io_3_segment_9 = sysml_value_9;
}