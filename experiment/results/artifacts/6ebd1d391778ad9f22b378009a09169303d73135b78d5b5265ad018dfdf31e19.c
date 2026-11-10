#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_initialized = false;
/* Global Variable define of  */
int sysml_state_displayNumber = 0;
/* Global Variable define of  */
int sysml_state_previousTensDigit = 0;
/* Global Variable define of  */
int sysml_state_previousUnitsDigit = 0;
/* Global Variable define of  */
bool sysml_state_previousIncrementPressed = false;
/* Global Variable define of  */
bool sysml_state_previousDecrementPressed = false;
/* Global Variable define of  */
double sysml_state_incrementElapsed = 0.0;
/* Global Variable define of  */
double sysml_state_decrementElapsed = 0.0;
/* Global Variable define of  */
int sysml_state_incrementSteps = 0;
/* Global Variable define of  */
int sysml_state_decrementSteps = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_initialized = false;
    sysml_state_displayNumber = 0;
    sysml_state_previousTensDigit = 0;
    sysml_state_previousUnitsDigit = 0;
    sysml_state_previousIncrementPressed = false;
    sysml_state_previousDecrementPressed = false;
    sysml_state_incrementElapsed = 0.0;
    sysml_state_decrementElapsed = 0.0;
    sysml_state_incrementSteps = 0;
    sysml_state_decrementSteps = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, double io_2, double io_3, int io_5, int* io_4) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    int sysml_arithmetic_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_arithmetic_8 = 0;
    int sysml_conditional_9 = 0;
    double sysml_arithmetic_10 = 0;
    double sysml_arithmetic_11 = 0;
    double sysml_arithmetic_12 = 0;
    int sysml_conditional_13 = 0;
    double sysml_arithmetic_14 = 0;
    double sysml_arithmetic_15 = 0;
    double sysml_arithmetic_16 = 0;
    double sysml_arithmetic_17 = 0;
    int sysml_conditional_18 = 0;
    double sysml_arithmetic_19 = 0;
    int sysml_conditional_20 = 0;
    int sysml_arithmetic_21 = 0;
    int sysml_conditional_22 = 0;
    int sysml_conditional_23 = 0;
    int sysml_conditional_24 = 0;
    int sysml_conditional_25 = 0;
    if ((!sysml_state_initialized)) {
        if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1024.0)))) {
            sysml_conditional_0 = 1;
        }
        else {
            sysml_conditional_0 = 0;
        }
        if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1024.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_state_displayNumber = ((((int)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_0) * 10) + (((int)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_1);
        sysml_state_initialized = true;
    }
    else {
        if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1024.0)))) {
            sysml_conditional_2 = 1;
        }
        else {
            sysml_conditional_2 = 0;
        }
        if ((((int)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_2) != sysml_state_previousTensDigit) {
            if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1024.0)))) {
                sysml_conditional_3 = 1;
            }
            else {
                sysml_conditional_3 = 0;
            }
            if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
                sysml_conditional_4 = 1;
            }
            else {
                sysml_conditional_4 = 0;
            }
            sysml_arithmetic_5 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_4;
            sysml_state_displayNumber = ((((int)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_3) * 10) + (sysml_state_displayNumber - (10 * sysml_arithmetic_5));
        }
        if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1024.0)))) {
            sysml_conditional_6 = 1;
        }
        else {
            sysml_conditional_6 = 0;
        }
        if ((((int)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_6) != sysml_state_previousUnitsDigit) {
            if ((((double)sysml_state_displayNumber) / 10) < ((double)((int)(((double)sysml_state_displayNumber) / 10)))) {
                sysml_conditional_7 = 1;
            }
            else {
                sysml_conditional_7 = 0;
            }
            sysml_arithmetic_8 = ((int)(((double)sysml_state_displayNumber) / 10)) - sysml_conditional_7;
            if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1024.0)))) {
                sysml_conditional_9 = 1;
            }
            else {
                sysml_conditional_9 = 0;
            }
            sysml_state_displayNumber = (10 * sysml_arithmetic_8) + (((int)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_9);
        }
    }
    sysml_state_incrementSteps = 0;
    sysml_state_decrementSteps = 0;
    if (io_0) {
        if ((!sysml_state_previousIncrementPressed)) {
            sysml_state_incrementSteps = 1;
            sysml_state_incrementElapsed = 0.0;
        }
        else {
            sysml_state_incrementElapsed = (sysml_state_incrementElapsed + io_5);
            sysml_arithmetic_10 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_arithmetic_11 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_arithmetic_12 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            if ((((double)sysml_state_incrementElapsed) / sysml_arithmetic_11) < ((double)((int)(((double)sysml_state_incrementElapsed) / sysml_arithmetic_12)))) {
                sysml_conditional_13 = 1;
            }
            else {
                sysml_conditional_13 = 0;
            }
            sysml_state_incrementSteps = ((int)(((double)sysml_state_incrementElapsed) / sysml_arithmetic_10)) - sysml_conditional_13;
            sysml_arithmetic_14 = 100.0 + (((double)(io_2 * 900.0)) / 1023.0);
            sysml_state_incrementElapsed = sysml_state_incrementElapsed - (sysml_state_incrementSteps * sysml_arithmetic_14);
        }
    }
    else {
        sysml_state_incrementElapsed = 0.0;
    }
    if (io_1) {
        if ((!sysml_state_previousDecrementPressed)) {
            sysml_state_decrementSteps = 1;
            sysml_state_decrementElapsed = 0.0;
        }
        else {
            sysml_state_decrementElapsed = (sysml_state_decrementElapsed + io_5);
            sysml_arithmetic_15 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_arithmetic_16 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_arithmetic_17 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            if ((((double)sysml_state_decrementElapsed) / sysml_arithmetic_16) < ((double)((int)(((double)sysml_state_decrementElapsed) / sysml_arithmetic_17)))) {
                sysml_conditional_18 = 1;
            }
            else {
                sysml_conditional_18 = 0;
            }
            sysml_state_decrementSteps = ((int)(((double)sysml_state_decrementElapsed) / sysml_arithmetic_15)) - sysml_conditional_18;
            sysml_arithmetic_19 = 100.0 + (((double)(io_3 * 900.0)) / 1023.0);
            sysml_state_decrementElapsed = sysml_state_decrementElapsed - (sysml_state_decrementSteps * sysml_arithmetic_19);
        }
    }
    else {
        sysml_state_decrementElapsed = 0.0;
    }
    sysml_state_displayNumber = ((sysml_state_displayNumber + sysml_state_incrementSteps) - sysml_state_decrementSteps);
    if ((((double)sysml_state_displayNumber) / 100) < ((double)((int)(((double)sysml_state_displayNumber) / 100)))) {
        sysml_conditional_20 = 1;
    }
    else {
        sysml_conditional_20 = 0;
    }
    sysml_arithmetic_21 = ((int)(((double)sysml_state_displayNumber) / 100)) - sysml_conditional_20;
    sysml_state_displayNumber = sysml_state_displayNumber - (100 * sysml_arithmetic_21);
    if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1024.0)))) {
        sysml_conditional_22 = 1;
    }
    else {
        sysml_conditional_22 = 0;
    }
    sysml_state_previousTensDigit = ((int)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_22;
    if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1024.0)))) {
        sysml_conditional_23 = 1;
    }
    else {
        sysml_conditional_23 = 0;
    }
    sysml_state_previousUnitsDigit = ((int)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_23;
    sysml_state_previousIncrementPressed = io_0;
    sysml_state_previousDecrementPressed = io_1;
    int sysml_value_0 = 0;
    if ((((double)(io_2 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_2 * 10.0)) / 1024.0)))) {
        sysml_conditional_24 = 1;
    }
    else {
        sysml_conditional_24 = 0;
    }
    sysml_value_0 = ((int)(((double)(io_2 * 10.0)) / 1024.0)) - sysml_conditional_24;
    int sysml_value_1 = 0;
    if ((((double)(io_3 * 10.0)) / 1024.0) < ((double)((int)(((double)(io_3 * 10.0)) / 1024.0)))) {
        sysml_conditional_25 = 1;
    }
    else {
        sysml_conditional_25 = 0;
    }
    sysml_value_1 = ((int)(((double)(io_3 * 10.0)) / 1024.0)) - sysml_conditional_25;
    double sysml_value_2 = 0;
    sysml_value_2 = (100.0 + ((double)((io_2 * 900.0)) / 1023.0));
    double sysml_value_3 = 0;
    sysml_value_3 = (100.0 + ((double)((io_3 * 900.0)) / 1023.0));
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_displayNumber;
    *io_4 = sysml_value_4;
}