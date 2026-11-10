#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_previousBtn1 = false;
/* Global Variable define of  */
bool sysml_state_previousBtn2 = false;
/* Global Variable define of  */
int sysml_state_mode = 0;
/* Global Variable define of  */
int sysml_state_maintenanceElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_flashAccumMs = 0;
/* Global Variable define of  */
bool sysml_state_ledFlashOn = false;
/* Global Variable define of  */
int sysml_state_btn1HoldMs = 0;
/* Global Variable define of  */
int sysml_state_btn2HoldMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_previousBtn1 = false;
    sysml_state_previousBtn2 = false;
    sysml_state_mode = 0;
    sysml_state_maintenanceElapsedMs = 0;
    sysml_state_flashAccumMs = 0;
    sysml_state_ledFlashOn = false;
    sysml_state_btn1HoldMs = 0;
    sysml_state_btn2HoldMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_3, bool io_4, int io_5, double* io_0, double* io_1, bool* io_2) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_lazy_1 = 0;
    bool sysml_lazy_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    bool sysml_lazy_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    double sysml_conditional_12 = 0;
    double sysml_conditional_13 = 0;
    double sysml_conditional_14 = 0;
    double sysml_conditional_15 = 0;
    bool sysml_conditional_16 = 0;
    bool sysml_conditional_17 = 0;
    sysml_lazy_0 = io_3;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_previousBtn1;
    }
    if (sysml_lazy_0) {
        if (io_4) {
            sysml_state_mode = 2;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    sysml_lazy_1 = io_4;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_previousBtn2;
    }
    if (sysml_lazy_1) {
        if (io_3) {
            sysml_state_mode = 2;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
        else {
            sysml_state_mode = 1;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = false;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    else {
        sysml_lazy_2 = !io_3;
        if (sysml_lazy_2) {
            sysml_lazy_2 = io_4;
        }
        sysml_lazy_3 = sysml_lazy_2;
        if (sysml_lazy_3) {
            sysml_lazy_3 = !sysml_state_previousBtn2;
        }
        if (sysml_lazy_3) {
            sysml_state_mode = 1;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = false;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
    }
    sysml_lazy_4 = io_3;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_previousBtn1;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !io_4;
    }
    if (sysml_lazy_5) {
        sysml_state_mode = 0;
        sysml_state_maintenanceElapsedMs = 0;
        sysml_state_flashAccumMs = 0;
        sysml_state_ledFlashOn = true;
        sysml_state_btn1HoldMs = 0;
        sysml_state_btn2HoldMs = 0;
    }
    sysml_lazy_6 = sysml_state_mode == 2;
    if (sysml_lazy_6) {
        sysml_lazy_7 = io_3;
        if (!sysml_lazy_7) {
            sysml_lazy_7 = io_4;
        }
        sysml_lazy_6 = sysml_lazy_7;
    }
    if (sysml_lazy_6) {
        sysml_lazy_8 = io_3;
        if (sysml_lazy_8) {
            sysml_lazy_8 = !io_4;
        }
        if (sysml_lazy_8) {
            sysml_state_mode = 0;
            sysml_state_maintenanceElapsedMs = 0;
            sysml_state_flashAccumMs = 0;
            sysml_state_ledFlashOn = true;
            sysml_state_btn1HoldMs = 0;
            sysml_state_btn2HoldMs = 0;
        }
        else {
            sysml_lazy_9 = io_4;
            if (sysml_lazy_9) {
                sysml_lazy_9 = !io_3;
            }
            if (sysml_lazy_9) {
                sysml_state_mode = 1;
                sysml_state_maintenanceElapsedMs = 0;
                sysml_state_flashAccumMs = 0;
                sysml_state_ledFlashOn = false;
                sysml_state_btn1HoldMs = 0;
                sysml_state_btn2HoldMs = 0;
            }
        }
    }
    if ((sysml_state_mode == 0)) {
        sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
        sysml_state_btn1HoldMs = (sysml_state_btn1HoldMs + io_5);
        sysml_state_btn2HoldMs = 0;
    }
    else {
        if ((sysml_state_mode == 1)) {
            sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
            sysml_state_btn2HoldMs = (sysml_state_btn2HoldMs + io_5);
            sysml_state_btn1HoldMs = 0;
        }
        else {
            if ((sysml_state_mode == 2)) {
                sysml_state_maintenanceElapsedMs = (sysml_state_maintenanceElapsedMs + io_5);
                sysml_state_flashAccumMs = (sysml_state_flashAccumMs + io_5);
                sysml_lazy_10 = sysml_state_ledFlashOn;
                if (sysml_lazy_10) {
                    sysml_lazy_10 = sysml_state_flashAccumMs >= 2000;
                }
                if (sysml_lazy_10) {
                    sysml_state_flashAccumMs = (sysml_state_flashAccumMs - 2000);
                    sysml_state_ledFlashOn = false;
                }
                sysml_lazy_11 = !sysml_state_ledFlashOn;
                if (sysml_lazy_11) {
                    sysml_lazy_11 = sysml_state_flashAccumMs >= 2000;
                }
                if (sysml_lazy_11) {
                    sysml_state_flashAccumMs = (sysml_state_flashAccumMs - 2000);
                    sysml_state_ledFlashOn = true;
                }
                sysml_state_btn1HoldMs = 0;
                sysml_state_btn2HoldMs = 0;
            }
        }
    }
    sysml_state_previousBtn1 = io_3;
    sysml_state_previousBtn2 = io_4;
    double sysml_value_0 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_12 = 90.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_13 = 0.0;
        }
        else {
            sysml_conditional_13 = 0.0;
        }
        sysml_conditional_12 = sysml_conditional_13;
    }
    sysml_value_0 = sysml_conditional_12;
    double sysml_value_1 = 0;
    if (sysml_state_mode == 1) {
        sysml_conditional_14 = 180.0;
    }
    else {
        if (sysml_state_mode == 2) {
            sysml_conditional_15 = 0.0;
        }
        else {
            sysml_conditional_15 = 0.0;
        }
        sysml_conditional_14 = sysml_conditional_15;
    }
    sysml_value_1 = sysml_conditional_14;
    bool sysml_value_2 = 0;
    if (sysml_state_mode == 0) {
        sysml_conditional_16 = true;
    }
    else {
        if (sysml_state_mode == 1) {
            sysml_conditional_17 = false;
        }
        else {
            sysml_conditional_17 = sysml_state_ledFlashOn;
        }
        sysml_conditional_16 = sysml_conditional_17;
    }
    sysml_value_2 = sysml_conditional_16;
    *io_0 = sysml_value_0;
    *io_1 = sysml_value_1;
    *io_2 = sysml_value_2;
}