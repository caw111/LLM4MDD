#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_number = 0;
/* Global Variable define of  */
bool sysml_state_previousPressed = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_number = 0;
    sysml_state_previousPressed = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, int* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    int sysml_conditional_1 = 0;
    sysml_lazy_0 = io_0;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousPressed;
    }
    if (sysml_lazy_0) {
        if (sysml_state_number >= 9) {
            sysml_conditional_1 = 0;
        }
        else {
            sysml_conditional_1 = sysml_state_number + 1;
        }
        sysml_state_number = sysml_conditional_1;
    }
    sysml_state_previousPressed = io_0;
    int sysml_value_0 = 0;
    sysml_value_0 = sysml_state_number;
    bool sysml_value_1 = 0;
    sysml_value_1 = (sysml_state_number >= 1);
    bool sysml_value_2 = 0;
    sysml_value_2 = (sysml_state_number >= 2);
    bool sysml_value_3 = 0;
    sysml_value_3 = (sysml_state_number >= 3);
    bool sysml_value_4 = 0;
    sysml_value_4 = (sysml_state_number >= 4);
    bool sysml_value_5 = 0;
    sysml_value_5 = (sysml_state_number >= 5);
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_number >= 6);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_number >= 7);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_number >= 8);
    bool sysml_value_9 = 0;
    sysml_value_9 = (sysml_state_number >= 9);
    bool sysml_value_10 = 0;
    sysml_value_10 = false;
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
    *io_11 = sysml_value_10;
}