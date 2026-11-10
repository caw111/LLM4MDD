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
        double exec_io_3;
        double exec_io_4;
        bool exec_io_8;
        bool exec_io_9;
        int exec_io_12;
        double exec_io_5;
        bool exec_io_6;
        bool exec_io_7;
        int exec_io_10;
        int exec_io_11;
        bool exec_serial_0_trigger;
        bool exec_serial_0_value_0;
        bool exec_serial_0_value_1;
        bool exec_serial_0_value_2;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_3, exec_io_4, exec_io_8, exec_io_9, exec_io_12, &exec_io_5, &exec_io_6, &exec_io_7, &exec_io_10, &exec_io_11, &exec_serial_0_trigger, &exec_serial_0_value_0, &exec_serial_0_value_1, &exec_serial_0_value_2);
        printf("io_5: %lf\n", exec_io_5);
        printf("io_10: %d\n", exec_io_10);
        printf("io_11: %d\n", exec_io_11);
    }
}