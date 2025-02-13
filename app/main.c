#include <msp430.h>
#include <stdbool.h>

/** Jack Glutting-Gilsdorf, Nick _____ , Ryan _______
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
    PM5CTL0 &= ~LOCKLPM5;

    HeartBeat();

    while (true)
    {
        
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
