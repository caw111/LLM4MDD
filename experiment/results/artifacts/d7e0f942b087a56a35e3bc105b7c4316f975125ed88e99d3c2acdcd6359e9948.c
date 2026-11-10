#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_elapsedModeMs = 0;
/* Global Variable define of  */
int sysml_state_modeIndex = 0;
/* Global Variable define of  */
int sysml_state_currentRed = 0;
/* Global Variable define of  */
int sysml_state_currentGreen = 0;
/* Global Variable define of  */
int sysml_state_currentBlue = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_elapsedModeMs = 0;
    sysml_state_modeIndex = 0;
    sysml_state_currentRed = 0;
    sysml_state_currentGreen = 0;
    sysml_state_currentBlue = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double* io_0, double* io_1, double* io_2, bool* io_4_segment_0, bool* io_4_segment_1, bool* io_4_segment_2, bool* io_4_segment_3, bool* io_4_segment_4, bool* io_4_segment_5, bool* io_4_segment_6, bool* io_4_segment_7, bool* io_4_segment_8, bool* io_4_segment_9) {
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
    int sysml_conditional_15 = 0;
    sysml_state_elapsedModeMs = 0;
    sysml_state_modeIndex = 0;
    if (sysml_state_modeIndex == 0) {
        if (io_3 < ((double)((int)io_3))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)io_3) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = sysml_state_currentRed;
    }
    sysml_state_currentRed = sysml_conditional_0;
    if (sysml_state_modeIndex == 1) {
        if (io_3 < ((double)((int)io_3))) {
            sysml_conditional_3 = 1;
        }
        else {
            sysml_conditional_3 = 0;
        }
        sysml_conditional_2 = ((int)io_3) - sysml_conditional_3;
    }
    else {
        sysml_conditional_2 = sysml_state_currentGreen;
    }
    sysml_state_currentGreen = sysml_conditional_2;
    if (sysml_state_modeIndex == 2) {
        if (io_3 < ((double)((int)io_3))) {
            sysml_conditional_5 = 1;
        }
        else {
            sysml_conditional_5 = 0;
        }
        sysml_conditional_4 = ((int)io_3) - sysml_conditional_5;
    }
    else {
        sysml_conditional_4 = sysml_state_currentBlue;
    }
    sysml_state_currentBlue = sysml_conditional_4;
    double sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_currentRed * 1.0);
    double sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_currentGreen * 1.0);
    double sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_currentBlue * 1.0);
    bool sysml_value_3 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_6 = 1;
    }
    else {
        sysml_conditional_6 = 0;
    }
    sysml_value_3 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_6) >= 1;
    bool sysml_value_4 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_7 = 1;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_4 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_7) >= 2;
    bool sysml_value_5 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_8 = 1;
    }
    else {
        sysml_conditional_8 = 0;
    }
    sysml_value_5 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_8) >= 3;
    bool sysml_value_6 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_9 = 1;
    }
    else {
        sysml_conditional_9 = 0;
    }
    sysml_value_6 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_9) >= 4;
    bool sysml_value_7 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_10 = 1;
    }
    else {
        sysml_conditional_10 = 0;
    }
    sysml_value_7 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_10) >= 5;
    bool sysml_value_8 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_11 = 1;
    }
    else {
        sysml_conditional_11 = 0;
    }
    sysml_value_8 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_11) >= 6;
    bool sysml_value_9 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_12 = 1;
    }
    else {
        sysml_conditional_12 = 0;
    }
    sysml_value_9 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_12) >= 7;
    bool sysml_value_10 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_13 = 1;
    }
    else {
        sysml_conditional_13 = 0;
    }
    sysml_value_10 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_13) >= 8;
    bool sysml_value_11 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_14 = 1;
    }
    else {
        sysml_conditional_14 = 0;
    }
    sysml_value_11 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_14) >= 9;
    bool sysml_value_12 = 0;
    if ((((double)(io_3 * 10.0)) / 1023.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1023.0)))) {
        sysml_conditional_15 = 1;
    }
    else {
        sysml_conditional_15 = 0;
    }
    sysml_value_12 = (((int)(((double)(io_3 * 10.0)) / 1023.0)) - sysml_conditional_15) >= 10;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_4_segment_0 = sysml_value_3;
    *io_4_segment_1 = sysml_value_4;
    *io_4_segment_2 = sysml_value_5;
    *io_4_segment_3 = sysml_value_6;
    *io_4_segment_4 = sysml_value_7;
    *io_4_segment_5 = sysml_value_8;
    *io_4_segment_6 = sysml_value_9;
    *io_4_segment_7 = sysml_value_10;
    *io_4_segment_8 = sysml_value_11;
    *io_4_segment_9 = sysml_value_12;
}