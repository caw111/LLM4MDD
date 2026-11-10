#ifdef ARDUINO
#include <Arduino.h>
#endif
// BEGIN COMPILED SYSML MODEL
#include <stdint.h>
#include <stdbool.h>
#ifndef _DEFINE_H_
#define _DEFINE_H_
struct ProgramPage1_state;
struct ProgramPage1_state {
};
/* Global Variable define of  */
extern struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
extern int32_t sysml_state_stockOne;
/* Global Variable define of  */
extern int32_t sysml_state_stockTwo;
/* Global Variable define of  */
extern int32_t sysml_state_stockThree;
/* Global Variable define of  */
extern int32_t sysml_state_priceOne;
/* Global Variable define of  */
extern int32_t sysml_state_priceTwo;
/* Global Variable define of  */
extern int32_t sysml_state_priceThree;
/* Global Variable define of  */
extern int32_t sysml_state_selectedItem;
/* Global Variable define of  */
extern int32_t sysml_state_displayPhase;
/* Global Variable define of  */
extern bool sysml_state_displayError;
/* Global Variable define of  */
extern bool sysml_state_adminMode;
/* Global Variable define of  */
extern bool sysml_state_previousOne;
/* Global Variable define of  */
extern bool sysml_state_previousTwo;
/* Global Variable define of  */
extern bool sysml_state_previousThree;
/* Global Variable define of  */
extern bool sysml_state_previousConfirm;
/* Global Variable define of  */
extern int32_t sysml_state_holdDuration;
/* Global Variable define of  */
extern bool sysml_state_holdHandled;
/* Global Variable define of  */
extern int32_t sysml_state_beepRemaining;
/* Global Variable define of  */
extern int32_t sysml_state_beepFrequency;
/* Global Variable define of  */
extern int32_t sysml_state_saleItem;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self);
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int32_t io_7, bool io_7_valid, int32_t io_11, bool* io_4, int32_t* io_5, int32_t* io_6, bool* io_8, bool* io_9, bool* io_10);
#endif


