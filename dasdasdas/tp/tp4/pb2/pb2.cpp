/// Jeu de réflexe avec l'utilisation d'une minuterie et des interruptions

// La broche A1 se connecte à la broche positive de la DEL
// et la broche A2 se connecte à la broche négative de la DEL

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8'000'000
#include <util/delay.h>

// Constants
namespace
{
    const uint16_t msDelayLoopMultiplier = 2000; // 8'000'000 / 1000 / 4 = 2000 : coefficient for generating a 1 ms delay for _delay_loop_2
}

/// Button states
enum class ButtonState : uint8_t
{
    Bounced,
    Pressed,
    Released
};

// Globals
volatile bool isTimerExpired;
volatile bool isButtonPressed;

/// Initialize initial program state
void initialize()
{
    cli(); // Clear global interrupt flag to disable interrupts

    // Setup pins
    DDRD = 0; // Input mode for interrupt button on D2
    DDRA = 0b11; // Output mode for LED on A0 and A1

    // Initialize globals
    isTimerExpired = false;
    isButtonPressed = false;

    // Enable external interrupts
    EIMSK |= (1 << INT0);

    // INT0 interrupt generation on rising edge with interrupt sense control bits
    EICRA |= (1 << ISC01) | (1 << ISC00); // Set ISC0 bits to 11

    sei(); // Set global interrupt flag to enable interrupts
}

/// Get the current state of the interrupt button with a software-controlled debouncer
/// \return The state of the button, of type ButtonState
ButtonState getInterruptButtonState()
{
    // Check button state twice to debounce
    static const uint8_t buttonPin = 2;
    bool isFirstStatePressed = (PIND >> buttonPin) & 1;
    static const uint8_t debounceDelay = 10;
    _delay_ms(debounceDelay);
    bool isSecondStatePressed = (PIND >> buttonPin) & 1;

    // Check if the button is bouncing
    if (isFirstStatePressed != isSecondStatePressed)
    {
        return ButtonState::Bounced;
    }

    if (isFirstStatePressed == true)
    {
        return ButtonState::Pressed;
    }

    return ButtonState::Released;
}

/// Reset and set timer and setup interrupt for when the duration is reached
void startTimer(uint16_t duration)
{
    cli(); // Clear global interrupt flag to disable interrupts
    
    isTimerExpired = false;

    // Reset counter
    TCNT1 = 0;
    
    // Set output compare register to the desired duration
    OCR1A = duration;
    
    // Timer counter control register flags (3x8 bits)
    TCCR1A = 0;
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set clock select to clock divided by 1024 by setting CS1 bits to 101
    TCCR1B |= (1 << WGM12); // Set CTC mode with OCR1A as TOP source by setting WGM1 bits to 0100 (two LSBs on TCCR1A and two MSBs on TCCR1B)
    TCCR1C = 0; // Not used in this particular case
    
    // Enable interrupt when TCNT1 = OCR1A
    TIMSK1 |= (1 << OCIE1A);
    
    sei(); // Set global interrupt flag to enable interrupts
}

/// Interrupt service routine for TCNT1 match with OCR1A
ISR(TIMER1_COMPA_vect)
{
    isTimerExpired = true;
}

/// Interrupt service routine for INT0 button
ISR(INT0_vect)
{
    isButtonPressed = (getInterruptButtonState() == ButtonState::Pressed);
    
    // Clear interrupt flag for INT0 to prevent bouncing
    EIFR |= (1 << INTF0);
}

int main()
{
    initialize();

    // Wait for 10 seconds
    const uint16_t msBeforeStart = 10 * 1000; // 10 seconds converted to milliseconds
    _delay_ms(msBeforeStart);
    
    // Turn LED on for 0.1 second and start timer
    const uint8_t ledRed = 0b01;
    const uint8_t ledGreen = 0b10;
    PORTA |= ledRed;
    _delay_ms(100);
    PORTA = 0;
    
    // Start 1 second timer
    const uint16_t msTimerMultiplier = F_CPU / 1024;
    startTimer(msTimerMultiplier * 1000); // Wait for 1000 ms (1 second)

    // Wait for one of the two variables two change (either the timer ran out or the button was pressed)
    while (isTimerExpired == false && isButtonPressed == false) 
    {
        // Do nothing
    }

    // Disable interrupts to process button state    
    cli();
    if (isButtonPressed == true)
    {
        PORTA |= ledGreen;
    }
    else
    {
        PORTA |= ledRed;
    }
}