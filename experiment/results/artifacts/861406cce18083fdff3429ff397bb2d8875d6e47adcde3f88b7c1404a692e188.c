#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_3, double io_4, double* io_0, double* io_1, double* io_2, int* io_5) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    int sysml_conditional_0 = 0;
    sysml_value_0 = ((double)((io_3 * 255.0)) / 1023.0);
    double sysml_value_1 = 0;
    sysml_value_1 = ((double)((io_3 * 255.0)) / 1023.0);
    double sysml_value_2 = 0;
    sysml_value_2 = ((double)((io_4 * 255.0)) / 1023.0);
    int sysml_value_3 = 0;
    if ((((double)(((double)(((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0)) + (((double)(io_4 * 255.0)) / 1023.0))) / 3.0)) / 28.33) < ((double)((int)(((double)(((double)(((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0)) + (((double)(io_4 * 255.0)) / 1023.0))) / 3.0)) / 28.33)))) {
        sysml_conditional_0 = 1;
    }
    else {
        sysml_conditional_0 = 0;
    }
    sysml_value_3 = ((int)(((double)(((double)(((((double)(io_3 * 255.0)) / 1023.0) + (((double)(io_3 * 255.0)) / 1023.0)) + (((double)(io_4 * 255.0)) / 1023.0))) / 3.0)) / 28.33)) - sysml_conditional_0;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_5 = sysml_value_3;
}