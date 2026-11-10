#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
int sysml_state_debounceTimer1_ms = 0;
/* Global Variable define of  */
int sysml_state_debounceTimer2_ms = 0;
/* Global Variable define of  */
int sysml_state_debounceTimer3_ms = 0;
/* Global Variable define of  */
bool sysml_state_stablePressed1 = false;
/* Global Variable define of  */
bool sysml_state_stablePressed2 = false;
/* Global Variable define of  */
bool sysml_state_stablePressed3 = false;
/* Global Variable define of  */
bool sysml_state_prevStablePressed1 = false;
/* Global Variable define of  */
bool sysml_state_prevStablePressed2 = false;
/* Global Variable define of  */
bool sysml_state_prevStablePressed3 = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_debounceTimer1_ms = 0;
    sysml_state_debounceTimer2_ms = 0;
    sysml_state_debounceTimer3_ms = 0;
    sysml_state_stablePressed1 = false;
    sysml_state_stablePressed2 = false;
    sysml_state_stablePressed3 = false;
    sysml_state_prevStablePressed1 = false;
    sysml_state_prevStablePressed2 = false;
    sysml_state_prevStablePressed3 = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, int* io_3, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    int sysml_conditional_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    int sysml_conditional_10 = 0;
    int sysml_conditional_11 = 0;
    int sysml_conditional_12 = 0;
    int sysml_conditional_13 = 0;
    int sysml_conditional_14 = 0;
    int sysml_conditional_15 = 0;
    int sysml_conditional_16 = 0;
    int sysml_conditional_17 = 0;
    int sysml_conditional_18 = 0;
    int sysml_conditional_19 = 0;
    int sysml_conditional_20 = 0;
    if (io_2) {
        if ((sysml_state_debounceTimer3_ms > 30)) {
            sysml_state_stablePressed3 = true;
        }
        else {
            sysml_state_debounceTimer3_ms = 0;
        }
    }
    else {
        sysml_state_debounceTimer3_ms = 0;
        sysml_state_stablePressed3 = false;
    }
    if (io_0) {
        if ((sysml_state_debounceTimer1_ms > 30)) {
            sysml_state_stablePressed1 = true;
        }
        else {
            sysml_state_debounceTimer1_ms = 0;
        }
    }
    else {
        sysml_state_debounceTimer1_ms = 0;
        sysml_state_stablePressed1 = false;
    }
    if (io_1) {
        if ((sysml_state_debounceTimer2_ms > 30)) {
            sysml_state_stablePressed2 = true;
        }
        else {
            sysml_state_debounceTimer2_ms = 0;
        }
    }
    else {
        sysml_state_debounceTimer2_ms = 0;
        sysml_state_stablePressed2 = false;
    }
    sysml_lazy_0 = sysml_state_stablePressed3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevStablePressed3;
    }
    if (sysml_lazy_0) {
        sysml_state_number = 0;
    }
    sysml_lazy_1 = sysml_state_stablePressed3;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_prevStablePressed3;
    }
    sysml_lazy_2 = !sysml_lazy_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = sysml_state_stablePressed1;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_prevStablePressed1;
    }
    if (sysml_lazy_3) {
        if (sysml_state_number >= 99) {
            sysml_conditional_4 = 0;
        }
        else {
            sysml_conditional_4 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_4;
    }
    sysml_lazy_5 = sysml_state_stablePressed3;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_prevStablePressed3;
    }
    sysml_lazy_6 = !sysml_lazy_5;
    if (sysml_lazy_6) {
        sysml_lazy_6 = sysml_state_stablePressed2;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_prevStablePressed2;
    }
    if (sysml_lazy_7) {
        if (sysml_state_number >= 95) {
            sysml_conditional_8 = 0;
        }
        else {
            sysml_conditional_8 = sysml_state_number + 5;
        }
        sysml_state_number = sysml_conditional_8;
    }
    sysml_state_prevStablePressed1 = sysml_state_stablePressed1;
    sysml_state_prevStablePressed2 = sysml_state_stablePressed2;
    sysml_state_prevStablePressed3 = sysml_state_stablePressed3;
    int sysml_value_0 = 0;
    sysml_lazy_9 = sysml_state_number < 0;
    if (!sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_number > 99;
    }
    if (sysml_lazy_9) {
        sysml_conditional_10 = 0;
    }
    else {
        sysml_conditional_10 = sysml_state_number;
    }
    sysml_value_0 = sysml_conditional_10;
    bool sysml_value_1 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_1 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_11) >= 1;
    bool sysml_value_2 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_2 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_12) >= 2;
    bool sysml_value_3 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_3 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_13) >= 3;
    bool sysml_value_4 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_4 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_14) >= 4;
    bool sysml_value_5 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    sysml_value_5 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_15) >= 5;
    bool sysml_value_6 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_16 = 1;
    }
    else {
        sysml_conditional_16 = 0;
    }
    sysml_value_6 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_16) >= 6;
    bool sysml_value_7 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_17 = 1;
    }
    else {
        sysml_conditional_17 = 0;
    }
    sysml_value_7 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_17) >= 7;
    bool sysml_value_8 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_18 = 1;
    }
    else {
        sysml_conditional_18 = 0;
    }
    sysml_value_8 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_18) >= 8;
    bool sysml_value_9 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_19 = 1;
    }
    else {
        sysml_conditional_19 = 0;
    }
    sysml_value_9 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_19) >= 9;
    bool sysml_value_10 = 0;
    if ((((double)sysml_state_number) / 10) < ((double)((int)(((double)sysml_state_number) / 10)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_value_10 = (((int)(((double)sysml_state_number) / 10)) - sysml_conditional_20) >= 10;
    *io_3 = sysml_value_0;
    *io_4_segment_0 = sysml_value_1;
    *io_4_segment_1 = sysml_value_2;
    *io_4_segment_2 = sysml_value_3;
    *io_4_segment_3 = sysml_value_4;
    *io_4_segment_4 = sysml_value_5;
    *io_4_segment_5 = sysml_value_6;
    *io_4_segment_6 = sysml_value_7;
    *io_4_segment_7 = sysml_value_8;
    *io_4_segment_8 = sysml_value_9;
    *io_4_segment_9 = sysml_value_10;
}