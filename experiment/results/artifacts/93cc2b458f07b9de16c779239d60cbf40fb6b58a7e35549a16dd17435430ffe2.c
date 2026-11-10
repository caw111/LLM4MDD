#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        double exec_io_2;
        double exec_io_3;
        double exec_io_4;
        int exec_io_0;
        bool exec_io_1_segment_0;
        bool exec_io_1_segment_1;
        bool exec_io_1_segment_2;
        bool exec_io_1_segment_3;
        bool exec_io_1_segment_4;
        bool exec_io_1_segment_5;
        bool exec_io_1_segment_6;
        bool exec_io_1_segment_7;
        bool exec_io_1_segment_8;
        bool exec_io_1_segment_9;
        ProgramPage1(exec_io_2, exec_io_3, exec_io_4, &exec_io_0, &exec_io_1_segment_0, &exec_io_1_segment_1, &exec_io_1_segment_2, &exec_io_1_segment_3, &exec_io_1_segment_4, &exec_io_1_segment_5, &exec_io_1_segment_6, &exec_io_1_segment_7, &exec_io_1_segment_8, &exec_io_1_segment_9);
        printf("io_0: %d\n", exec_io_0);
    }
}