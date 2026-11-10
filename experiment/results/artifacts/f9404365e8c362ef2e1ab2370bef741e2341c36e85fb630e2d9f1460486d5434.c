#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
double sysml_state_led_brightness_internal = 0.0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_led_brightness_internal = 0.0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(double io_1, double io_2, double io_3, double* io_0, bool* serial_0_trigger, double* serial_0_value_0, double* serial_0_value_1, double* serial_0_value_2, double* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    double sysml_conditional_0 = 0;
    double sysml_arithmetic_1 = 0;
    if (io_1 >= io_2) {
        sysml_arithmetic_1 = io_1 - io_2;
        sysml_conditional_0 = io_2 + (io_3 * sysml_arithmetic_1);
    }
    else {
        sysml_conditional_0 = 0.0;
    }
    sysml_state_led_brightness_internal = sysml_conditional_0;
    double sysml_value_0 = 0;
    sysml_value_0 = sysml_state_led_brightness_internal;
    bool sysml_value_1 = 0;
    sysml_value_1 = true;
    double sysml_value_2 = 0;
    sysml_value_2 = io_1;
    double sysml_value_3 = 0;
    sysml_value_3 = io_2;
    double sysml_value_4 = 0;
    sysml_value_4 = io_3;
    double sysml_value_5 = 0;
    sysml_value_5 = sysml_state_led_brightness_internal;
    *io_0 = sysml_value_0;
    *serial_0_trigger = sysml_value_1;
    *serial_0_value_0 = sysml_value_2;
    *serial_0_value_1 = sysml_value_3;
    *serial_0_value_2 = sysml_value_4;
    *serial_0_value_3 = sysml_value_5;
}