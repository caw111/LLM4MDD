#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        double exec_io_0;
        bool exec_io_1;
        double exec_io_2;
        bool exec_io_3;
        double exec_io_4;
        double exec_io_5;
        bool exec_io_6;
        bool exec_io_9;
        double exec_io_7;
        int exec_io_8;
        bool exec_serial_0_trigger;
        int exec_serial_0_value_0;
        double exec_serial_0_value_1;
        bool exec_serial_0_value_2;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_3, exec_io_4, exec_io_5, exec_io_6, exec_io_9, &exec_io_7, &exec_io_8, &exec_serial_0_trigger, &exec_serial_0_value_0, &exec_serial_0_value_1, &exec_serial_0_value_2);
        printf("io_7: %lf\n", exec_io_7);
        printf("io_8: %d\n", exec_io_8);
        printf("serial_0_value_0: %d\n", exec_serial_0_value_0);
        printf("serial_0_value_1: %lf\n", exec_serial_0_value_1);
    }
}