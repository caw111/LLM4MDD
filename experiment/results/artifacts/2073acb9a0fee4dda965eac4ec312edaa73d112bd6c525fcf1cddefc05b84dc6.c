#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_6, double io_7, double io_8, double* io_0, double* io_1, double* io_2, double* io_3, double* io_4, double* io_5, int* io_9) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    sysml_value_0 = ((double)((io_6 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_7 * 255.0)) / 1023.0);
    double sysml_value_2 = 0;
    sysml_value_2 = ((double)((io_8 * 255.0)) / 1023.0);
    double sysml_value_3 = 0;
    sysml_value_3 = ((double)((io_6 * 255.0)) / 1023.0);
    double sysml_value_4 = 0;
    sysml_value_4 = ((double)((io_7 * 255.0)) / 1023.0);
    double sysml_value_5 = 0;
    sysml_value_5 = ((double)((io_8 * 255.0)) / 1023.0);
    int sysml_value_6 = 0;
    if (io_8 >= 0.0) {
        if ((((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0) < ((double)((int)(((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_conditional_0 = ((int)(((((double)(io_6 * 255.0)) / 1023.0) + (io_8 * 0.0)) * 0.0)) - sysml_conditional_1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_6 = sysml_conditional_0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_9 = sysml_value_6;
}