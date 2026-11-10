#include "define.h"
/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
bool sysml_state_locked = false;
/* Global Variable define of  */
bool sysml_state_previousPressedAny = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP1 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP2 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP3 = false;
/* Global Variable define of  */
bool sysml_state_previousPressedP4 = false;
/* Global Variable define of  */
bool sysml_state_resetHeld = false;
/* Global Variable define of  */
bool sysml_state_resetLatch = false;
/* Global Variable define of  */
int sysml_state_playerId = 0;
/* Global Variable define of  */
int sysml_state_buzzerTone = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_locked = false;
    sysml_state_previousPressedAny = false;
    sysml_state_previousPressedP1 = false;
    sysml_state_previousPressedP2 = false;
    sysml_state_previousPressedP3 = false;
    sysml_state_previousPressedP4 = false;
    sysml_state_resetHeld = false;
    sysml_state_resetLatch = false;
    sysml_state_playerId = 0;
    sysml_state_buzzerTone = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, bool io_4, bool* io_5, int* io_6, int* io_7, bool* serial_0_trigger, int* serial_0_value_0, int* serial_0_value_1) {
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
    bool sysml_lazy_12 = 0;
    bool sysml_lazy_13 = 0;
    bool sysml_lazy_14 = 0;
    bool sysml_lazy_15 = 0;
    bool sysml_lazy_16 = 0;
    bool sysml_lazy_17 = 0;
    bool sysml_lazy_18 = 0;
    bool sysml_lazy_19 = 0;
    bool sysml_lazy_20 = 0;
    if (io_4) {
        sysml_lazy_0 = io_4;
        if (sysml_lazy_0) {
            sysml_lazy_0 = !sysml_state_resetLatch;
        }
        if (sysml_lazy_0) {
            sysml_state_resetHeld = true;
        }
        sysml_state_resetLatch = true;
    }
    else {
        sysml_state_resetHeld = false;
        sysml_state_resetLatch = false;
    }
    sysml_lazy_1 = sysml_state_resetLatch;
    if (sysml_lazy_1) {
        sysml_lazy_1 = !sysml_state_locked;
    }
    if (sysml_lazy_1) {
        sysml_state_buzzerTone = 0;
    }
    if (io_4) {
        sysml_state_locked = false;
        sysml_state_playerId = 0;
        sysml_state_buzzerTone = 0;
        sysml_state_previousPressedAny = false;
        sysml_state_previousPressedP1 = false;
        sysml_state_previousPressedP2 = false;
        sysml_state_previousPressedP3 = false;
        sysml_state_previousPressedP4 = false;
    }
    sysml_lazy_2 = !sysml_state_locked;
    if (sysml_lazy_2) {
        sysml_lazy_2 = io_0;
    }
    sysml_lazy_3 = sysml_lazy_2;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousPressedP1;
    }
    if (sysml_lazy_3) {
        sysml_state_locked = true;
        sysml_state_playerId = 1;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_4 = !sysml_state_locked;
    if (sysml_lazy_4) {
        sysml_lazy_4 = io_1;
    }
    sysml_lazy_5 = sysml_lazy_4;
    if (sysml_lazy_5) {
        sysml_lazy_5 = !sysml_state_previousPressedP2;
    }
    if (sysml_lazy_5) {
        sysml_state_locked = true;
        sysml_state_playerId = 2;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_6 = !sysml_state_locked;
    if (sysml_lazy_6) {
        sysml_lazy_6 = io_2;
    }
    sysml_lazy_7 = sysml_lazy_6;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_previousPressedP3;
    }
    if (sysml_lazy_7) {
        sysml_state_locked = true;
        sysml_state_playerId = 3;
        sysml_state_buzzerTone = 1000;
    }
    sysml_lazy_8 = !sysml_state_locked;
    if (sysml_lazy_8) {
        sysml_lazy_8 = io_3;
    }
    sysml_lazy_9 = sysml_lazy_8;
    if (sysml_lazy_9) {
        sysml_lazy_9 = !sysml_state_previousPressedP4;
    }
    if (sysml_lazy_9) {
        sysml_state_locked = true;
        sysml_state_playerId = 4;
        sysml_state_buzzerTone = 1000;
    }
    if (sysml_state_locked) {
        sysml_state_buzzerTone = 0;
    }
    sysml_lazy_10 = io_0;
    if (!sysml_lazy_10) {
        sysml_lazy_10 = io_1;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (!sysml_lazy_11) {
        sysml_lazy_11 = io_2;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (!sysml_lazy_12) {
        sysml_lazy_12 = io_3;
    }
    sysml_state_previousPressedAny = sysml_lazy_12;
    sysml_state_previousPressedP1 = io_0;
    sysml_state_previousPressedP2 = io_1;
    sysml_state_previousPressedP3 = io_2;
    sysml_state_previousPressedP4 = io_3;
    bool sysml_value_0 = 0;
    sysml_value_0 = sysml_state_locked;
    int sysml_value_1 = 0;
    sysml_value_1 = sysml_state_buzzerTone;
    int sysml_value_2 = 0;
    sysml_value_2 = sysml_state_playerId;
    bool sysml_value_3 = 0;
    sysml_lazy_13 = !sysml_state_locked;
    if (sysml_lazy_13) {
        sysml_lazy_14 = io_0;
        if (sysml_lazy_14) {
            sysml_lazy_14 = !sysml_state_previousPressedP1;
        }
        sysml_lazy_15 = sysml_lazy_14;
        if (!sysml_lazy_15) {
            sysml_lazy_16 = io_1;
            if (sysml_lazy_16) {
                sysml_lazy_16 = !sysml_state_previousPressedP2;
            }
            sysml_lazy_15 = sysml_lazy_16;
        }
        sysml_lazy_17 = sysml_lazy_15;
        if (!sysml_lazy_17) {
            sysml_lazy_18 = io_2;
            if (sysml_lazy_18) {
                sysml_lazy_18 = !sysml_state_previousPressedP3;
            }
            sysml_lazy_17 = sysml_lazy_18;
        }
        sysml_lazy_19 = sysml_lazy_17;
        if (!sysml_lazy_19) {
            sysml_lazy_20 = io_3;
            if (sysml_lazy_20) {
                sysml_lazy_20 = !sysml_state_previousPressedP4;
            }
            sysml_lazy_19 = sysml_lazy_20;
        }
        sysml_lazy_13 = sysml_lazy_19;
    }
    sysml_value_3 = sysml_lazy_13;
    int sysml_value_4 = 0;
    sysml_value_4 = 0;
    int sysml_value_5 = 0;
    sysml_value_5 = sysml_state_playerId;
    *io_5 = sysml_value_0;
    *io_6 = sysml_value_1;
    *io_7 = sysml_value_2;
    *serial_0_trigger = sysml_value_3;
    *serial_0_value_0 = sysml_value_4;
    *serial_0_value_1 = sysml_value_5;
}