/* Code of ProgramPage1 */
/* Global Variable define of  */
struct ProgramPage1_state ProgramPage1_instance;
/* Global Variable define of  */
int32_t sysml_state_stockOne = 2;
/* Global Variable define of  */
int32_t sysml_state_stockTwo = 2;
/* Global Variable define of  */
int32_t sysml_state_stockThree = 2;
/* Global Variable define of  */
int32_t sysml_state_priceOne = 1;
/* Global Variable define of  */
int32_t sysml_state_priceTwo = 2;
/* Global Variable define of  */
int32_t sysml_state_priceThree = 3;
/* Global Variable define of  */
int32_t sysml_state_selectedItem = 0;
/* Global Variable define of  */
int32_t sysml_state_displayPhase = 0;
/* Global Variable define of  */
bool sysml_state_displayError = false;
/* Global Variable define of  */
bool sysml_state_adminMode = false;
/* Global Variable define of  */
bool sysml_state_previousOne = false;
/* Global Variable define of  */
bool sysml_state_previousTwo = false;
/* Global Variable define of  */
bool sysml_state_previousThree = false;
/* Global Variable define of  */
bool sysml_state_previousConfirm = false;
/* Global Variable define of  */
int32_t sysml_state_holdDuration = 0;
/* Global Variable define of  */
bool sysml_state_holdHandled = false;
/* Global Variable define of  */
int32_t sysml_state_beepRemaining = 0;
/* Global Variable define of  */
int32_t sysml_state_beepFrequency = 0;
/* Global Variable define of  */
int32_t sysml_state_saleItem = 0;
/* Init function of CompositeActor named "ProgramPage1" */
void ProgramPage1_Init(struct ProgramPage1_state* self) {
    /* Calculate code of CompositeActor named "ProgramPage1.reset" */
    sysml_state_stockOne = 2;
    sysml_state_stockTwo = 2;
    sysml_state_stockThree = 2;
    sysml_state_priceOne = 1;
    sysml_state_priceTwo = 2;
    sysml_state_priceThree = 3;
    sysml_state_selectedItem = 0;
    sysml_state_displayPhase = 0;
    sysml_state_displayError = false;
    sysml_state_adminMode = false;
    sysml_state_previousOne = false;
    sysml_state_previousTwo = false;
    sysml_state_previousThree = false;
    sysml_state_previousConfirm = false;
    sysml_state_holdDuration = 0;
    sysml_state_holdHandled = false;
    sysml_state_beepRemaining = 0;
    sysml_state_beepFrequency = 0;
    sysml_state_saleItem = 0;
}
/* Update function of CompositeActor named "ProgramPage1" */
void ProgramPage1(bool io_0, bool io_1, bool io_2, bool io_3, int32_t io_7, bool io_7_valid, int32_t io_11, bool* io_4, int32_t* io_5, int32_t* io_6, bool* io_8, bool* io_9, bool* io_10) {
    /* Calculate code of CompositeActor named "ProgramPage1.body" */
    int32_t sysml_conditional_0 = 0;
    int32_t sysml_conditional_1 = 0;
    int32_t sysml_arithmetic_2 = 0;
    bool sysml_lazy_3 = 0;
    bool sysml_lazy_4 = 0;
    bool sysml_lazy_5 = 0;
    bool sysml_lazy_6 = 0;
    bool sysml_lazy_7 = 0;
    int32_t sysml_conditional_8 = 0;
    bool sysml_lazy_9 = 0;
    bool sysml_lazy_10 = 0;
    bool sysml_lazy_11 = 0;
    bool sysml_lazy_12 = 0;
    int32_t sysml_conditional_13 = 0;
    bool sysml_lazy_14 = 0;
    int32_t sysml_conditional_15 = 0;
    int32_t sysml_conditional_16 = 0;
    int32_t sysml_conditional_17 = 0;
    int32_t sysml_conditional_18 = 0;
    int32_t sysml_conditional_19 = 0;
    int32_t sysml_conditional_20 = 0;
    int32_t sysml_conditional_21 = 0;
    int32_t sysml_conditional_22 = 0;
    int32_t sysml_conditional_23 = 0;
    int32_t sysml_conditional_24 = 0;
    int32_t sysml_conditional_25 = 0;
    int32_t sysml_conditional_26 = 0;
    int32_t sysml_conditional_27 = 0;
    bool sysml_lazy_28 = 0;
    bool sysml_lazy_29 = 0;
    bool sysml_lazy_30 = 0;
    bool sysml_lazy_31 = 0;
    int32_t sysml_conditional_32 = 0;
    int32_t sysml_conditional_33 = 0;
    int32_t sysml_conditional_34 = 0;
    int32_t sysml_arithmetic_35 = 0;
    int32_t sysml_conditional_36 = 0;
    int32_t sysml_conditional_37 = 0;
    int32_t sysml_conditional_38 = 0;
    int32_t sysml_conditional_39 = 0;
    int32_t sysml_conditional_40 = 0;
    int32_t sysml_conditional_41 = 0;
    int32_t sysml_conditional_42 = 0;
    int32_t sysml_conditional_43 = 0;
    int32_t sysml_conditional_44 = 0;
    sysml_state_saleItem = 0;
    if ((sysml_state_beepRemaining > 0)) {
        if (io_11 >= sysml_state_beepRemaining) {
            sysml_conditional_0 = 0;
        }
        else {
            sysml_conditional_0 = sysml_state_beepRemaining - io_11;
        }
        sysml_state_beepRemaining = sysml_conditional_0;
    }
    if ((sysml_state_selectedItem > 0)) {
        if ((((double)io_11) / 2000) < ((double)((int32_t)(((double)io_11) / 2000)))) {
            sysml_conditional_1 = 1;
        }
        else {
            sysml_conditional_1 = 0;
        }
        sysml_arithmetic_2 = ((int32_t)(((double)io_11) / 2000)) - sysml_conditional_1;
        sysml_state_displayPhase = sysml_state_displayPhase + (io_11 - (2000 * sysml_arithmetic_2));
        if ((sysml_state_displayPhase >= 2000)) {
            sysml_state_displayPhase = (sysml_state_displayPhase - 2000);
        }
    }
    sysml_lazy_3 = io_0;
    if (sysml_lazy_3) {
        sysml_lazy_3 = !sysml_state_previousOne;
    }
    if (sysml_lazy_3) {
        sysml_state_selectedItem = 1;
        sysml_state_displayPhase = 0;
        sysml_state_displayError = false;
    }
    else {
        sysml_lazy_4 = io_1;
        if (sysml_lazy_4) {
            sysml_lazy_4 = !sysml_state_previousTwo;
        }
        if (sysml_lazy_4) {
            sysml_state_selectedItem = 2;
            sysml_state_displayPhase = 0;
            sysml_state_displayError = false;
        }
        else {
            sysml_lazy_5 = io_2;
            if (sysml_lazy_5) {
                sysml_lazy_5 = !sysml_state_previousThree;
            }
            if (sysml_lazy_5) {
                sysml_state_selectedItem = 3;
                sysml_state_displayPhase = 0;
                sysml_state_displayError = false;
            }
        }
    }
    sysml_lazy_6 = io_3;
    if (sysml_lazy_6) {
        sysml_lazy_6 = !sysml_state_previousConfirm;
    }
    if (sysml_lazy_6) {
        sysml_state_holdDuration = 0;
        sysml_state_holdHandled = false;
    }
    sysml_lazy_7 = sysml_state_previousConfirm;
    if (sysml_lazy_7) {
        sysml_lazy_7 = !sysml_state_holdHandled;
    }
    if (sysml_lazy_7) {
        if (io_11 >= (2000 - sysml_state_holdDuration)) {
            sysml_conditional_8 = 2000;
        }
        else {
            sysml_conditional_8 = sysml_state_holdDuration + io_11;
        }
        sysml_state_holdDuration = sysml_conditional_8;
        if ((sysml_state_holdDuration >= 2000)) {
            sysml_state_adminMode = (!sysml_state_adminMode);
            sysml_state_holdHandled = true;
        }
    }
    sysml_lazy_9 = io_7_valid;
    if (sysml_lazy_9) {
        sysml_lazy_9 = sysml_state_adminMode;
    }
    sysml_lazy_10 = sysml_lazy_9;
    if (sysml_lazy_10) {
        sysml_lazy_10 = io_7 >= 10000;
    }
    sysml_lazy_11 = sysml_lazy_10;
    if (sysml_lazy_11) {
        sysml_lazy_11 = io_7 <= 39999;
    }
    sysml_lazy_12 = sysml_lazy_11;
    if (sysml_lazy_12) {
        if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
            sysml_conditional_13 = 1;
        }
        else {
            sysml_conditional_13 = 0;
        }
        sysml_lazy_12 = (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_13) * 10000)) >= 0;
    }
    sysml_lazy_14 = sysml_lazy_12;
    if (sysml_lazy_14) {
        if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
            sysml_conditional_15 = 1;
        }
        else {
            sysml_conditional_15 = 0;
        }
        sysml_lazy_14 = (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_15) * 10000)) <= 9999;
    }
    if (sysml_lazy_14) {
        if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
            sysml_conditional_16 = 1;
        }
        else {
            sysml_conditional_16 = 0;
        }
        if ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_16) == 1) {
            if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                sysml_conditional_17 = 1;
            }
            else {
                sysml_conditional_17 = 0;
            }
            if ((sysml_state_stockOne + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_17) * 10000))) > 9999) {
                sysml_conditional_18 = 9999;
            }
            else {
                if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                    sysml_conditional_19 = 1;
                }
                else {
                    sysml_conditional_19 = 0;
                }
                sysml_conditional_18 = sysml_state_stockOne + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_19) * 10000));
            }
            sysml_state_stockOne = sysml_conditional_18;
        }
        else {
            if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                sysml_conditional_20 = 1;
            }
            else {
                sysml_conditional_20 = 0;
            }
            if ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_20) == 2) {
                if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                    sysml_conditional_21 = 1;
                }
                else {
                    sysml_conditional_21 = 0;
                }
                if ((sysml_state_stockTwo + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_21) * 10000))) > 9999) {
                    sysml_conditional_22 = 9999;
                }
                else {
                    if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                        sysml_conditional_23 = 1;
                    }
                    else {
                        sysml_conditional_23 = 0;
                    }
                    sysml_conditional_22 = sysml_state_stockTwo + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_23) * 10000));
                }
                sysml_state_stockTwo = sysml_conditional_22;
            }
            else {
                if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                    sysml_conditional_24 = 1;
                }
                else {
                    sysml_conditional_24 = 0;
                }
                if ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_24) == 3) {
                    if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                        sysml_conditional_25 = 1;
                    }
                    else {
                        sysml_conditional_25 = 0;
                    }
                    if ((sysml_state_stockThree + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_25) * 10000))) > 9999) {
                        sysml_conditional_26 = 9999;
                    }
                    else {
                        if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
                            sysml_conditional_27 = 1;
                        }
                        else {
                            sysml_conditional_27 = 0;
                        }
                        sysml_conditional_26 = sysml_state_stockThree + (io_7 - ((((int32_t)(((double)io_7) / 10000)) - sysml_conditional_27) * 10000));
                    }
                    sysml_state_stockThree = sysml_conditional_26;
                }
            }
        }
    }
    sysml_lazy_28 = sysml_state_previousConfirm;
    if (sysml_lazy_28) {
        sysml_lazy_28 = !io_3;
    }
    sysml_lazy_29 = sysml_lazy_28;
    if (sysml_lazy_29) {
        sysml_lazy_29 = !sysml_state_holdHandled;
    }
    sysml_lazy_30 = sysml_lazy_29;
    if (sysml_lazy_30) {
        sysml_lazy_30 = !sysml_state_adminMode;
    }
    sysml_lazy_31 = sysml_lazy_30;
    if (sysml_lazy_31) {
        sysml_lazy_31 = sysml_state_selectedItem > 0;
    }
    if (sysml_lazy_31) {
        if (sysml_state_selectedItem == 1) {
            sysml_conditional_32 = sysml_state_stockOne;
        }
        else {
            if (sysml_state_selectedItem == 2) {
                sysml_conditional_33 = sysml_state_stockTwo;
            }
            else {
                sysml_conditional_33 = sysml_state_stockThree;
            }
            sysml_conditional_32 = sysml_conditional_33;
        }
        if (sysml_conditional_32 > 0) {
            sysml_state_saleItem = sysml_state_selectedItem;
            if ((sysml_state_selectedItem == 1)) {
                sysml_state_stockOne = (sysml_state_stockOne - 1);
            }
            else {
                if ((sysml_state_selectedItem == 2)) {
                    sysml_state_stockTwo = (sysml_state_stockTwo - 1);
                }
                else {
                    sysml_state_stockThree = (sysml_state_stockThree - 1);
                }
            }
            sysml_state_displayError = false;
            sysml_state_beepFrequency = 2000;
            sysml_state_beepRemaining = 100;
        }
        else {
            sysml_state_displayError = true;
            sysml_state_beepFrequency = 1000;
            sysml_state_beepRemaining = 500;
        }
    }
    sysml_state_previousOne = io_0;
    sysml_state_previousTwo = io_1;
    sysml_state_previousThree = io_2;
    sysml_state_previousConfirm = io_3;
    int32_t sysml_value_0 = 0;
    if ((((double)io_11) / 2000) < ((double)((int32_t)(((double)io_11) / 2000)))) {
        sysml_conditional_34 = 1;
    }
    else {
        sysml_conditional_34 = 0;
    }
    sysml_arithmetic_35 = ((int32_t)(((double)io_11) / 2000)) - sysml_conditional_34;
    sysml_value_0 = io_11 - (2000 * sysml_arithmetic_35);
    int32_t sysml_value_1 = 0;
    if ((((double)io_7) / 10000) < ((double)((int32_t)(((double)io_7) / 10000)))) {
        sysml_conditional_36 = 1;
    }
    else {
        sysml_conditional_36 = 0;
    }
    sysml_value_1 = ((int32_t)(((double)io_7) / 10000)) - sysml_conditional_36;
    int32_t sysml_value_2 = 0;
    if (sysml_state_selectedItem == 1) {
        sysml_conditional_37 = sysml_state_stockOne;
    }
    else {
        if (sysml_state_selectedItem == 2) {
            sysml_conditional_38 = sysml_state_stockTwo;
        }
        else {
            sysml_conditional_38 = sysml_state_stockThree;
        }
        sysml_conditional_37 = sysml_conditional_38;
    }
    sysml_value_2 = sysml_conditional_37;
    int32_t sysml_value_3 = 0;
    if (sysml_state_selectedItem == 1) {
        sysml_conditional_39 = sysml_state_priceOne;
    }
    else {
        if (sysml_state_selectedItem == 2) {
            sysml_conditional_40 = sysml_state_priceTwo;
        }
        else {
            sysml_conditional_40 = sysml_state_priceThree;
        }
        sysml_conditional_39 = sysml_conditional_40;
    }
    sysml_value_3 = sysml_conditional_39;
    bool sysml_value_4 = 0;
    sysml_value_4 = sysml_state_adminMode;
    int32_t sysml_value_5 = 0;
    if (sysml_state_beepRemaining > 0) {
        sysml_conditional_41 = sysml_state_beepFrequency;
    }
    else {
        sysml_conditional_41 = 0;
    }
    sysml_value_5 = sysml_conditional_41;
    bool sysml_value_6 = 0;
    sysml_value_6 = (sysml_state_saleItem == 1);
    bool sysml_value_7 = 0;
    sysml_value_7 = (sysml_state_saleItem == 2);
    bool sysml_value_8 = 0;
    sysml_value_8 = (sysml_state_saleItem == 3);
    int32_t sysml_value_9 = 0;
    sysml_value_9 = (io_7 - (sysml_value_1 * 10000));
    int32_t sysml_value_10 = 0;
    if (sysml_state_displayError) {
        sysml_conditional_42 = -1;
    }
    else {
        if (sysml_state_selectedItem == 0) {
            sysml_conditional_43 = 10000;
        }
        else {
            if (sysml_state_displayPhase < 1000) {
                sysml_conditional_44 = sysml_state_selectedItem;
            }
            else {
                sysml_conditional_44 = sysml_value_3;
            }
            sysml_conditional_43 = sysml_conditional_44;
        }
        sysml_conditional_42 = sysml_conditional_43;
    }
    sysml_value_10 = sysml_conditional_42;
    *io_4 = sysml_value_4;
    *io_5 = sysml_value_5;
    *io_6 = sysml_value_10;
    *io_8 = sysml_value_6;
    *io_9 = sysml_value_7;
    *io_10 = sysml_value_8;
}
// END COMPILED SYSML MODEL

