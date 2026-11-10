#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_rgb1R = 0.0;
/* Global Variable define of  */
double sysml_state_rgb1G = 0.0;
/* Global Variable define of  */
double sysml_state_rgb1B = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2R = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2G = 0.0;
/* Global Variable define of  */
double sysml_state_rgb2B = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_rgb1R = 0.0;
    sysml_state_rgb1G = 0.0;
    sysml_state_rgb1B = 0.0;
    sysml_state_rgb2R = 0.0;
    sysml_state_rgb2G = 0.0;
    sysml_state_rgb2B = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_0, double io_1, double* io_2, double* io_3, double* io_4, double* io_5, double* io_6, double* io_7) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    sysml_state_rgb1R = 0.0;
    sysml_state_rgb1G = 0.0;
    sysml_state_rgb1B = 0.0;
    sysml_state_rgb2R = 0.0;
    sysml_state_rgb2G = 0.0;
    sysml_state_rgb2B = 0.0;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_rgb1R;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_rgb1G;
    double sysml_value_2 = 0;
    sysml_value_2 = sysml_state_rgb1B;
    double sysml_value_3 = 0;
    sysml_value_3 = sysml_state_rgb2R;
    double sysml_value_4 = 0;
    sysml_value_4 = sysml_state_rgb2G;
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_rgb2B;
    *io_2 = sysml_value_0;
    *io_3 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
    *io_7 = sysml_value_5;
}