#ifndef LED_BAR_H
#define LED_BAR_H

#include <msp430.h>
#include <stdbool.h>

// Global Variables
extern int current_value;

extern int Upper_Lightbar_array0;
extern int Lower_Lightbar_array0;

extern int Upper_Lightbar_array1[];
extern int Lower_Lightbar_array1[];
extern int LIGHTBAR1_LENGTH;
extern int LIGHTBAR1_POS;

extern int Upper_Lightbar_value2;
extern int Lower_Lightbar_value2;
extern int LIGHTBAR2_LENGTH;
extern int LIGHTBAR2_POS;

extern int Upper_Lightbar_array3[];
extern int Lower_Lightbar_array3[];
extern int LIGHTBAR3_LENGTH;
extern int LIGHTBAR3_POS;

extern int Upper_Lightbar_value4;
extern int Lower_Lightbar_value4;
extern int LIGHTBAR4_LENGTH;
extern int LIGHTBAR4_POS;

extern int Upper_Lightbar_array5[];
extern int Lower_Lightbar_array5[];
extern int LIGHTBAR5_LENGTH;
extern int LIGHTBAR5_POS;

extern int Upper_Lightbar_array6[];
extern int Lower_Lightbar_array6[];
extern int LIGHTBAR6_LENGTH;
extern int LIGHTBAR6_POS;

extern int Upper_Lightbar_array7[];
extern int Lower_Lightbar_array7[];
extern int LIGHTBAR7_LENGTH;
extern int LIGHTBAR7_POS;

extern int delay_var;
extern int pattern;

// Function Prototypes
void delay_loop();
void Initialize_LEDBAR();
int choose_pattern(char input);

#endif // LED_BAR_H