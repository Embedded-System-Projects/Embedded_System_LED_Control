/*
 * File:   main.c
 * Author: Kaleab Tesfaye
 *
 * Created on November 3, 2024, 4:49 AM
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// Configuration Bits for PIC24F04KA200
#pragma config FNOSC = FRC       // Select Fast RC Oscillator as the clock source
#pragma config FWDTEN = OFF      // Disable Watchdog Timer

// Define the system clock frequency for delay calculations
#define _XTAL_FREQ 8000000       // 8 MHz internal oscillator

// Pin definitions for LEDs and the switch
#define GREEN_LED   LATAbits.LATA0   // Green LED connected to RA0
#define RED_LED     LATAbits.LATA1   // Red LED connected to RA1
#define WHITE_LED   LATAbits.LATA2   // White LED connected to RA2
#define SWITCH      PORTBbits.RB4    // Switch connected to RB4

// Function prototypes
void InitPIC24F(void);            // Initialize the microcontroller's pins and settings
void Timer1_Init(void);           // Set up Timer1 for LED toggling
void ToggleGreenLED(void);        // Function to toggle the Green LED
void CheckSwitch(void);           // Check the switch state and control Red/White LEDs

// Global variable to count Timer1 intervals
volatile uint16_t timerCounter = 0;

int main(void) {
    // Initialize the PIC24F
    InitPIC24F();
    
    // Set up Timer1 for periodic interrupts
    Timer1_Init();

    // Main program loop
    while (1) {
        // Continuously check the switch state
        CheckSwitch();
    }

    return 0; // Should never reach here in an embedded system
}

// Function to initialize microcontroller pins and settings
void InitPIC24F(void) {
    // Configure LED pins as outputs
    TRISAbits.TRISA0 = 0;  // Set RA0 (Green LED) as output
    TRISAbits.TRISA1 = 0;  // Set RA1 (Red LED) as output
    TRISAbits.TRISA2 = 0;  // Set RA2 (White LED) as output
    
    // Configure the switch pin as input
    TRISBbits.TRISB4 = 1;  // Set RB4 as input for the switch
    
    // Turn off all LEDs initially
    GREEN_LED = 0;
    RED_LED = 0;
    WHITE_LED = 0;
}

// Function to initialize Timer1
void Timer1_Init(void) {
    T1CON = 0x8030;    // Enable Timer1, prescaler 256
    TMR1 = 0;          // Reset Timer1 counter
    PR1 = 7812;        // Set Timer1 period for a 1-second delay
    IFS0bits.T1IF = 0; // Clear Timer1 interrupt flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupts
}

// Timer1 interrupt service routine
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    timerCounter++;              // Increment the timer interval counter
    if (timerCounter >= 3) {     // Toggle the Green LED every 3 seconds
        ToggleGreenLED();
        timerCounter = 0;        // Reset the counter after toggling
    }else {
        
           GREEN_LED = 0; // Ensure the Green LED is off when the switch is not pressed
   
           }

    IFS0bits.T1IF = 0;           // Clear the Timer1 interrupt flag
}

// Function to toggle the Green LED
void ToggleGreenLED(void) {
    GREEN_LED = !GREEN_LED;      // Toggle the Green LED state
}

// Function to check the switch state and control Red/White LEDs
void CheckSwitch(void) {
    if (SWITCH == 0) {           // If the switch is pressed (active low)
        RED_LED = 1;             // Turn on the Red LED
        WHITE_LED = 0;           // Turn off the White LED
    } else {                     // If the switch is not pressed
        GREEN_LED = 0;
        RED_LED = 0;             // Turn off the Red LED
        WHITE_LED = 1;           // Turn on the White LED
    }
}
