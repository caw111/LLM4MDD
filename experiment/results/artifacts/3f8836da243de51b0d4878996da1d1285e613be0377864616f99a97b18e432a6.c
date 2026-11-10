#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int sysml_state_stageIndex = 0;
/* Global Variable define of  */
bool sysml_state_paused = true;
/* Global Variable define of  */
bool sysml_state_completed = false;
/* Global Variable define of  */
bool sysml_state_prevIo0 = false;
/* Global Variable define of  */
bool sysml_state_prevIo1 = false;
/* Global Variable define of  */
bool sysml_state_prevIo2 = false;
/* Global Variable define of  */
int sysml_state_stage1RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_stage2RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_stage3RemainingMs = 0;
/* Global Variable define of  */
int sysml_state_stage1ElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_stage2ElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_stage3ElapsedMs = 0;
/* Global Variable define of  */
int sysml_state_totalElapsedMs = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stageIndex = 0;
    sysml_state_paused = true;
    sysml_state_completed = false;
    sysml_state_prevIo0 = false;
    sysml_state_prevIo1 = false;
    sysml_state_prevIo2 = false;
    sysml_state_stage1RemainingMs = 0;
    sysml_state_stage2RemainingMs = 0;
    sysml_state_stage3RemainingMs = 0;
    sysml_state_stage1ElapsedMs = 0;
    sysml_state_stage2ElapsedMs = 0;
    sysml_state_stage3ElapsedMs = 0;
    sysml_state_totalElapsedMs = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool* io_3, int* io_4, int* io_5, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1, int* serial_0_value_2, int* serial_0_value_3) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    bool sysml_lazy_0 = 0;
    bool sysml_conditional_1 = 0;
    bool sysml_lazy_2 = 0;
    int sysml_conditional_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    int sysml_conditional_6 = 0;
    int sysml_conditional_7 = 0;
    int sysml_conditional_8 = 0;
    int sysml_conditional_9 = 0;
    int sysml_conditional_10 = 0;
    sysml_lazy_0 = io_2;
    if (sysml_lazy_0) {
        sysml_lazy_0 = !sysml_state_prevIo2;
    }
    if (sysml_lazy_0) {
        if (sysml_state_completed) {
            sysml_conditional_1 = false;
        }
        else {
            sysml_conditional_1 = !sysml_state_paused;
        }
        sysml_state_paused = sysml_conditional_1;
        sysml_state_completed = false;
    }
    sysml_lazy_2 = io_1;
    if (sysml_lazy_2) {
        sysml_lazy_2 = !sysml_state_prevIo1;
    }
    if (sysml_lazy_2) {
        if (sysml_state_stageIndex == 2) {
            sysml_conditional_3 = 0;
        }
        else {
            sysml_conditional_3 = sysml_state_stageIndex + 1;
        }
        sysml_state_stageIndex = sysml_conditional_3;
    }
    sysml_lazy_4 = io_0;
    if (sysml_lazy_4) {
        sysml_lazy_4 = !sysml_state_prevIo0;
    }
    if (sysml_lazy_4) {
        if ((sysml_state_stageIndex == 0)) {
            sysml_state_stage1RemainingMs = (sysml_state_stage1RemainingMs + 1000);
        }
        else {
            if ((sysml_state_stageIndex == 1)) {
                sysml_state_stage2RemainingMs = (sysml_state_stage2RemainingMs + 1000);
            }
            else {
                sysml_state_stage3RemainingMs = (sysml_state_stage3RemainingMs + 1000);
            }
        }
    }
    sysml_lazy_5 = !sysml_state_completed;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_paused;
    }
    if (sysml_lazy_5) {
        if ((sysml_state_stageIndex == 0)) {
            if ((sysml_state_stage1RemainingMs <= 0)) {
                if (sysml_state_stageIndex == 2) {
                    sysml_conditional_6 = 2;
                }
                else {
                    sysml_conditional_6 = 1;
                }
                sysml_state_stageIndex = sysml_conditional_6;
            }
        }
        else {
            if ((sysml_state_stageIndex == 1)) {
                if ((sysml_state_stage2RemainingMs <= 0)) {
                    sysml_state_stageIndex = 2;
                }
            }
            else {
                if ((sysml_state_stageIndex == 2)) {
                    if ((sysml_state_stage3RemainingMs <= 0)) {
                        sysml_state_completed = true;
                    }
                }
            }
        }
    }
    sysml_state_prevIo0 = io_0;
    sysml_state_prevIo1 = io_1;
    sysml_state_prevIo2 = io_2;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_completed;
    int sysml_value_1 = 0;
    if (sysml_state_completed) {
        sysml_conditional_7 = 0;
    }
    else {
        sysml_conditional_7 = 0;
    }
    sysml_value_1 = sysml_conditional_7;
    int sysml_value_2 = 0;
    if (sysml_state_completed) {
        sysml_conditional_8 = 0;
    }
    else {
        if (sysml_state_stageIndex == 0) {
            sysml_conditional_9 = sysml_state_stage1RemainingMs;
        }
        else {
            if (sysml_state_stageIndex == 1) {
                sysml_conditional_10 = sysml_state_stage2RemainingMs;
            }
            else {
                sysml_conditional_10 = sysml_state_stage3RemainingMs;
            }
            sysml_conditional_9 = sysml_conditional_10;
        }
        sysml_conditional_8 = sysml_conditional_9;
    }
    sysml_value_2 = sysml_conditional_8;
    bool sysml_value_3 = 0;
    sysml_value_3 = sysml_state_completed;
    int sysml_value_4 = 0;
    sysml_value_4 = sysml_state_totalElapsedMs;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_stage1ElapsedMs;
    int sysml_value_6 = 0;
    sysml_value_6 = sysml_state_stage2ElapsedMs;
    int sysml_value_7 = 0;
    sysml_value_7 = sysml_state_stage3ElapsedMs;
    *io_3 = sysml_value_0;
    *io_4 = sysml_value_1;
    *io_5 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
    *serial_0_value_2 = sysml_value_6;
    *serial_0_value_3 = sysml_value_7;
}