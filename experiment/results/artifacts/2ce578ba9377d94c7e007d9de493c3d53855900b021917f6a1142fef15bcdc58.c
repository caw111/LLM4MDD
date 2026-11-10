#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        double exec_io_0;
        double exec_io_1;
        bool exec_io_2;
        double exec_io_3;
        double exec_io_4;
        bool exec_io_5;
        double exec_io_6;
        double exec_io_7;
        bool exec_io_8;
        bool exec_io_9;
        int exec_io_13;
        bool exec_io_10;
        bool exec_io_11;
        int exec_io_12;
        bool exec_serial_0_trigger;
        double exec_serial_0_value_0;
        double exec_serial_0_value_1;
        double exec_serial_0_value_2;
        double exec_serial_0_value_3;
        double exec_serial_0_value_4;
        double exec_serial_0_value_5;
        bool exec_serial_1_trigger;
        bool exec_serial_1_value_0;
        bool exec_serial_1_value_1;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_3, exec_io_4, exec_io_5, exec_io_6, exec_io_7, exec_io_8, exec_io_9, exec_io_13, &exec_io_10, &exec_io_11, &exec_io_12, &exec_serial_0_trigger, &exec_serial_0_value_0, &exec_serial_0_value_1, &exec_serial_0_value_2, &exec_serial_0_value_3, &exec_serial_0_value_4, &exec_serial_0_value_5, &exec_serial_1_trigger, &exec_serial_1_value_0, &exec_serial_1_value_1);
        printf("io_12: %d\n", exec_io_12);
        printf("serial_0_value_0: %lf\n", exec_serial_0_value_0);
        printf("serial_0_value_1: %lf\n", exec_serial_0_value_1);
        printf("serial_0_value_2: %lf\n", exec_serial_0_value_2);
        printf("serial_0_value_3: %lf\n", exec_serial_0_value_3);
        printf("serial_0_value_4: %lf\n", exec_serial_0_value_4);
        printf("serial_0_value_5: %lf\n", exec_serial_0_value_5);
    }
}