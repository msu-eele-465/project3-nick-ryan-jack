#include "msp430fr2355.h"
#include <msp430.h>
#include <stdbool.h>
#include "LED_BAR.h"
//#include "LED_BAR.c"


// Global variables
int arrayCounter = 0;
int DrillOn[] = {1, 2, 4, 8};
int keypadInput[16] = {0};  // initialize all elements to 0
int col1Input = 0;
int col2Input = 0;
int col3Input = 0;
int col4Input = 0;
bool testHigh = false;
char button_input;
char passkey[] = {'1', '3', 'C', '0'};
char user_input[4] = {'0'};
int locked;


void Keypad_init(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
   
    // --- Setup Keypad Ports ---
    // Configure P1.4-P1.7 as outputs (for driving keypad rows)
    P1DIR |= BIT0 | BIT4 | BIT5 | BIT6 | BIT7;
    P1OUT &= ~(BIT0 | BIT4 | BIT5 | BIT6 | BIT7); // start with all rows low
   
    // Configure P5.0-P5.3 as inputs (for reading keypad columns)
    P5DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    P5REN &= BIT0 | BIT1 | BIT2 | BIT3;
    P5OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

    P6DIR |= BIT6;
    P6OUT &= ~BIT6;
   
    // --- Setup Timer B0 ---
    TB0CTL = TBCLR;                      // clear timer
    TB0CTL |= TBSSEL__SMCLK | MC__UP;      // use SMCLK in Up mode
    TB0CTL |= ID__4;                     // Divider /4
    TB0EX0 |= TBIDEX__2;                 // Extended divider /2 (total division factor = 8)
    TB0CCR0 = 9356;                      // Timer period (adjust as needed) PREV 4678 -> changed to double for better reads
   
    // Enable CCR0 interrupt
    TB0CCTL0 &= ~CCIFG;                  // Clear flag
    TB0CCTL0 |= CCIE;                    // Enable interrupt
   
    __enable_interrupt();                // Enable maskable interrupts
}


void locked(){
    locked = 1;
}

void unlocked(){
    locked =0;
}

void Passkey_func(char input){

    if(locked == 1){
        
    }else if(input == 'D'){
        locked();
    }
    choose_pattern(input);


}

int main(void)
{
    // Stop watchdog timer and disable default high-impedance mode
    PM5CTL0 &= ~LOCKLPM5;
   
    Keypad_init();
    Initialize_LEDBAR();
    // Main loop enters low-power mode; all processing is in the ISR.
    while(1)
    {
        __low_power_mode_0(); // Sleep until an interrupt occurs
    }
}


// Timer B0 CCR0 ISR
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void)
{
    // Static counter to toggle LED only every ~0.5s.
    // Based on a SMCLK = 1MHz, divided by 8 (from ID__4 and TBIDEX__2),
    // the timer clock is 125kHz and the period is ~4679/125000 ≈ 0.03743s.
    // About 14 interrupts yield ~0.524s.
    static unsigned int toggleCount = 0;
   
    // --- Keypad Scanning via Switch-Case ---
    switch(arrayCounter)
    {
        case 0:
            // Activate row 1 (P1.4) and clear other rows.
            P1OUT |= BIT4;
            P1OUT &= ~(BIT5 | BIT6 | BIT7);
            col4Input = P5IN;  // Sample keypad column inputs
            switch(col4Input){
                case 0b0001:
                    button_input = '1';
                    P6OUT ^= BIT6;
                    Passkey_func(button_input);
                    break;
                case 0b0010:
                    button_input = '4';
                    P6OUT ^= BIT6;
                    Passkey_func(button_input);
                    break;
                case 0b0100:
                    button_input = '7';
                    P6OUT ^= BIT6;
                    break;
                case 0b1000:
                    button_input = '*';
                    P6OUT ^= BIT6;
                    break;
            };
            break;
           
        case 1:
            // Activate row 2 (P1.5)
            P1OUT |= BIT5;
            P1OUT &= ~(BIT4 | BIT6 | BIT7);
            col3Input = P5IN;
            switch(col3Input){
                case 0b0001:
                    button_input = '2';
                    P6OUT ^= BIT6;
                    break;
                case 0b0010:
                    button_input = '5';
                    P6OUT ^= BIT6;
                    break;
                case 0b0100:
                    button_input = '8';
                    P6OUT ^= BIT6;
                    break;
                case 0b1000:
                    button_input = '0';
                    P6OUT ^= BIT6;
                    break;
            };
            break;
           
        case 2:
            // Activate row 3 (P1.6)
            P1OUT |= BIT6;
            P1OUT &= ~(BIT4 | BIT5 | BIT7);
            col2Input = P5IN;
            switch(col2Input){
                case 0b0001:
                    button_input = '3';
                    P6OUT ^= BIT6;
                    break;
                case 0b0010:
                    button_input = '6';
                    P6OUT ^= BIT6;
                    break;
                case 0b0100:
                    button_input = '9';
                    P6OUT ^= BIT6;
                    break;
                case 0b1000:
                    button_input = '#';
                    P6OUT ^= BIT6;
                    break;
            };
            break;
           
        case 3:
            // Activate row 4 (P1.7)
            P1OUT |= BIT7;
            P1OUT &= ~(BIT4 | BIT5 | BIT6);
            if (P5IN == 0b00001000)  // Example condition for key detection
            {
                testHigh = true;
                keypadInput[0] = 1;
            }
            col1Input = P5IN;
            switch(col1Input){
                case 0b0001:
                    button_input = 'A';
                    P6OUT ^= BIT6;
                    break;
                case 0b0010:
                    button_input = 'B';
                    P6OUT ^= BIT6;
                    break;
                case 0b0100:
                    button_input = 'C';
                    P6OUT ^= BIT6;
                    break;
                case 0b1000:
                    button_input = 'D';
                    P6OUT ^= BIT6;
                    break;
            };
            break;
           
        default:
            break;
    }
   
    // Update the row counter, wrapping from 3 back to 0.
    arrayCounter = (arrayCounter + 1) % 4;
   
    // Increment toggleCount and toggle LED (P1.0) every ~0.5 seconds.
    toggleCount++;
    if(toggleCount >= 14)
    {
        P1OUT ^= BIT0;  // Toggle LED on P1.0
        toggleCount = 0;
    }
   
    // Clear CCR0 interrupt flag
    TB0CCTL0 &= ~CCIFG;
}





