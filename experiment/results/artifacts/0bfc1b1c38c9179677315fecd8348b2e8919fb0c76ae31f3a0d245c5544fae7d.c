#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    double sysml_conditional_0 = 0;
    double sysml_arithmetic_1 = 0;
    double sysml_arithmetic_2 = 0;
    double sysml_arithmetic_3 = 0;
    int sysml_conditional_4 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 1023.0;
    }
    else {
        sysml_arithmetic_1 = 1.0 - (((double)io_0) / 1023.0);
        sysml_arithmetic_2 = 1.0 - (((double)io_0) / 1023.0);
        sysml_arithmetic_3 = 1.0 - (((double)io_0) / 1023.0);
        if ((1023.0 * sysml_arithmetic_2) < ((double)((int)(1023.0 * sysml_arithmetic_3)))) {
            sysml_conditional_4 = 1;
        }
        else {
            sysml_conditional_4 = 0;
        }
        sysml_conditional_0 = ((int)(1023.0 * sysml_arithmetic_1)) - sysml_conditional_4;
    }
    sysml_value_0 = sysml_conditional_0;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
}