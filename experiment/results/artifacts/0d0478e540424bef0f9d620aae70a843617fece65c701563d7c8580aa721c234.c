#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        int exec_io_5;
        bool exec_io_0;
        bool exec_io_1;
        double exec_io_2;
        double exec_io_3;
        double exec_io_4;
        ProgramPage1(exec_io_5, &exec_io_0, &exec_io_1, &exec_io_2, &exec_io_3, &exec_io_4);
        printf("io_2: %lf\n", exec_io_2);
        printf("io_3: %lf\n", exec_io_3);
        printf("io_4: %lf\n", exec_io_4);
    }
}