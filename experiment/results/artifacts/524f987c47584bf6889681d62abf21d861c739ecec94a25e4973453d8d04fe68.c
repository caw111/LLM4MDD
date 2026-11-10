#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        bool exec_io_0;
        bool exec_io_4;
        double exec_io_5;
        bool exec_io_1;
        int exec_io_2;
        int exec_io_3;
        bool exec_serial_0_trigger;
        int exec_serial_0_value_0;
        ProgramPage1(exec_io_0, exec_io_4, exec_io_5, &exec_io_1, &exec_io_2, &exec_io_3, &exec_serial_0_trigger, &exec_serial_0_value_0);
        printf("io_2: %d\n", exec_io_2);
        printf("io_3: %d\n", exec_io_3);
        printf("serial_0_value_0: %d\n", exec_serial_0_value_0);
    }
}