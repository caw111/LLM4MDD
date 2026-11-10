#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7, double* io_8, double* io_9, double* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    bool sysml_lazy_0 = 0;
    double sysml_conditional_1 = 0;
    sysml_value_0 = ((double)((io_0 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_1 * 255.0)) / 1023.0);
    double sysml_value_2 = 0;
    sysml_lazy_0 = io_0 == 1023.0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_1 == 1023.0;
    }
    if (sysml_lazy_0) {
        sysml_conditional_1 = 255.0;
    }
    else {
        sysml_conditional_1 = 0.0;
    }
    sysml_value_2 = sysml_conditional_1;
    double sysml_value_3 = 0;
    sysml_value_3 = ((double)((io_0 * 255.0)) / 1023.0);
    double sysml_value_4 = 0;
    sysml_value_4 = ((double)((io_1 * 255.0)) / 1023.0);
    double sysml_value_5 = 0;
    sysml_value_5 = ((double)(((io_0 + io_1) * 255.0)) / 2046.0);
    double sysml_value_6 = 0;
    sysml_value_6 = ((double)((io_0 * 255.0)) / 1023.0);
    double sysml_value_7 = 0;
    sysml_value_7 = ((double)((io_1 * 255.0)) / 1023.0);
    double sysml_value_8 = 0;
    sysml_value_8 = ((double)(((io_0 + io_1) * 255.0)) / 2046.0);
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
}