#include <msp430.h>
#include <stdbool.h>


/** Jack Glutting-Gilsdorf, Nick Haldar , Ryan Adolph
* Project #3
* 2/13/2025
* EELE465
*/

int current_value;

int Upper_Lightbar_array0 = 0b10100000;
int Lower_Lightbar_array0 = 0b00001010;

int Upper_Lightbar_array1[] = {0b10100000, 0b01010000};
int Lower_Lightbar_array1[] = {0b00001010, 0b00000101};
int LIGHTBAR1_LENGTH = 1;
int LIGHTBAR1_POS;

int Upper_Lightbar_value2;
int Lower_Lightbar_value2;
int LIGHTBAR2_LENGTH = 15;
int LIGHTBAR2_POS; // instead of doing this just increment P6 by one 16 times, and increment P4 by 16 every 16 times

int Upper_Lightbar_array3[] = {0b00010000, 0b00100000, 0b01000000, 0b10000000, 0b01000000, 0b00100000};
int Lower_Lightbar_array3[] = {0b00001000, 0b00000100, 0b00000010, 0b00000001, 0b00000010, 0b00000100};
int LIGHTBAR3_LENGTH = 5;
int LIGHTBAR3_POS;

int Upper_Lightbar_value4;
int Lower_Lightbar_value4;
int LIGHTBAR4_LENGTH = 15;
int LIGHTBAR4_POS; // instead of doing this just decrement P6 by one 16 times, and decrement P4 by 16 every 16 times

int Upper_Lightbar_array5[] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};
int Lower_Lightbar_array5[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};
int LIGHTBAR5_LENGTH = 7;
int LIGHTBAR5_POS;

int Upper_Lightbar_array6[] = {0b01110000, 0b10110000, 0b11010000, 0b11100000, 0b11110000, 0b11110000, 0b11110000, 0b11110000};
int Lower_Lightbar_array6[] = {0b00001111, 0b0001111, 0b00001111, 0b00001111, 0b00000111, 0b00001011, 0b00001101, 0b00001110};
int LIGHTBAR6_LENGTH = 7;
int LIGHTBAR6_POS;

int Upper_Lightbar_array7[] = {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00010000, 0b00110000, 0b01110000, 0b11110000};
int Lower_Lightbar_array7[] = {0b00000001, 0b0000011, 0b00000111, 0b00001111, 0b00001111, 0b00001111, 0b00001111, 0b00001111};
int LIGHTBAR7_LENGTH = 7;
int LIGHTBAR7_POS;

int delay_var;


int pattern = 4; 



void delay_loop(){ 
    for(delay_var = 0; delay_var < 20000; delay_var++);
}


void Initialize_LEDBAR()
{
   // Stop watchdog timer
   WDTCTL = WDTPW | WDTHOLD;


    //----- Setup light bar output pins   ( (Lower) P6.0 -> P6.3, (Upper) P4.4 -> P4.7 )--------
        //P6SEL0 |= 0b00011111;   // P6.0 -> P6.3
        //P6SEL1 |= 0b00011111;   // make IO
        P6DIR |= 0b00001111;    // make it output
        P6REN &= ~0b00001111;   // give pull down resistor
        P6OUT &= ~0b00001111;   // clear output
        
        //P3SEL0 |= 0b10001101;   // P4.4 -> P4.7
        //P3SEL1 |= 0b10001101;
        P4DIR |= 0b11110000;
        P4REN &= ~0b11110000;
        P4OUT &= ~0b11110000;

    // ------- light bar done -----
}


