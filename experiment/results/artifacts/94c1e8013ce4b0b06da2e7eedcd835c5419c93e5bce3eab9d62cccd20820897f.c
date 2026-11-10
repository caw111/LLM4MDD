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
        int exec_io_6;
        bool exec_io_6_valid;
        int exec_io_3;
        bool exec_io_4;
        int exec_io_5;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, exec_io_6, exec_io_6_valid, &exec_io_3, &exec_io_4, &exec_io_5);
        printf("io_3: %d\n", exec_io_3);
        printf("io_5: %d\n", exec_io_5);
    }
}