#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int32_t sysml_state_displayNumber;
/* Global Variable define of  */
extern int32_t sysml_state_accumulatedMs;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9, double* io_3, double* io_4);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_displayNumber = 0;
/* Global Variable define of  */
int32_t sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_displayNumber = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int32_t io_0, int32_t* io_1, bool* io_2_segment_0, bool* io_2_segment_1, bool* io_2_segment_2, bool* io_2_segment_3, bool* io_2_segment_4, bool* io_2_segment_5, bool* io_2_segment_6, bool* io_2_segment_7, bool* io_2_segment_8, bool* io_2_segment_9, double* io_3, double* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_conditional_2 = 0;
    int32_t sysml_conditional_3 = 0;
    int32_t sysml_conditional_4 = 0;
    int32_t sysml_conditional_5 = 0;
    int32_t sysml_conditional_6 = 0;
    bool sysml_conditional_7 = 0;
    int32_t sysml_conditional_8 = 0;
    int32_t sysml_conditional_9 = 0;
    int32_t sysml_conditional_10 = 0;
    int32_t sysml_conditional_11 = 0;
    bool sysml_conditional_12 = 0;
    int32_t sysml_conditional_13 = 0;
    int32_t sysml_conditional_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    bool sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    bool sysml_conditional_27 = 0;
    int32_t sysml_conditional_28 = 0;
    int32_t sysml_conditional_29 = 0;
    int32_t sysml_conditional_30 = 0;
    int32_t sysml_conditional_31 = 0;
    bool sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    int32_t sysml_conditional_34 = 0;
    int32_t sysml_conditional_35 = 0;
    int32_t sysml_conditional_36 = 0;
    bool sysml_conditional_37 = 0;
    int32_t sysml_conditional_38 = 0;
    int32_t sysml_conditional_39 = 0;
    int32_t sysml_conditional_40 = 0;
    int32_t sysml_conditional_41 = 0;
    bool sysml_conditional_42 = 0;
    int32_t sysml_conditional_43 = 0;
    int32_t sysml_conditional_44 = 0;
    int32_t sysml_conditional_45 = 0;
    int32_t sysml_conditional_46 = 0;
    bool sysml_conditional_47 = 0;
    int32_t sysml_conditional_48 = 0;
    int32_t sysml_conditional_49 = 0;
    int32_t sysml_conditional_50 = 0;
    int32_t sysml_conditional_51 = 0;
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
    int32_t sysml_value_0 = 0;
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
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_4 = 1;
    }
    else {
        sysml_conditional_4 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_4) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_5) * 1.0)) / 10.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_3) * 1.0)) / 10.0)) - sysml_conditional_6) >= 1) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_1 = sysml_conditional_7;
    bool sysml_value_2 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_9) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_10) * 1.0)) / 10.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_8) * 1.0)) / 10.0)) - sysml_conditional_11) >= 2) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_2 = sysml_conditional_12;
    bool sysml_value_3 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_14) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_15) * 1.0)) / 10.0)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_13) * 1.0)) / 10.0)) - sysml_conditional_16) >= 3) {
        sysml_conditional_17 = true;
    }
    else {
        sysml_conditional_17 = false;
    }
    sysml_value_3 = sysml_conditional_17;
    bool sysml_value_4 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_19) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_20) * 1.0)) / 10.0)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_18) * 1.0)) / 10.0)) - sysml_conditional_21) >= 4) {
        sysml_conditional_22 = true;
    }
    else {
        sysml_conditional_22 = false;
    }
    sysml_value_4 = sysml_conditional_22;
    bool sysml_value_5 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_24) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_25) * 1.0)) / 10.0)))) {
        sysml_conditional_26 = 1;
    }
    else {
        sysml_conditional_26 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_23) * 1.0)) / 10.0)) - sysml_conditional_26) >= 5) {
        sysml_conditional_27 = true;
    }
    else {
        sysml_conditional_27 = false;
    }
    sysml_value_5 = sysml_conditional_27;
    bool sysml_value_6 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_28 = 1;
    }
    else {
        sysml_conditional_28 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_29 = 1;
    }
    else {
        sysml_conditional_29 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_30 = 1;
    }
    else {
        sysml_conditional_30 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_29) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_30) * 1.0)) / 10.0)))) {
        sysml_conditional_31 = 1;
    }
    else {
        sysml_conditional_31 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_28) * 1.0)) / 10.0)) - sysml_conditional_31) >= 6) {
        sysml_conditional_32 = true;
    }
    else {
        sysml_conditional_32 = false;
    }
    sysml_value_6 = sysml_conditional_32;
    bool sysml_value_7 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_33 = 1;
    }
    else {
        sysml_conditional_33 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_34 = 1;
    }
    else {
        sysml_conditional_34 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_35 = 1;
    }
    else {
        sysml_conditional_35 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_34) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_35) * 1.0)) / 10.0)))) {
        sysml_conditional_36 = 1;
    }
    else {
        sysml_conditional_36 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_33) * 1.0)) / 10.0)) - sysml_conditional_36) >= 7) {
        sysml_conditional_37 = true;
    }
    else {
        sysml_conditional_37 = false;
    }
    sysml_value_7 = sysml_conditional_37;
    bool sysml_value_8 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_38 = 1;
    }
    else {
        sysml_conditional_38 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_39 = 1;
    }
    else {
        sysml_conditional_39 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_40 = 1;
    }
    else {
        sysml_conditional_40 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_39) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_40) * 1.0)) / 10.0)))) {
        sysml_conditional_41 = 1;
    }
    else {
        sysml_conditional_41 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_38) * 1.0)) / 10.0)) - sysml_conditional_41) >= 8) {
        sysml_conditional_42 = true;
    }
    else {
        sysml_conditional_42 = false;
    }
    sysml_value_8 = sysml_conditional_42;
    bool sysml_value_9 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_43 = 1;
    }
    else {
        sysml_conditional_43 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_44 = 1;
    }
    else {
        sysml_conditional_44 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_45 = 1;
    }
    else {
        sysml_conditional_45 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_44) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_45) * 1.0)) / 10.0)))) {
        sysml_conditional_46 = 1;
    }
    else {
        sysml_conditional_46 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_43) * 1.0)) / 10.0)) - sysml_conditional_46) >= 9) {
        sysml_conditional_47 = true;
    }
    else {
        sysml_conditional_47 = false;
    }
    sysml_value_9 = sysml_conditional_47;
    bool sysml_value_10 = 0;
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_48 = 1;
    }
    else {
        sysml_conditional_48 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_49 = 1;
    }
    else {
        sysml_conditional_49 = 0;
    }
    if ((((double)(sysml_state_displayNumber * 100.0)) / 99.0) < ((double)((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)))) {
        sysml_conditional_50 = 1;
    }
    else {
        sysml_conditional_50 = 0;
    }
    if ((((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_49) * 1.0)) / 10.0) < ((double)((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_50) * 1.0)) / 10.0)))) {
        sysml_conditional_51 = 1;
    }
    else {
        sysml_conditional_51 = 0;
    }
    if ((((int32_t)(((double)((((int32_t)(((double)(sysml_state_displayNumber * 100.0)) / 99.0)) - sysml_conditional_48) * 1.0)) / 10.0)) - sysml_conditional_51) >= 10) {
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
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:mega
#include <Servo.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_MEGA2560)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static int32_t hw_value_io_0 = 0;
static int32_t hw_value_io_1 = 0;
static void hw_shift595_io_1(int32_t value) {
  static const uint8_t digits[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
  bool valid = value >= 0 && value < 100L;
  digitalWrite(A1, LOW);
  {
    uint8_t segments = valid && (true) ? digits[(value / 1L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  {
    uint8_t segments = valid && (true) ? digits[(value / 10L) % 10] : 0;
    uint8_t bits = 0;
    if (segments & 1) bits |= 1;
    if (segments & 2) bits |= 2;
    if (segments & 4) bits |= 4;
    if (segments & 8) bits |= 8;
    if (segments & 16) bits |= 16;
    if (segments & 32) bits |= 32;
    if (segments & 64) bits |= 64;
    bits = (uint8_t)~bits;
    shiftOut(A0, A2, MSBFIRST, bits);
  }
  digitalWrite(A1, HIGH);
}
static int32_t hw_shift595_previous_io_1 = INT32_MIN;
static bool hw_value_io_2_segment_0 = false;
static bool hw_value_io_2_segment_1 = false;
static bool hw_value_io_2_segment_2 = false;
static bool hw_value_io_2_segment_3 = false;
static bool hw_value_io_2_segment_4 = false;
static bool hw_value_io_2_segment_5 = false;
static bool hw_value_io_2_segment_6 = false;
static bool hw_value_io_2_segment_7 = false;
static bool hw_value_io_2_segment_8 = false;
static bool hw_value_io_2_segment_9 = false;
static double hw_value_io_3 = 0;
Servo hw_servo_io_3;
static int hw_servo_previous_io_3 = -1;
static double hw_value_io_4 = 0;
Servo hw_servo_io_4;
static int hw_servo_previous_io_4 = -1;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  pinMode(A0, OUTPUT); digitalWrite(A0, LOW);
  pinMode(A2, OUTPUT); digitalWrite(A2, LOW);
  pinMode(A1, OUTPUT); digitalWrite(A1, LOW);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
  pinMode(2, OUTPUT);
  digitalWrite(2, hw_value_io_2_segment_0 ? HIGH : LOW);
  pinMode(3, OUTPUT);
  digitalWrite(3, hw_value_io_2_segment_1 ? HIGH : LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, hw_value_io_2_segment_2 ? HIGH : LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, hw_value_io_2_segment_3 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  digitalWrite(6, hw_value_io_2_segment_4 ? HIGH : LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, hw_value_io_2_segment_5 ? HIGH : LOW);
  pinMode(8, OUTPUT);
  digitalWrite(8, hw_value_io_2_segment_6 ? HIGH : LOW);
  pinMode(9, OUTPUT);
  digitalWrite(9, hw_value_io_2_segment_7 ? HIGH : LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, hw_value_io_2_segment_8 ? HIGH : LOW);
  pinMode(11, OUTPUT);
  digitalWrite(11, hw_value_io_2_segment_9 ? HIGH : LOW);
  hw_servo_io_3.attach(12);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  hw_servo_io_4.attach(13);
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 10UL) return;
  if (hw_elapsed > 2147483647UL) return;
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_0 = (int32_t)hw_elapsed;
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, &hw_value_io_1, &hw_value_io_2_segment_0, &hw_value_io_2_segment_1, &hw_value_io_2_segment_2, &hw_value_io_2_segment_3, &hw_value_io_2_segment_4, &hw_value_io_2_segment_5, &hw_value_io_2_segment_6, &hw_value_io_2_segment_7, &hw_value_io_2_segment_8, &hw_value_io_2_segment_9, &hw_value_io_3, &hw_value_io_4);
  if (hw_value_io_1 != hw_shift595_previous_io_1) {
    hw_shift595_io_1(hw_value_io_1); hw_shift595_previous_io_1 = hw_value_io_1;
  }
  digitalWrite(2, hw_value_io_2_segment_0 ? HIGH : LOW);
  digitalWrite(3, hw_value_io_2_segment_1 ? HIGH : LOW);
  digitalWrite(4, hw_value_io_2_segment_2 ? HIGH : LOW);
  digitalWrite(5, hw_value_io_2_segment_3 ? HIGH : LOW);
  digitalWrite(6, hw_value_io_2_segment_4 ? HIGH : LOW);
  digitalWrite(7, hw_value_io_2_segment_5 ? HIGH : LOW);
  digitalWrite(8, hw_value_io_2_segment_6 ? HIGH : LOW);
  digitalWrite(9, hw_value_io_2_segment_7 ? HIGH : LOW);
  digitalWrite(10, hw_value_io_2_segment_8 ? HIGH : LOW);
  digitalWrite(11, hw_value_io_2_segment_9 ? HIGH : LOW);
  if (isfinite((double)hw_value_io_3) && hw_value_io_3 >= 0 && hw_value_io_3 <= 180) {
    int command = (int)hw_value_io_3;
    if (command != hw_servo_previous_io_3) {
      hw_servo_io_3.write(command); hw_servo_previous_io_3 = command;
    }
  }
  if (isfinite((double)hw_value_io_4) && hw_value_io_4 >= 0 && hw_value_io_4 <= 180) {
    int command = (int)hw_value_io_4;
    if (command != hw_servo_previous_io_4) {
      hw_servo_io_4.write(command); hw_servo_previous_io_4 = command;
    }
  }
}
