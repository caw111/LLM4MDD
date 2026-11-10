#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_totalCount = 0.0;
/* Global Variable define of  */
int sysml_state_displayDigit = 0;
/* Global Variable define of  */
bool sysml_state_motionArmed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_totalCount = 0.0;
    sysml_state_displayDigit = 0;
    sysml_state_motionArmed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(int io_0, bool io_1, int* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_arithmetic_4 = 0;
    if (io_1) {
        sysml_lazy_0 = io_0 > (-2000);
        if (sysml_lazy_0) {
            sysml_lazy_0 = io_0 < 2000;
        }
        if (sysml_lazy_0) {
            sysml_state_motionArmed = true;
        }
        else {
            sysml_lazy_1 = sysml_state_motionArmed;
            if (sysml_lazy_1) {
                sysml_lazy_2 = io_0 > 15000;
                if (!sysml_lazy_2) {
                    sysml_lazy_2 = io_0 < (-15000);
                }
                sysml_lazy_1 = sysml_lazy_2;
            }
            if (sysml_lazy_1) {
                sysml_state_totalCount = (sysml_state_totalCount + 1.0);
                sysml_state_displayDigit = (sysml_state_displayDigit + 1);
                if ((((double)sysml_state_displayDigit) / 10) < ((double)((int)(((double)sysml_state_displayDigit) / 10)))) {
                    sysml_conditional_3 = 1;
                }
                else {
                    sysml_conditional_3 = 0;
                }
                sysml_arithmetic_4 = ((int)(((double)sysml_state_displayDigit) / 10)) - sysml_conditional_3;
                sysml_state_displayDigit = sysml_state_displayDigit - (10 * sysml_arithmetic_4);
                sysml_state_motionArmed = false;
            }
        }
    }
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_displayDigit;
    *io_2 = sysml_value_0;
}