int choose_pattern(char input)
{

    // clear R3 - 6
    //R3 |= 0b00000000;
    //R4 |= 0b00000000;
    //R5 |= 0b00000000;
    //R6 |= 0b00000000;


    

   // Stop watchdog timer
   WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
  
   // Disable GPIO power-on default high-impedance mode
   PM5CTL0 &= ~LOCKLPM5;


        //delay_loop();
        switch(input) {
            case '0':
                P6OUT = Lower_Lightbar_array0;
                P4OUT = Upper_Lightbar_array0;

                break;
           case '1':
                if(LIGHTBAR1_POS == LIGHTBAR1_LENGTH){ 
                    P6OUT = Lower_Lightbar_array1[LIGHTBAR1_POS];
                    P4OUT = Upper_Lightbar_array1[LIGHTBAR1_POS];
                    LIGHTBAR1_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_array1[LIGHTBAR1_POS];
                P4OUT = Upper_Lightbar_array1[LIGHTBAR1_POS];
                LIGHTBAR1_POS++;
                break;
           case '2': 
                if(LIGHTBAR2_POS == LIGHTBAR2_LENGTH){ 
                    if(Upper_Lightbar_value2 == 0b11110000){
                        Upper_Lightbar_value2 = 0;
                    }else{Upper_Lightbar_value2 += 16;}
                    Lower_Lightbar_value2 = 0;
                    P6OUT = Lower_Lightbar_value2;
                    P4OUT = Upper_Lightbar_value2;
                    LIGHTBAR2_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_value2;
                Lower_Lightbar_value2 ++;
                LIGHTBAR2_POS++;
                break;

           case '3':
                if(LIGHTBAR3_POS == LIGHTBAR3_LENGTH){ 
                    P6OUT = Lower_Lightbar_array3[LIGHTBAR3_POS];
                    P4OUT = Upper_Lightbar_array3[LIGHTBAR3_POS];
                    LIGHTBAR3_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_array3[LIGHTBAR3_POS];
                P4OUT = Upper_Lightbar_array3[LIGHTBAR3_POS];
                LIGHTBAR3_POS++;
                break;

            case '4': 
                if(Lower_Lightbar_value4 == 0b00000000 && LIGHTBAR4_POS == 0 && Upper_Lightbar_value4 == 0b11110000){
                     Upper_Lightbar_value4 = 0b11110000;}
                if(LIGHTBAR4_POS == LIGHTBAR4_LENGTH){ 
                    if(Lower_Lightbar_value4 == 0b00000000){
                        Upper_Lightbar_value4 = 0b11110000;
                    }else{Upper_Lightbar_value4 -= 16;}
                    Lower_Lightbar_value4 = 0b00001111;
                    P6OUT = Lower_Lightbar_value4;
                    P4OUT = Upper_Lightbar_value4;
                    LIGHTBAR4_POS = 0;
                    break;
                }
                Lower_Lightbar_value4 --;
                P6OUT = Lower_Lightbar_value4;
                LIGHTBAR4_POS++;
                break;
            case '5':
                if(LIGHTBAR5_POS == LIGHTBAR5_LENGTH){ 
                    P6OUT = Lower_Lightbar_array5[LIGHTBAR5_POS];
                    P4OUT = Upper_Lightbar_array5[LIGHTBAR5_POS];
                    LIGHTBAR5_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_array5[LIGHTBAR5_POS];
                P4OUT = Upper_Lightbar_array5[LIGHTBAR5_POS];
                LIGHTBAR5_POS++;
                break;

            case '6':
                if(LIGHTBAR6_POS == LIGHTBAR6_LENGTH){ 
                    P6OUT = Lower_Lightbar_array6[LIGHTBAR6_POS];
                    P4OUT = Upper_Lightbar_array6[LIGHTBAR6_POS];
                    LIGHTBAR6_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_array6[LIGHTBAR6_POS];
                P4OUT = Upper_Lightbar_array6[LIGHTBAR6_POS];
                LIGHTBAR6_POS++;
                break;
            case '7':
                if(LIGHTBAR7_POS == LIGHTBAR7_LENGTH){ 
                    P6OUT = Lower_Lightbar_array7[LIGHTBAR7_POS];
                    P4OUT = Upper_Lightbar_array7[LIGHTBAR7_POS];
                    LIGHTBAR7_POS = 0;
                    break;
                }
                P6OUT = Lower_Lightbar_array7[LIGHTBAR7_POS];
                P4OUT = Upper_Lightbar_array7[LIGHTBAR7_POS];
                LIGHTBAR7_POS++;
                break;
       }; 

}








