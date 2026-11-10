#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        double exec_io_1;
        double exec_io_0;
        int exec_io_2;
        int exec_io_3;
        ProgramPage1(exec_io_1, &exec_io_0, &exec_io_2, &exec_io_3);
        printf("io_0: %lf\n", exec_io_0);
        printf("io_2: %d\n", exec_io_2);
        printf("io_3: %d\n", exec_io_3);
    }
}