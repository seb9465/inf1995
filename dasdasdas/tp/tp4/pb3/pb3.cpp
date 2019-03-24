/// PWM de façon matérielle

// Connexions
//
// Roue gauche:
// La broche D5 (PWM OC1A) se connecte à la broche E du connecteur JM2 du pont en H,
// et la broche D7 se connecte à la broche D du même connecteur
//
// Roue droite:
// La broche D6 (PWM OC1B) se connecte à la broche E du connecteur JM1 du pont en H,
// et la broche D8 se connecte à la broche D du même connecteur

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8'000'000
#include <util/delay.h>

namespace
{
    const uint8_t pwmTop = 0xFF;
    const uint8_t speed25 = 0x40;
    const uint8_t speed50 = 0x80;
    const uint8_t speed75 = 0xC0;
    const uint8_t speed100 = pwmTop;
}

/// Initialize initial program state
void initialize()
{
    cli(); // Clear global interrupt flag to disable interrupts

    // Setup pins
    DDRD |= 0b1111'0000; // Output mode for OC1A on OCR1A match with TCNT1, OC1B on OCR1B match with TCNT1, and direction ports D7 and D8
    PORTD &= ~(1 << 7 | 1 << 6); // Set D (direction) ports on H bridge JM1 and JM2 connector to 0

    sei(); // Set global interrupt flag to enable interrupts
}

/// Adjust PWM timer and output compare registers for motor control
/// \param dutyCycle Value between 0 and 255 to control pulse width (intensity from 0 to 255)
void adjustPwm(uint8_t dutyCycle)
{
    OCR1A = dutyCycle;
    OCR1B = dutyCycle;

    // Timer counter control register flags (3x8 bits)
    TCCR1A |= (1 << WGM10);  // Set wave generation mode to phase-correct 8-bit (0x00FF TOP value) PWM by setting WGM1 bits to 001
    TCCR1A |= (1 << COM1A1); // Set compare output mode to clear OC1A on compare match when up-counting and vice-versa by setting COM1A bits to 10
    TCCR1A |= (1 << COM1B1); // Set compare output mode to clear OC1B on compare match when up-counting and vice-versa by setting COM1B bits to 10
    TCCR1B |= (1 << CS11);   // Set clock select to clock divided by 8 by setting CS1 bits to 010
    TCCR1C = 0;              // Not used in this particular case
}

int main()
{
    initialize();

    while (true)
    {
        adjustPwm(0);
        _delay_ms(2000);
        adjustPwm(speed25);
        _delay_ms(2000);
        adjustPwm(speed50);
        _delay_ms(2000);
        adjustPwm(speed75);
        _delay_ms(2000);
        adjustPwm(speed100);
        _delay_ms(2000);
    }
}