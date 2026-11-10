#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, double* io_2, double* io_3, bool* io_4, bool* io_5, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    double sysml_conditional_3 = 0;
    double sysml_conditional_4 = 0;
    bool sysml_conditional_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_conditional_7 = 0;
    bool sysml_conditional_8 = 0;
    if (io_0 >= 0.0) {
        if (io_0 <= 341.0) {
            sysml_conditional_1 = 255.0;
        }
        else {
            sysml_conditional_1 = 0.0;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_value_0 = sysml_conditional_0;
    double sysml_value_1 = 0;
    sysml_lazy_2 = io_0 >= 342.0;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0 <= 682.0;
    }
    if (sysml_lazy_2) {
        sysml_conditional_3 = 255.0;
    }
    else {
        sysml_conditional_3 = 0.0;
    }
    sysml_value_1 = sysml_conditional_3;
    double sysml_value_2 = 0;
    if (io_0 >= 683.0) {
        sysml_conditional_4 = 255.0;
    }
    else {
        sysml_conditional_4 = 0.0;
    }
    sysml_value_2 = sysml_conditional_4;
    bool sysml_value_3 = 0;
    if (io_0 <= 341.0) {
        sysml_conditional_5 = true;
    }
    else {
        sysml_conditional_5 = false;
    }
    sysml_value_3 = sysml_conditional_5;
    bool sysml_value_4 = 0;
    sysml_lazy_6 = io_0 >= 342.0;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_0 <= 682.0;
    }
    if (sysml_lazy_6) {
        sysml_conditional_7 = true;
    }
    else {
        sysml_conditional_7 = false;
    }
    sysml_value_4 = sysml_conditional_7;
    bool sysml_value_5 = 0;
    if (io_0 >= 683.0) {
        sysml_conditional_8 = true;
    }
    else {
        sysml_conditional_8 = false;
    }
    sysml_value_5 = sysml_conditional_8;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
}