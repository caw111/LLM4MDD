#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayNumber = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, int* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_conditional_5 = 0;
    int sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    int sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    int sysml_conditional_28 = 0;
    int sysml_conditional_29 = 0;
    int sysml_conditional_30 = 0;
    int sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    int sysml_conditional_33 = 0;
    int sysml_conditional_34 = 0;
    int sysml_conditional_35 = 0;
    int sysml_conditional_36 = 0;
    bool sysml_conditional_37 = 0;
    int sysml_conditional_38 = 0;
    int sysml_conditional_39 = 0;
    int sysml_conditional_40 = 0;
    int sysml_conditional_41 = 0;
    bool sysml_conditional_42 = 0;
    int sysml_conditional_43 = 0;
    int sysml_conditional_44 = 0;
    int sysml_conditional_45 = 0;
    int sysml_conditional_46 = 0;
    bool sysml_conditional_47 = 0;
    int sysml_conditional_48 = 0;
    int sysml_conditional_49 = 0;
    int sysml_conditional_50 = 0;
    int sysml_conditional_51 = 0;
    bool sysml_conditional_52 = 0;
    if (((sysml_state_accumulatedMs + io_0) >= 2000)) {
        sysml_state_accumulatedMs = 0;
        if ((sysml_state_displayNumber + 1) > 99) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_displayNumber + 1;
        }
        sysml_state_displayNumber = sysml_conditional_0;
    }
    int sysml_value_0 = 0;
    if (sysml_state_displayNumber < 0) {
        sysml_conditional_1 = 0;
    }
    else {
        if (sysml_state_displayNumber > 99) {
            sysml_conditional_2 = 0;
        }
        else {
            sysml_conditional_2 = sysml_state_displayNumber;
        }
        sysml_conditional_1 = sysml_conditional_2;
    }
    sysml_value_0 = sysml_conditional_1;
    bool sysml_value_1 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_4) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_5) * 1.0)) / 10.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_3) * 1.0)) / 10.0)) - sysml_conditional_6) >= 1) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_9) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_10) * 1.0)) / 10.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_8) * 1.0)) / 10.0)) - sysml_conditional_11) >= 2) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_14) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_15) * 1.0)) / 10.0)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_13) * 1.0)) / 10.0)) - sysml_conditional_16) >= 3) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_3 = sysml_conditional_17;
    bool sysml_value_4 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_19) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_20) * 1.0)) / 10.0)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_18) * 1.0)) / 10.0)) - sysml_conditional_21) >= 4) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_4 = sysml_conditional_22;
    bool sysml_value_5 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_24) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_25) * 1.0)) / 10.0)))) {
        sysml_conditional_26 = 1;
    }
    else {
        sysml_conditional_26 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_23) * 1.0)) / 10.0)) - sysml_conditional_26) >= 5) {
        sysml_conditional_27 = true;
    }
    else {
        sysml_conditional_27 = false;
    }
    sysml_value_5 = sysml_conditional_27;
    bool sysml_value_6 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_28 = 1;
    }
    else {
        sysml_conditional_28 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_29 = 1;
    }
    else {
        sysml_conditional_29 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_30 = 1;
    }
    else {
        sysml_conditional_30 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_29) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_30) * 1.0)) / 10.0)))) {
        sysml_conditional_31 = 1;
    }
    else {
        sysml_conditional_31 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_28) * 1.0)) / 10.0)) - sysml_conditional_31) >= 6) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_6 = sysml_conditional_32;
    bool sysml_value_7 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_33 = 1;
    }
    else {
        sysml_conditional_33 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_34 = 1;
    }
    else {
        sysml_conditional_34 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_35 = 1;
    }
    else {
        sysml_conditional_35 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_34) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_35) * 1.0)) / 10.0)))) {
        sysml_conditional_36 = 1;
    }
    else {
        sysml_conditional_36 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_33) * 1.0)) / 10.0)) - sysml_conditional_36) >= 7) {
        sysml_conditional_37 = true;
    }
    else {
        sysml_conditional_37 = false;
    }
    sysml_value_7 = sysml_conditional_37;
    bool sysml_value_8 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_38 = 1;
    }
    else {
        sysml_conditional_38 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_39 = 1;
    }
    else {
        sysml_conditional_39 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_40 = 1;
    }
    else {
        sysml_conditional_40 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_39) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_40) * 1.0)) / 10.0)))) {
        sysml_conditional_41 = 1;
    }
    else {
        sysml_conditional_41 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_38) * 1.0)) / 10.0)) - sysml_conditional_41) >= 8) {
        sysml_conditional_42 = true;
    }
    else {
        sysml_conditional_42 = false;
    }
    sysml_value_8 = sysml_conditional_42;
    bool sysml_value_9 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_43 = 1;
    }
    else {
        sysml_conditional_43 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_44 = 1;
    }
    else {
        sysml_conditional_44 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_45 = 1;
    }
    else {
        sysml_conditional_45 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_44) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_45) * 1.0)) / 10.0)))) {
        sysml_conditional_46 = 1;
    }
    else {
        sysml_conditional_46 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_43) * 1.0)) / 10.0)) - sysml_conditional_46) >= 9) {
        sysml_conditional_47 = true;
    }
    else {
        sysml_conditional_47 = false;
    }
    sysml_value_9 = sysml_conditional_47;
    bool sysml_value_10 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_48 = 1;
    }
    else {
        sysml_conditional_48 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_49 = 1;
    }
    else {
        sysml_conditional_49 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_50 = 1;
    }
    else {
        sysml_conditional_50 = 0;
    }
    if ((((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_49) * 1.0)) / 10.0) < ((double)((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_50) * 1.0)) / 10.0)))) {
        sysml_conditional_51 = 1;
    }
    else {
        sysml_conditional_51 = 0;
    }
    if ((((int)(((double)((((int)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_48) * 1.0)) / 10.0)) - sysml_conditional_51) >= 10) {
        sysml_conditional_52 = true;
    }
    else {
        sysml_conditional_52 = false;
    }
    sysml_value_10 = sysml_conditional_52;
    double sysml_value_11 = 0;
    sysml_value_11 = (sysml_state_displayNumber * 1.8);
    double sysml_value_12 = 0;
    sysml_value_12 = (sysml_state_displayNumber * 1.8);
    *io_1 = sysml_value_0;
    *io_2_segment_0 = sysml_value_1;
    *io_2_segment_1 = sysml_value_2;
    *io_2_segment_2 = sysml_value_3;
    *io_2_segment_3 = sysml_value_4;
    *io_2_segment_4 = sysml_value_5;
    *io_2_segment_5 = sysml_value_6;
    *io_2_segment_6 = sysml_value_7;
    *io_2_segment_7 = sysml_value_8;
    *io_2_segment_8 = sysml_value_9;
    *io_2_segment_9 = sysml_value_10;
    *io_3 = sysml_value_11;
    *io_4 = sysml_value_12;
}