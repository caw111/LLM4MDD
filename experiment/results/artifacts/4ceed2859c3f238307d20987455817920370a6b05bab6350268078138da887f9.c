#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_count = 0;
/* Global Variable define of  */
int sysml_state_accumulatedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_count = 0;
    sysml_state_accumulatedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_4, int* io_0, bool* io_1_segment_0, bool* io_1_segment_1, bool* io_1_segment_2, bool* io_1_segment_3, bool* io_1_segment_4, bool* io_1_segment_5, bool* io_1_segment_6, bool* io_1_segment_7, bool* io_1_segment_8, bool* io_1_segment_9, double* io_2, double* io_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
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
    bool sysml_conditional_20 = 0;
    int sysml_conditional_21 = 0;
    int sysml_conditional_22 = 0;
    if ((io_4 > 0)) {
        sysml_state_accumulatedMs = (sysml_state_accumulatedMs + io_4);
        if ((sysml_state_accumulatedMs >= 2000)) {
            sysml_state_accumulatedMs = (sysml_state_accumulatedMs - 2000);
            if (sysml_state_count >= 99) {
                sysml_conditional_0 = 0;
            }
            else {
                sysml_conditional_0 = sysml_state_count + 1;
            }
            sysml_state_count = sysml_conditional_0;
        }
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_count;
    bool sysml_value_1 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_1) >= 1.0) {
        sysml_conditional_2 = true;
    }
    else {
        sysml_conditional_2 = false;
    }
    sysml_value_1 = sysml_conditional_2;
    bool sysml_value_2 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_3 = 1;
    }
    else {
        sysml_conditional_3 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_3) >= 2.0) {
        sysml_conditional_4 = true;
    }
    else {
        sysml_conditional_4 = false;
    }
    sysml_value_2 = sysml_conditional_4;
    bool sysml_value_3 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_5 = 1;
    }
    else {
        sysml_conditional_5 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_5) >= 3.0) {
        sysml_conditional_6 = true;
    }
    else {
        sysml_conditional_6 = false;
    }
    sysml_value_3 = sysml_conditional_6;
    bool sysml_value_4 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_7) >= 4.0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_4 = sysml_conditional_8;
    bool sysml_value_5 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_9) >= 5.0) {
        sysml_conditional_10 = true;
    }
    else {
        sysml_conditional_10 = false;
    }
    sysml_value_5 = sysml_conditional_10;
    bool sysml_value_6 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_11) >= 6.0) {
        sysml_conditional_12 = true;
    }
    else {
        sysml_conditional_12 = false;
    }
    sysml_value_6 = sysml_conditional_12;
    bool sysml_value_7 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_13) >= 7.0) {
        sysml_conditional_14 = true;
    }
    else {
        sysml_conditional_14 = false;
    }
    sysml_value_7 = sysml_conditional_14;
    bool sysml_value_8 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_15) >= 8.0) {
        sysml_conditional_16 = true;
    }
    else {
        sysml_conditional_16 = false;
    }
    sysml_value_8 = sysml_conditional_16;
    bool sysml_value_9 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_17) >= 9.0) {
        sysml_conditional_18 = true;
    }
    else {
        sysml_conditional_18 = false;
    }
    sysml_value_9 = sysml_conditional_18;
    bool sysml_value_10 = 0;
    if ((((double)(sysml_state_count * 10.0)) / 99.0) < ((double)((int)(((double)(sysml_state_count * 10.0)) / 99.0)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    if ((((int)(((double)(sysml_state_count * 10.0)) / 99.0)) - sysml_conditional_19) >= 10.0) {
        sysml_conditional_20 = true;
    }
    else {
        sysml_conditional_20 = false;
    }
    sysml_value_10 = sysml_conditional_20;
    double sysml_value_11 = 0;
    if ((sysml_state_count * 1.8) < ((double)((int)(sysml_state_count * 1.8)))) {
        sysml_conditional_21 = 1;
    }
    else {
        sysml_conditional_21 = 0;
    }
    sysml_value_11 = ((int)(sysml_state_count * 1.8)) - sysml_conditional_21;
    double sysml_value_12 = 0;
    if ((sysml_state_count * 1.8) < ((double)((int)(sysml_state_count * 1.8)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_value_12 = ((int)(sysml_state_count * 1.8)) - sysml_conditional_22;
    *io_0 = sysml_value_0;
    *io_1_segment_0 = sysml_value_1;
    *io_1_segment_1 = sysml_value_2;
    *io_1_segment_2 = sysml_value_3;
    *io_1_segment_3 = sysml_value_4;
    *io_1_segment_4 = sysml_value_5;
    *io_1_segment_5 = sysml_value_6;
    *io_1_segment_6 = sysml_value_7;
    *io_1_segment_7 = sysml_value_8;
    *io_1_segment_8 = sysml_value_9;
    *io_1_segment_9 = sysml_value_10;
    *io_2 = sysml_value_11;
    *io_3 = sysml_value_12;
}