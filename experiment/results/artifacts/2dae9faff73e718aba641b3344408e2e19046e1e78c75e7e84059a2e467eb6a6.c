#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_12, double io_13, double io_14, double* io_0, double* io_1, bool* io_2, bool* io_3, bool* io_4, bool* io_5, bool* io_6, bool* io_7, bool* io_8, bool* io_9, bool* io_10, bool* io_11) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_value_0 = 0;
    sysml_value_0 = io_12;
    double sysml_value_1 = 0;
    sysml_value_1 = io_13;
    bool sysml_value_2 = 0;
    sysml_value_2 = (io_14 >= 1);
    bool sysml_value_3 = 0;
    sysml_value_3 = (io_14 >= 2);
    bool sysml_value_4 = 0;
    sysml_value_4 = (io_14 >= 3);
    bool sysml_value_5 = 0;
    sysml_value_5 = (io_14 >= 4);
    bool sysml_value_6 = 0;
    sysml_value_6 = (io_14 >= 5);
    bool sysml_value_7 = 0;
    sysml_value_7 = (io_14 >= 6);
    bool sysml_value_8 = 0;
    sysml_value_8 = (io_14 >= 7);
    bool sysml_value_9 = 0;
    sysml_value_9 = (io_14 >= 8);
    bool sysml_value_10 = 0;
    sysml_value_10 = (io_14 >= 9);
    bool sysml_value_11 = 0;
    sysml_value_11 = (io_14 >= 10);
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
    *io_3 = sysml_value_3;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_6;
    *io_7 = sysml_value_7;
    *io_8 = sysml_value_8;
    *io_9 = sysml_value_9;
    *io_10 = sysml_value_10;
    *io_11 = sysml_value_11;
}