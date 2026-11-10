#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_mappedLevel = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_mappedLevel = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, bool* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int sysml_conditional_0 = 0;
    int sysml_conditional_1 = 0;
    int sysml_conditional_2 = 0;
    int sysml_conditional_3 = 0;
    int sysml_conditional_4 = 0;
    if (io_0 <= 0.0) {
        sysml_conditional_0 = 10;
    }
    else {
        if (io_0 >= 1023.0) {
            sysml_conditional_1 = 0;
        }
        else {
            if ((((double)(1023.0 - io_0)) / 102.3) < ((double)((int)(((double)(1023.0 - io_0)) / 102.3)))) {
                sysml_conditional_2 = 1;
            }
            else {
                sysml_conditional_2 = 0;
            }
            if ((((int)(((double)(1023.0 - io_0)) / 102.3)) - sysml_conditional_2) >= 10) {
                sysml_conditional_3 = 10;
            }
            else {
                if ((((double)(1023.0 - io_0)) / 102.3) < ((double)((int)(((double)(1023.0 - io_0)) / 102.3)))) {
                    sysml_conditional_4 = 1;
                }
                else {
                    sysml_conditional_4 = 0;
                }
                sysml_conditional_3 = ((int)(((double)(1023.0 - io_0)) / 102.3)) - sysml_conditional_4;
            }
            sysml_conditional_1 = sysml_conditional_3;
        }
        sysml_conditional_0 = sysml_conditional_1;
    }
    sysml_state_mappedLevel = sysml_conditional_0;
    bool sysml_value_0 = 0;
    sysml_value_0 = (sysml_state_mappedLevel > 0);
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_mappedLevel > 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_mappedLevel > 2);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_mappedLevel > 3);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_mappedLevel > 4);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_mappedLevel > 5);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_mappedLevel > 6);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_mappedLevel > 7);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_mappedLevel > 8);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_mappedLevel > 9);
    *io_1 = sysml_value_0;
    *io_2 = sysml_value_1;
    *io_3 = sysml_value_2;
    *io_4 = sysml_value_3;
    *io_5 = sysml_value_4;
    *io_6 = sysml_value_5;
    *io_7 = sysml_value_6;
    *io_8 = sysml_value_7;
    *io_9 = sysml_value_8;
    *io_10 = sysml_value_9;
}