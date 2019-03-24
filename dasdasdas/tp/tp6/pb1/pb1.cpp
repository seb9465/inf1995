/// Programme qui incrémente un compteur en fonction d'un bouton pesé pour finir par faire clignoter une DEL
/// \file pb1.cpp (TP6)
/// \author Misha Krieger-Raynauld (1952515) et Simon Gauvin (1951457)
/// \date 2019-02-15
/// Créé le 2019-02-13

// Connexions
//
// Bouton-poussoir blanc:
// La broche D3 (port D2) se connecte au fil de données (le fil bleu du câble brun-bleu)
// Connecter le fil rouge à l'alimentation (Vcc) et les deux fils noirs au ground (GND)
// Retirer le cavalier IntEN
//
// LED:
// La broche B1 (port B0) se connecte à la broche positive de la LED
// et la broche B2 (port B1) se connecte à la broche négative de la LED

// +------------------------------------------------------ Tableau d'états -----------------------------------------------------+
// | État présent           | Bouton                 | Minuterie expirée      | État suivant           | Sortie DEL             |
// +------------------------+------------------------+------------------------+------------------------+------------------------+
// | Initial                | 0                      | X                      | Initial                | Éteinte                |
// | Initial                | 1                      | X                      | Counting               | Éteinte                |
// | Counting               | 0                      | X                      | GreenFlashing          | Éteinte                |
// | Counting               | X                      | 1                      | GreenFlashing          | Éteinte                |
// | Counting               | 1                      | 0                      | Counting               | Éteinte                |
// | GreenFlashing          | X                      | 1                      | Waiting                | Verte (clignotante)    |
// | Waiting                | X                      | 1                      | RedFlashing            | Éteinte                |
// | RedFlashing            | X                      | 1                      | GreenSolid             | Rouge (clignotante)    |
// | GreenSolid             | X                      | 1                      | Initial                | Verte                  |
// +------------------------+------------------------+------------------------+------------------------+------------------------+

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8'000'000
#include <util/delay.h>

// Constants
namespace
{
    constexpr uint16_t msDelayLoop2Multiplier =  F_CPU / 1000 / 4; // Coefficient (2000) for generating a 1 ms delay with _delay_loop_2
    constexpr uint16_t secTimerMultiplier = F_CPU / 1024; // Coefficient (7813) for generating a 1 second duration with startTimer()
}

/// FSM - see state table above
enum class State : uint8_t
{
    Initial,
    Counting,
    GreenFlashing,
    Waiting,
    RedFlashing,
    GreenSolid
};

// Global variables
volatile bool isButtonPressed;
volatile bool isTimerExpired;

/// Get the current state of the interrupt button with a software-controlled debouncer
/// \return Whether the button is pressed
bool getButtonPressed()
{
    bool isFirstStatePressed;
    bool isSecondStatePressed;
    // Loop until valid button state is polled
    do
    {
        // Check button state twice to debounce
        static constexpr uint8_t buttonMask = 0b100;
        isFirstStatePressed = !(PIND & buttonMask);
        
        static constexpr uint8_t msDebounceDelay = 10;
        _delay_ms(msDebounceDelay);
        
        isSecondStatePressed = !(PIND & buttonMask);
    } while (isFirstStatePressed != isSecondStatePressed);

    return isFirstStatePressed;
}

/// Initialize initial program state
void initialize()
{
    cli(); // Clear global interrupt flag to disable interrupts

    // Setup pins
    DDRD = 0; // Input mode for interrupt button on D2
    DDRB = 0b11; // Output mode for LED on B0 and B1

    // Explicitly initialize globals
    isButtonPressed = getButtonPressed();
    isTimerExpired = false;

    // Enable external interrupts on INT0
    EIMSK |= (1 << INT0);

    // INT0 interrupt sense control bits
    EICRA |= (1 << ISC00); // Set interrupt generation on any edge by setting ISC0 bits to 01

    sei(); // Set global interrupt flag to enable interrupts
}

/// Reset and set timer and setup interrupt for when the duration is reached
/// \param duration Duration of the timer in clock cycles multiplied by prescaler (1024)
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

/// Interrupt service routine for INT0 button interrupt on any edge
ISR(INT0_vect)
{   
    isButtonPressed = getButtonPressed();
        
    // Clear interrupt flag for INT0 to prevent bouncing
    EIFR |= (1 << INTF0);
}

int main()
{
    initialize();
    
    constexpr uint8_t ledMask = 0b11;
    constexpr uint8_t ledRed = 0b01;
    constexpr uint8_t ledGreen = 0b10;
    
    State currentState = State::Initial;

    uint8_t counter = 0;
    while (true)
    {
        switch (currentState)
        {
        case State::Initial:
            if (isButtonPressed == true)
            {
                currentState = State::Counting;
            }
            break;
        case State::Counting:
        {
            static constexpr uint8_t maxCounter = 120;
            if (counter < maxCounter && isButtonPressed == true) // Count while counter has not reached 120 and button is still held
            {
                counter++;
                static constexpr uint8_t msDelayCounting = 100;
                _delay_ms(msDelayCounting); // Sleep for 0.1 seconds (increment 10 times per second)
            }
            else // Change state
            {
                currentState = State::GreenFlashing;
                static constexpr uint16_t greenFlashingTimerDuration = secTimerMultiplier / 2;
                startTimer(greenFlashingTimerDuration); // Start timer for 0.5 seconds
            }
            break;
        }
        case State::GreenFlashing:
            if (isTimerExpired == false) // Make LED flash green
            {
                PORTB ^= ledGreen; // Toggle bits for making LED green
                static constexpr uint16_t msDelayGreenFlashing = 50;
                _delay_ms(msDelayGreenFlashing); // Sleep for 50 milliseconds
            }
            else // Change state
            {
                currentState = State::Waiting;
                PORTB &= ~ledMask; // Turn off LED
                static constexpr uint16_t waitingTimerDuration = 2 * secTimerMultiplier;
                startTimer(waitingTimerDuration); // Start timer for 2 seconds
            }
            break;
        case State::Waiting:
            if (isTimerExpired == true) // Change state
            {
                currentState = State::RedFlashing;
            }
            break;
        case State::RedFlashing:
        {
            static constexpr uint8_t counterDivider = 2;
            if (counter >= counterDivider) // Make LED flash red "counter / 2" times
            {
                static constexpr uint8_t msDelayRedFlashing = 250;
                PORTB |= ledRed; // Toggle bits for making LED red
                _delay_ms(msDelayRedFlashing); // Sleep for 250 milliseconds to flash twice per second
                PORTB &= ~ledRed; // Turn LED off
                _delay_ms(msDelayRedFlashing); // Sleep for 250 milliseconds to flash twice per second
                counter -= counterDivider;
            }
            else // Change state
            {
                currentState = State::GreenSolid;
                counter = 0; // Reset counter to 0 for next state cycle
            }
            break;
        }
        case State::GreenSolid:
            PORTB |= ledGreen;
            static constexpr uint16_t msDelayGreenSolid = 1000;
            _delay_ms(msDelayGreenSolid); // Sleep for one second
            currentState = State::Initial; // Loop back to initial state
            PORTB &= ~ledGreen;
            break;
        }
    }
}