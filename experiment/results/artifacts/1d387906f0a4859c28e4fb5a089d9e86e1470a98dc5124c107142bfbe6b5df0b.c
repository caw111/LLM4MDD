#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        bool exec_io_0;
        bool exec_io_1;
        double exec_io_2;
        double exec_io_3;
        double exec_io_4;
        double exec_io_9;
        double exec_io_10;
        bool exec_io_11;
        double exec_io_5;
        bool exec_io_6;
        bool exec_io_7;
        int exec_io_8;
        int exec_io_12;
        bool exec_serial_0_trigger;
        bool exec_serial_0_value_0;
        double exec_serial_0_value_1;
        double exec_serial_0_value_2;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_3, exec_io_4, exec_io_9, exec_io_10, exec_io_11, &exec_io_5, &exec_io_6, &exec_io_7, &exec_io_8, &exec_io_12, &exec_serial_0_trigger, &exec_serial_0_value_0, &exec_serial_0_value_1, &exec_serial_0_value_2);
        printf("io_5: %lf\n", exec_io_5);
        printf("io_8: %d\n", exec_io_8);
        printf("io_12: %d\n", exec_io_12);
        printf("serial_0_value_1: %lf\n", exec_serial_0_value_1);
        printf("serial_0_value_2: %lf\n", exec_serial_0_value_2);
    }
}