#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        bool exec_io_0;
        bool exec_io_1;
        bool exec_io_2;
        double exec_io_3;
        double exec_io_4;
        double exec_io_5;
        double exec_io_6;
        double exec_io_7;
        ProgramPage1(exec_io_0, exec_io_1, exec_io_2, &exec_io_3, &exec_io_4, &exec_io_5, &exec_io_6, &exec_io_7);
        printf("io_3: %lf\n", exec_io_3);
        printf("io_4: %lf\n", exec_io_4);
        printf("io_5: %lf\n", exec_io_5);
        printf("io_6: %lf\n", exec_io_6);
        printf("io_7: %lf\n", exec_io_7);
    }
}