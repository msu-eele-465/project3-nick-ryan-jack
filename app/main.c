#include "msp430fr2355.h"
#include <msp430.h>
#include <stdbool.h>

/** Jack Glutting-Gilsdorf, Nick _____ , Ryan Adolph
* Project #3
* 2/13/2025
* EELE465
*/

void HeartBeat()
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // LED1 (P1.0) and LED2 (P1.1) set as output
    P1DIR |= BIT0 | BIT1;  // Set P1.0 (LED1) and P1.1 (LED2) as outputs
    P1OUT &= ~(BIT0 | BIT1);  // Ensure LEDs are off initially

    //----- Setup Timer B0 -----
    TB0CTL = TBCLR;          // Clear timer and dividers
    TB0CTL |= TBSSEL__ACLK;  // Select ACLK as timer source
    TB0CTL |= MC__CONTINUOUS;// Choose continuous counting mode
    TB0CTL |= CNTL_1;        // Using N = 2^12
    TB0CTL |= ID__8;         // Setting divider d1 = 8 (d2 default = 1), total D = 8
    TB0CTL |= TBIE;          // Enable overflow interrupt

    TB0CTL &= ~TBIFG;        // Clear interrupt flag

    __enable_interrupt();  // Enable maskable interrupts
}

int main(void)
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
    // Disable GPIO power-on default high-impedance mode
    

    HeartBeat();

    // light bar output pins
    P6SEL0 |= 0b00011111;
    P6SEL1 |= 0b00011111;   // make IO
    P6DIR |= 0b00011111;    // make it output
    P6REN &= ~0b00011111;   // give pull down resistor
    P6OUT &= ~0b00011111;   // clear output
    
    P3SEL0 |= 0b10001101;
    P3SEL1 |= 0b10001101;
    P3DIR |= 0b10001101;
    P3REN &= ~0b10001101;
    P3OUT &= ~0b10001101;

    P2SEL0 |= BIT4;
    P2SEL1 |= BIT4;
    P2DIR |= BIT2;
    P2REN &= ~BIT2;
    P2OUT &= ~BIT2;


    PM5CTL0 &= ~LOCKLPM5;
    while (true)
    {

        P3OUT |= BIT4;
        P3OUT |= BIT1;
        
        P6IN |= BIT2;
        P6IN |= BIT3;

    }
}

// Timer B0 Overflow ISR
#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_Overflow(void)
{
    if (TB0CTL & TBIFG)  // Check if overflow flag is set
    {
        P1OUT ^= BIT0;  // Toggle LED1
        TB0CTL &= ~TBIFG;  // Clear interrupt flag
    }
}
