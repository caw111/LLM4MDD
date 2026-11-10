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
        bool exec_io_3;
        int exec_io_4;
        ProgramPage1(&exec_io_0, &exec_io_1, &exec_io_2, &exec_io_3, &exec_io_4);
        printf("io_4: %d\n", exec_io_4);
    }
}