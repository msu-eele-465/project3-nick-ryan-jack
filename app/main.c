#include <msp430.h>
#include <stdbool.h>

/** Jack Glutting-Gilsdorf, Nick _____ , Ryan _______
* Project #3
* 2/13/2025
* EELE465
*/
int arrayCounter = 0;
int DrillOn[] = {1, 2, 4, 8};

void HeartBeat()
{
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
   //----- Setup Keypad ports (P1.4 -> P1.7) (P5.0, P5.1, P5.3, P5.4)--------
        // keypad outputs (P1.4 -> P1.7)
    P1SEL0 |= 0b11110000;       // P1.4 -> P1.7
    P1SEL1 |= 0b11110000;       // make IO
    P1DIR |=  0b11110000;       // configure P1's as output
	P1OUT &=  ~0b11110000;      // clearing P1 output

        // keypad inputs (P5.0, P5.1, P5.3, P5.4)
    P5SEL0 |= 0b00011011;       // P5.0, P5.1, P5.3, P5.4
    P5SEL1 |= 0b00011011;       // make IO
    P5DIR &=  ~0b00011011;      // configure P5's as Input
	P5OUT &=  ~0b00011011;      // clearing P5 output
    // ------- keypad ports done -----

    //Setup Timer B0
        TB0CTL |= TBCLR;            // clear timer and dividers
        TB0CTL |= TBSSEL__SMCLK;    // source = SMCLK
        TB0CTL |= MC__UP;           // mode = up
        TB0CTL |= ID__1;            // D1 = 1
        TB0EX0 |=  TBIDEX__1;       // D2 = 1

        TB0CCR0 = 4678;            // CCR0 = 4678
                                       //(0.004678s * 1,000,000 Hz) / (1*1) = 4678

    //----- Setup Timers Overflow IQR--------
    TB0CCTL0 &= ~CCIFG;           //Clear CCR0 flag
    TB0CCTL0 |= CCIE;             //Enable CCR0 Overflow IQR
    TB0CCTL1 &= ~CCIFG;           //Clear CCR1 flag
    TB0CCTL1 |= CCIE;             //Enable CCR1 Overflow IQR

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
        DrillOn[arrayCounter] = P1OUT;
    };
}
 

// Timer B0 Overflow ISR
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void)
{
    if (TB0CTL & TBIFG)  // Check if overflow flag is set
    {
        P1OUT ^= BIT0;  // Toggle LED1
        TB0CTL &= ~TBIFG;  // Clear interrupt flag
    }

    if (arrayCounter == 3){ 
        arrayCounter = 0;         // increment array position variable

    }
    else{
        arrayCounter++;
    }

    TB0CCTL0 &= ~CCIFG;         // clear TB0 flag  
}

