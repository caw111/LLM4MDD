#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_servo3State = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo3State = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, bool* io_4, double* io_5, double* io_6, bool* io_7, double* io_8, double* io_9, bool* io_10, double* io_11, double* io_12, double* io_13) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    double sysml_conditional_5 = 0;
    double sysml_conditional_6 = 0;
    double sysml_conditional_7 = 0;
    sysml_state_servo3State = sysml_state_servo3State;
    double sysml_value_0 = 0;
    if (io_0 < ((double)((int)io_0))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_0 = ((int)io_0) - sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_value_1 = 0.0;
    bool sysml_value_2 = 0;
    sysml_value_2 = false;
    double sysml_value_3 = 0;
    if (io_0 < ((double)((int)io_0))) {
        sysml_conditional_1 = 1;
    }
    else {
        sysml_conditional_1 = 0;
    }
    sysml_value_3 = ((int)io_0) - sysml_conditional_1;
    double sysml_value_4 = 0;
    sysml_value_4 = 0.0;
    bool sysml_value_5 = 0;
    sysml_value_5 = false;
    double sysml_value_6 = 0;
    if (io_0 < ((double)((int)io_0))) {
        sysml_conditional_2 = 1;
    }
    else {
        sysml_conditional_2 = 0;
    }
    sysml_value_6 = ((int)io_0) - sysml_conditional_2;
    double sysml_value_7 = 0;
    sysml_value_7 = 0.0;
    bool sysml_value_8 = 0;
    sysml_value_8 = false;
    double sysml_value_9 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_3 = 0.0;
    }
    else {
        if (io_0 >= 255.0) {
            sysml_conditional_4 = 180.0;
        }
        else {
            sysml_conditional_4 = ((double)(io_0 * 180.0)) / 255.0;
        }
        sysml_conditional_3 = sysml_conditional_4;
    }
    sysml_value_9 = sysml_conditional_3;
    double sysml_value_10 = 0;
    if (io_1 <= 0.0) {
        sysml_conditional_5 = 0.0;
    }
    else {
        if (io_1 >= 255.0) {
            sysml_conditional_6 = 180.0;
        }
        else {
            sysml_conditional_6 = ((double)(io_1 * 180.0)) / 255.0;
        }
        sysml_conditional_5 = sysml_conditional_6;
    }
    sysml_value_10 = sysml_conditional_5;
    double sysml_value_11 = 0;
    if (sysml_state_servo3State) {
        sysml_conditional_7 = 180.0;
    }
    else {
        sysml_conditional_7 = 0.0;
    }
    sysml_value_11 = sysml_conditional_7;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
    *io_11 = sysml_value_9;
    *io_12 = sysml_value_10;
    *io_13 = sysml_value_11;
}