// LLM4MDD target-board: arduino:avr:uno
#include <TM1637Display.h>
#include <Arduino.h>
#include <math.h>
#if defined(ARDUINO) && !defined(ARDUINO_AVR_UNO)
#error "Selected Arduino board differs from the interface contract"
#endif
static uint32_t hw_last_step = 0;
static bool hw_started = false;
static bool hw_value_io_0 = false;
static uint32_t hw_sampled_io_0 = 0;
static bool hw_attempted_io_0 = false;
static bool hw_value_io_1 = false;
static uint32_t hw_sampled_io_1 = 0;
static bool hw_attempted_io_1 = false;
static bool hw_value_io_2 = false;
static uint32_t hw_sampled_io_2 = 0;
static bool hw_attempted_io_2 = false;
static bool hw_value_io_3 = false;
static uint32_t hw_sampled_io_3 = 0;
static bool hw_attempted_io_3 = false;
static bool hw_value_io_4 = false;
static int32_t hw_value_io_5 = 0;
static int32_t hw_tone_io_5 = -1;
static int32_t hw_value_io_6 = 10000;
TM1637Display hw_display_io_6(8, 5);
static int32_t hw_display_previous_io_6 = INT32_MIN;
static int32_t hw_value_io_7 = 0;
#include <string.h>
static char hw_rx_buffer[64];
static char hw_rx_text[64];
static uint8_t hw_rx_size = 0;
static bool hw_rx_overflow = false;
static uint32_t hw_rx_last_byte = 0;
static bool hw_rx_parse(int32_t* value) {
  hw_rx_buffer[hw_rx_size] = '\0';
  char* begin = hw_rx_buffer;
  while (*begin == ' ' || *begin == '\t') ++begin;
  char* end = hw_rx_buffer + hw_rx_size;
  while (end > begin && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
  if (!*begin) return false;
  char* space=strchr(begin,' ');if(!space) return false;*space++='\0';
  while(*space==' '||*space=='\t')++space;if(!*space)return false;
  uint32_t payload=0;
  for(char* cursor=space;*cursor;++cursor){if(*cursor<'0'||*cursor>'9')return false;payload=payload*10+(*cursor-'0');if(payload>9999)return false;}
  if(payload<0 || payload>9999) return false;
  if(strcmp(begin,"RESTOCK1")==0) {*value=1L*10000L+(int32_t)payload;return true;}
  if(strcmp(begin,"RESTOCK2")==0) {*value=2L*10000L+(int32_t)payload;return true;}
  if(strcmp(begin,"RESTOCK3")==0) {*value=3L*10000L+(int32_t)payload;return true;}
  return false;
}
static bool hw_rx_finish(int32_t* value) {
  bool accepted = !hw_rx_overflow && hw_rx_parse(value);
  hw_rx_size = 0; hw_rx_overflow = false;
  return accepted;
}
static bool hw_rx_poll(uint32_t hw_now, int32_t* value) {
  for (uint8_t budget = 0; budget < 32 && Serial.available() > 0; ++budget) {
    int byte = Serial.read();
    if (byte < 0) break;
    hw_rx_last_byte = hw_now;
    if (byte == '\r' || byte == '\n') {
      if (!hw_rx_size && !hw_rx_overflow) continue;
      return hw_rx_finish(value);
    }
    if (byte == 0 || byte > 127) hw_rx_overflow = true;
    if (hw_rx_size < 63) hw_rx_buffer[hw_rx_size++] = (char)byte;
    else hw_rx_overflow = true;
  }
  if (Serial.available() > 0) return false;
  if ((hw_rx_size || hw_rx_overflow) && (uint32_t)(hw_now - hw_rx_last_byte) >= 20UL) return hw_rx_finish(value);
  return false;
}
static bool hw_value_io_7_valid = false;
static bool hw_value_io_8 = false;
static bool hw_value_io_9 = false;
static bool hw_value_io_10 = false;
static int32_t hw_value_io_11 = 0;

void setup() {
  ProgramPage1_Init(&ProgramPage1_instance);
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  pinMode(6, OUTPUT);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  hw_display_io_6.setBrightness(7, true);
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
}

void loop() {
  uint32_t hw_now = (uint32_t)millis();
  uint32_t hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_started && hw_elapsed < 5UL) return;
  if (hw_elapsed > 2147483647UL) return;
  if (!hw_attempted_io_0 || (uint32_t)(hw_now - hw_sampled_io_0) >= 5UL) {
    hw_attempted_io_0 = true; hw_sampled_io_0 = hw_now;
    hw_value_io_0 = (digitalRead(2) == LOW);
  }
  if (!hw_attempted_io_1 || (uint32_t)(hw_now - hw_sampled_io_1) >= 5UL) {
    hw_attempted_io_1 = true; hw_sampled_io_1 = hw_now;
    hw_value_io_1 = (digitalRead(3) == LOW);
  }
  if (!hw_attempted_io_2 || (uint32_t)(hw_now - hw_sampled_io_2) >= 5UL) {
    hw_attempted_io_2 = true; hw_sampled_io_2 = hw_now;
    hw_value_io_2 = (digitalRead(4) == LOW);
  }
  if (!hw_attempted_io_3 || (uint32_t)(hw_now - hw_sampled_io_3) >= 5UL) {
    hw_attempted_io_3 = true; hw_sampled_io_3 = hw_now;
    hw_value_io_3 = (digitalRead(7) == LOW);
  }
  hw_now = (uint32_t)millis();
  hw_elapsed = hw_started ? (uint32_t)(hw_now - hw_last_step) : 0;
  if (hw_elapsed > 2147483647UL) return;
  hw_value_io_11 = (int32_t)hw_elapsed;
  hw_value_io_7_valid = hw_rx_poll(hw_now, &hw_value_io_7);
  hw_last_step = hw_now; hw_started = true;
  ProgramPage1(hw_value_io_0, hw_value_io_1, hw_value_io_2, hw_value_io_3, hw_value_io_7, hw_value_io_7_valid, hw_value_io_11, &hw_value_io_4, &hw_value_io_5, &hw_value_io_6, &hw_value_io_8, &hw_value_io_9, &hw_value_io_10);
  digitalWrite(13, hw_value_io_4 ? HIGH : LOW);
  int32_t hw_tone_command_io_5 = (hw_value_io_5 >= 31 && hw_value_io_5 <= 65535) ? hw_value_io_5 : 0;
  if (hw_tone_command_io_5 != hw_tone_io_5) {
    if (hw_tone_command_io_5 == 0) noTone(6);
    else tone(6, (unsigned int)hw_tone_command_io_5);
    hw_tone_io_5 = hw_tone_command_io_5;
  }
  if (hw_value_io_6 != hw_display_previous_io_6) {
    if (hw_value_io_6 < -999 || hw_value_io_6 > 9999) hw_display_io_6.clear();
    else hw_display_io_6.showNumberDec((int)hw_value_io_6, false, 4, 0);
    hw_display_previous_io_6 = hw_value_io_6;
  }
  if (hw_value_io_8) Serial.println("dispense:item1");
  if (hw_value_io_9) Serial.println("dispense:item2");
  if (hw_value_io_10) Serial.println("dispense:item3");
}
