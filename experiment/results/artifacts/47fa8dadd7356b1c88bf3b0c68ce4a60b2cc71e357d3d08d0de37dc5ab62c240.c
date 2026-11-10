#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        bool exec_io_0;
        bool exec_serial_0_trigger;
        double exec_serial_0_value_0;
        ProgramPage1(exec_io_0, &exec_serial_0_trigger, &exec_serial_0_value_0);
        printf("serial_0_value_0: %lf\n", exec_serial_0_value_0);
    }
}