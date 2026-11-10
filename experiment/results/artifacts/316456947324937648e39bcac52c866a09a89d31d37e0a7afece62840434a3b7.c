#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        bool exec_io_0;
        double exec_io_1;
        double exec_io_2;
        bool exec_io_3;
        int exec_io_5;
        bool exec_io_5_valid;
        int exec_io_4;
        bool exec_serial_0_trigger;
        double exec_serial_0_value_0;
        double exec_serial_0_value_1;
        bool exec_serial_0_value_2;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_3, exec_io_5, exec_io_5_valid, &exec_io_4, &exec_serial_0_trigger, &exec_serial_0_value_0, &exec_serial_0_value_1, &exec_serial_0_value_2);
        printf("io_4: %d\n", exec_io_4);
        printf("serial_0_value_0: %lf\n", exec_serial_0_value_0);
        printf("serial_0_value_1: %lf\n", exec_serial_0_value_1);
    }
}