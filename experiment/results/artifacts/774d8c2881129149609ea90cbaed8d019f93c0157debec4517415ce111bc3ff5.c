#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_servo1Angle = 0.0;
/* Global Variable define of  */
double sysml_state_servo2Angle = 0.0;
/* Global Variable define of  */
bool sysml_state_rgbRed = false;
/* Global Variable define of  */
bool sysml_state_rgbGreen = false;
/* Global Variable define of  */
bool sysml_state_rgbBlue = false;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_servo1Angle = 0.0;
    sysml_state_servo2Angle = 0.0;
    sysml_state_rgbRed = false;
    sysml_state_rgbGreen = false;
    sysml_state_rgbBlue = false;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_2, bool io_3, double* io_0, double* io_1, bool* io_4, bool* io_5, bool* io_6) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = io_3;
    }
    if (sysml_lazy_0) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_rgbRed = false;
        sysml_state_rgbGreen = true;
        sysml_state_rgbBlue = false;
    }
    sysml_lazy_1 = io_2;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !io_3;
    }
    if (sysml_lazy_1) {
        sysml_state_servo1Angle = 90.0;
        sysml_state_servo2Angle = 0.0;
        sysml_state_rgbRed = true;
        sysml_state_rgbGreen = false;
        sysml_state_rgbBlue = false;
    }
    sysml_lazy_2 = !io_2;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_3;
    }
    if (sysml_lazy_2) {
        sysml_state_servo1Angle = 0.0;
        sysml_state_servo2Angle = 180.0;
        sysml_state_rgbRed = false;
        sysml_state_rgbGreen = false;
        sysml_state_rgbBlue = true;
    }
    sysml_lazy_3 = !io_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !io_3;
    }
    if (sysml_lazy_3) {
        sysml_state_rgbRed = false;
        sysml_state_rgbGreen = false;
        sysml_state_rgbBlue = false;
    }
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_servo1Angle;
    double sysml_value_1 = 0;
    sysml_value_1 = sysml_state_servo2Angle;
    bool sysml_value_2 = 0;
    sysml_value_2 = sysml_state_rgbRed;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_rgbGreen;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_rgbBlue;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_4 = sysml_value_2;
    *io_5 = sysml_value_3;
    *io_6 = sysml_value_4;
}