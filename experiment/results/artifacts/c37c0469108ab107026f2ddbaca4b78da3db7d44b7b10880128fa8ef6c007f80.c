#include "define.h"
#include <stdio.h>
/* Main Function of model */
int main(int argc, char *argv[]) {
    ProgramPage1_Init(&ProgramPage1_instance);
    /* Main Loop of model */
    for (int i = 0; i < 1; i++) {
        double exec_io_6;
        double exec_io_7;
        double exec_io_0;
        double exec_io_1;
        double exec_io_2;
        double exec_io_3;
        double exec_io_4;
        double exec_io_5;
        bool exec_io_8_segment_0;
        bool exec_io_8_segment_1;
        bool exec_io_8_segment_2;
        bool exec_io_8_segment_3;
        bool exec_io_8_segment_4;
        bool exec_io_8_segment_5;
        bool exec_io_8_segment_6;
        bool exec_io_8_segment_7;
        ProgramPage1(exec_io_6, exec_io_7, &exec_io_0, &exec_io_1, &exec_io_2, &exec_io_3, &exec_io_4, &exec_io_5, &exec_io_8_segment_0, &exec_io_8_segment_1, &exec_io_8_segment_2, &exec_io_8_segment_3, &exec_io_8_segment_4, &exec_io_8_segment_5, &exec_io_8_segment_6, &exec_io_8_segment_7);
        printf("io_0: %lf\n", exec_io_0);
        printf("io_1: %lf\n", exec_io_1);
        printf("io_2: %lf\n", exec_io_2);
        printf("io_3: %lf\n", exec_io_3);
        printf("io_4: %lf\n", exec_io_4);
        printf("io_5: %lf\n", exec_io_5);
    }
}