/// Programme qui change la couleur de la DEL en fonction du nombre d'appuis et de relâchements du bouton interrupt, avec des interruptions

// La broche A1 se connecte à la broche positive de la DEL
// et la broche A2 se connecte à la broche négative de la DEL

// TABLEAU D'ÉTATS
// État présent | Bouton interrupt | État suivant | Sortie LED
// Red1         | 0                | Red1         | Rouge
// Red1         | 1                | Amber        | Rouge
// Amber        | 0                | Green1       | Ambre
// Amber        | 1                | Amber        | Ambre
// Green1       | 0                | Green1       | Verte
// Green1       | 1                | Red2         | Verte
// Red2         | 0                | Off          | Rouge
// Red2         | 1                | Red2         | Rouge
// Off          | 0                | Off          | Éteinte
// Off          | 1                | Green2       | Éteinte
// Green2       | 0                | Red1         | Verte
// Green2       | 1                | Green2       | Verte

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 8'000'000
#include <util/delay.h>

// Constants
namespace
{
    const uint16_t msDelayLoopMultiplier = 2000; // 8'000'000 / 1000 / 4 = 2000 : coefficient for generating a 1 ms delay for _delay_loop_2
}

/// FSM states
enum class State : uint8_t
{
    Red1,
    Amber,
    Green1,
    Red2,
    Off,
    Green2,
    Count
};

/// Button states
enum class ButtonState : uint8_t
{
    Bounced,
    Pressed,
    Released
};

// Globals
volatile State currentState;

/// Initialize initial program state
void initialize()
{
    cli();

    // Setup pins
    DDRD = 0;    // Input mode for interrupt button on D2
    DDRA = 0b11; // Output mode for LED on A0 and A1

    // Initialize globals
    currentState = State::Red1;

    // Enable external interrupts
    EIMSK |= (1 << INT0);

    // INT0 interrupt generation on any edge
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);

    sei();
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

// Interrupt service routine for INT0 button
ISR(INT0_vect)
{
    // Verify that the button is not bouncing
    if (getInterruptButtonState() != ButtonState::Bounced)
    {
        // Increment state
        if (static_cast<uint8_t>(currentState) < static_cast<uint8_t>(State::Count) - 1)
        {
            currentState = static_cast<State>(static_cast<uint8_t>(currentState) + 1);
        }
        else
        {
            currentState = State::Red1;
        }
    }

    // Clear interrupt flag for INT0 to prevent bouncing
    EIFR |= (1 << INTF0);
}

int main()
{
    initialize();

    const uint8_t ledMask = 0b11;
    const uint8_t ledRed = 0b01;
    const uint8_t ledGreen = 0b10;

    while (true)
    {
        switch (currentState)
        {
        case State::Red1:
            PORTA = ledRed;
            break;
        case State::Amber:
            PORTA ^= ledMask; // Toggle two leastmost significant bits
            break;
        case State::Green1:
            PORTA = ledGreen;
            break;
        case State::Red2:
            PORTA = ledRed;
            break;
        case State::Off:
            PORTA = 0;
            break;
        case State::Green2:
            PORTA = ledGreen;
            break;
        default:
            break;
        }
    }
}