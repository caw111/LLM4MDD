#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_value0 = false;
/* Global Variable define of  */
bool sysml_state_value1 = false;
/* Global Variable define of  */
bool sysml_state_value2 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn0 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn1 = false;
/* Global Variable define of  */
bool sysml_state_prevBtn2 = false;
/* Global Variable define of  */
int sysml_state_accBtn0Ms = 0;
/* Global Variable define of  */
int sysml_state_accBtn1Ms = 0;
/* Global Variable define of  */
int sysml_state_accBtn2Ms = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_value0 = false;
    sysml_state_value1 = false;
    sysml_state_value2 = false;
    sysml_state_prevBtn0 = false;
    sysml_state_prevBtn1 = false;
    sysml_state_prevBtn2 = false;
    sysml_state_accBtn0Ms = 0;
    sysml_state_accBtn1Ms = 0;
    sysml_state_accBtn2Ms = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6_segment_0, bool* io_6_segment_1, bool* io_6_segment_2, bool* io_6_segment_3, bool* io_6_segment_4, bool* io_6_segment_5, bool* io_6_segment_6, bool* io_6_segment_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    bool sysml_lazy_21 = 0;
    if (io_0) {
        sysml_state_accBtn0Ms = (sysml_state_accBtn0Ms + 5);
    }
    else {
        sysml_state_accBtn0Ms = 0;
    }
    if (io_1) {
        sysml_state_accBtn1Ms = (sysml_state_accBtn1Ms + 5);
    }
    else {
        sysml_state_accBtn1Ms = 0;
    }
    if (io_2) {
        sysml_state_accBtn2Ms = (sysml_state_accBtn2Ms + 5);
    }
    else {
        sysml_state_accBtn2Ms = 0;
    }
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevBtn0;
    }
    sysml_lazy_1 = sysml_lazy_0;
    if (sysml_lazy_1) {
        sysml_lazy_1 = sysml_state_accBtn0Ms >= 150;
    }
    if (sysml_lazy_1) {
        sysml_state_value0 = (!sysml_state_value0);
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevBtn1;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = sysml_state_accBtn1Ms >= 150;
    }
    if (sysml_lazy_3) {
        sysml_state_value1 = (!sysml_state_value1);
    }
    sysml_lazy_4 = io_2;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevBtn2;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = sysml_state_accBtn2Ms >= 150;
    }
    if (sysml_lazy_5) {
        sysml_state_value2 = (!sysml_state_value2);
    }
    sysml_state_prevBtn0 = io_0;
    sysml_state_prevBtn1 = io_1;
    sysml_state_prevBtn2 = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_value0;
    bool sysml_value_1 = 0;
    sysml_value_1 = sysml_state_value1;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_value2;
    bool sysml_value_3 = 0;
    sysml_lazy_6 = sysml_state_value0;
    if (!sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_value1;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (!sysml_lazy_7) {
        sysml_lazy_7 = sysml_state_value2;
    }
    sysml_value_3 = !sysml_lazy_7;
    bool sysml_value_4 = 0;
    sysml_lazy_8 = sysml_state_value0;
    if (sysml_lazy_8) {
        sysml_lazy_8 = !sysml_state_value1;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_value2;
    }
    sysml_value_4 = sysml_lazy_9;
    bool sysml_value_5 = 0;
    sysml_lazy_10 = !sysml_state_value0;
    if (sysml_lazy_10) {
        sysml_lazy_10 = sysml_state_value1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = !sysml_state_value2;
    }
    sysml_value_5 = sysml_lazy_11;
    bool sysml_value_6 = 0;
    sysml_lazy_12 = sysml_state_value0;
    if (sysml_lazy_12) {
        sysml_lazy_12 = sysml_state_value1;
    }
    sysml_lazy_13 = sysml_lazy_12;
    if (sysml_lazy_13) {
        sysml_lazy_13 = !sysml_state_value2;
    }
    sysml_value_6 = sysml_lazy_13;
    bool sysml_value_7 = 0;
    sysml_lazy_14 = !sysml_state_value0;
    if (sysml_lazy_14) {
        sysml_lazy_14 = !sysml_state_value1;
    }
    sysml_lazy_15 = sysml_lazy_14;
    if (sysml_lazy_15) {
        sysml_lazy_15 = sysml_state_value2;
    }
    sysml_value_7 = sysml_lazy_15;
    bool sysml_value_8 = 0;
    sysml_lazy_16 = sysml_state_value0;
    if (sysml_lazy_16) {
        sysml_lazy_16 = !sysml_state_value1;
    }
    sysml_lazy_17 = sysml_lazy_16;
    if (sysml_lazy_17) {
        sysml_lazy_17 = sysml_state_value2;
    }
    sysml_value_8 = sysml_lazy_17;
    bool sysml_value_9 = 0;
    sysml_lazy_18 = !sysml_state_value0;
    if (sysml_lazy_18) {
        sysml_lazy_18 = sysml_state_value1;
    }
    sysml_lazy_19 = sysml_lazy_18;
    if (sysml_lazy_19) {
        sysml_lazy_19 = sysml_state_value2;
    }
    sysml_value_9 = sysml_lazy_19;
    bool sysml_value_10 = 0;
    sysml_lazy_20 = sysml_state_value0;
    if (sysml_lazy_20) {
        sysml_lazy_20 = sysml_state_value1;
    }
    sysml_lazy_21 = sysml_lazy_20;
    if (sysml_lazy_21) {
        sysml_lazy_21 = sysml_state_value2;
    }
    sysml_value_10 = sysml_lazy_21;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *io_6_segment_0 = sysml_value_3;
    *io_6_segment_1 = sysml_value_4;
    *io_6_segment_2 = sysml_value_5;
    *io_6_segment_3 = sysml_value_6;
    *io_6_segment_4 = sysml_value_7;
    *io_6_segment_5 = sysml_value_8;
    *io_6_segment_6 = sysml_value_9;
    *io_6_segment_7 = sysml_value_10;
}