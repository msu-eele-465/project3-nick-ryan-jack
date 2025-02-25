#ifndef RGB_PWM_H
#define RGB_PWM_H

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

#endif // RGB_PWM_H
