#include "msp430fr2355.h"
#include <msp430.h>
#include <stdbool.h>

/** Jack Glutting-Gilsdorf, Nick Haldar, Ryan Adolph
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

    //----- Setup light bar output pins   ( P6.0 -> P6.4, P3.7, P2.4, P3.3. P3.2, P3.0)--------
    P6SEL0 |= 0b00011111;   // P6.0 -> P6.4
    P6SEL1 |= 0b00011111;   // make IO
    P6DIR |= 0b00011111;    // make it output
    P6REN &= ~0b00011111;   // give pull down resistor
    P6OUT &= ~0b00011111;   // clear output
    
    P3SEL0 |= 0b10001101;   // P3.7, P3.3, P3.2, P3.0
    P3SEL1 |= 0b10001101;
    P3DIR |= 0b10001101;
    P3REN &= ~0b10001101;
    P3OUT &= ~0b10001101;

    P2SEL0 |= BIT4;         // P2.4
    P2SEL1 |= BIT4;
    P2DIR |= BIT2;
    P2REN &= ~BIT2;
    P2OUT &= ~BIT2;
    // ------- light bar done -----


    //----- Setup Keypad ports (P1.4 -> P1.7) (P5.0, P5.1, P5.3, P5.4)--------
        // keypad outputs (P1.4 -> P1.7)
    P4SEL0 |= 0b11110000;       // P1.4 -> P1.7
    P4SEL1 |= 0b11110000;       // make IO
    P1DIR |=  0b11110000;       // configure P1's as output
	P1OUT &=  ~0b11110000;      // clearing P1 output

        // keypad inputs (P5.0, P5.1, P5.3, P5.4)
    P5SEL0 |= 0b00011011;       // P5.0, P5.1, P5.3, P5.4
    P5SEL1 |= 0b00011011;       // make IO
    P5DIR &=  ~0b00011011;      // configure P5's as Input
	P5OUT &=  ~0b00011011;      // clearing P5 output
    // ------- keypad ports done -----


    //----- Setup RGB LED ports (P2.0, P2.2, P4.0)--------
    P2SEL0 |= 0b00000101;       // P2.0, P2.2
    P2SEL1 |= 0b00000101;       // make IO
    P2DIR |=  0b00000101;       // configure P2's output
	P2OUT &=  ~0b00000101;      // clearing P2 output
    // ------- RGB LED done -----

    //----- Setup Timer B1 (USE FOR RED PIN RGB LED)--------
    TB1CTL |= TBCLR;            // Clear Timers and Dividers
    TB1CTL |= TBSSEL__SMCLK;    // Source = SMCLK
    TB1CTL |= MC__UP;           // Mode   = Up
    TB1CTL |= CNTL__16;         // Choose counter length (16 bits)
    TB1CTL |= ID__4;            // Choose D1
    TB1EX0 |= TBIDEX__7;        // Choose D2 such that D1 * D2 = D
    TB1CCR0 = 214;              // TB0CCR0 = 214

    //----- Setup Timers Overflow IQR--------
    TB1CCTL0 &= ~CCIFG;           //Clear CCR0 flag
    TB1CCTL0 |= CCIE;             //Enable CCR0 Overflow IQR

    TB1CCTL1 &= ~CCIFG;           //Clear CCR1 flag

    PM5CTL0 &= ~LOCKLPM5;       // Turn off GPIO

    while (true)
    {

        P3OUT |= BIT4;
        P3OUT |= BIT1;
        
        P6IN |= BIT2;
        P6IN |= BIT3;

    }
}

// Timer B0 Overflow ISR
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_Overflow(void)
{
    if (TB0CTL & TBIFG)  // Check if overflow flag is set
    {
        P1OUT ^= BIT0;  // Toggle LED1
        TB0CTL &= ~TBIFG;  // Clear interrupt flag
    }
}

// Timer B1 Overflow ISR
#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB1_Overflow(void)
{
    if (TB0CTL & TBIFG)  // Check if overflow flag is set
    {
        P1OUT ^= BIT0;  // Toggle LED1
        TB0CTL &= ~TBIFG;  // Clear interrupt flag
    }
}
