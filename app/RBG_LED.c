#include "msp430fr2355.h"
#include <msp430.h>
#include <stdint.h>


#define PWM_MAX 255


// Define LED pins with new mapping:
// Red: P2.0, Green: P2.2, Blue: P4.0
#define RED_PIN   BIT0  // on Port 2
#define GREEN_PIN BIT2  // on Port 2
#define BLUE_PIN  BIT0  // on Port 4


// Function prototypes
void initPWM(void);
void setColorLocked(void);
void setColorUnlocking(void);
void setColorUnlocked(void);
void setCustomColor(uint8_t red, uint8_t green, uint8_t blue);


void Workin(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
   
    // Initialize PWM and RGB LED ports
    initPWM();
   
    // Start in the locked state (LED color: #c43e1d)
    setColorLocked();
   
    while (1) {
        // For demonstration, cycle through the different LED states.
        __delay_cycles(1000000);  // Adjust delay as needed for your clock
        setColorUnlocking();      // LED color: #c4921d
        __delay_cycles(1000000);
        setColorUnlocked();       // LED color: #1da2c4
        __delay_cycles(1000000);
        setColorLocked();
       
        // Extra Credit: Uncomment to set a custom color (e.g., purple)
        // setCustomColor(128, 0, 128);
        // __delay_cycles(1000000);
    }
   
    // Code never reaches here
    return 0;
}


void initPWM(void) {
    //----- Setup RGB LED ports (P2.0, P2.2, P4.0) --------
    // Configure Port 2 for Red (P2.0) and Green (P2.2)
    P2SEL0 |= RED_PIN | GREEN_PIN;   // Set alternate function for P2.0 and P2.2
    P2SEL1 |= RED_PIN | GREEN_PIN;   // to enable Timer_A output.
    P2DIR |= RED_PIN | GREEN_PIN;    // Set as outputs.
    P2OUT &= ~(RED_PIN | GREEN_PIN); // Clear outputs.
   
    // Configure Port 4 for Blue (P4.0)
    P4SEL0 |= BLUE_PIN;    // Set alternate function for P4.0
    P4SEL1 |= BLUE_PIN;    // to enable Timer_A output.
    P4DIR |= BLUE_PIN;     // Set as output.
    P4OUT &= ~BLUE_PIN;    // Clear output.
    // ------- RGB LED done --------


    // Initialize duty cycles to 0 (LED off)
    TB0CCR0 = 0;  // Red
    TB0CCR1 = 0;  // Green
    TB0CCR2 = 0;  // Blue
   
    // Start Timer_A in Up mode using SMCLK
    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__SMCLK;
    TBOCTL |= MC__UP;


    TB0CCTL0 &= ~CCIFG;
    TB0CCTL0 |= CCIE;


    TB0CCTL1 &= ~CCIFG;
    TB0CCTL1 |= CCIE;
   
    TB0CCTL2 &= ~CCIFG;
    TB0CCTL2 |= CCIE;


}


void setColorLocked(void) {
    // Locked state: LED color #c43e1d (Red=196, Green=62, Blue=29)
    TB0CCR0 = 196; // Red channel (P2.0)
    TB0CCR1 = 62;  // Green channel (P2.2)
    TB0CCR2 = 29;  // Blue channel (P4.0)
}


void setColorUnlocking(void) {
    // Unlocking state: LED color #c4921d (Red=196, Green=146, Blue=29)
    TB0CCR0 = 196; // Red channel
    TB0CCR1 = 146; // Green channel
    TB0CCR2 = 29;  // Blue channel
}


void setColorUnlocked(void) {
    // Unlocked state: LED color #1da2c4 (Red=29, Green=162, Blue=196)
    TB0CCR0 = 29;  // Red channel
    TB0CCR1 = 162; // Green channel
    TB0CCR2 = 196; // Blue channel
}


void setCustomColor(uint8_t red, uint8_t green, uint8_t blue) {
    // Extra Credit: Set a custom LED color using 8-bit RGB values (0-255)
    TB0CCR0 = red;
    TB0CCR1 = green;
    TB0CCR2 = blue;
