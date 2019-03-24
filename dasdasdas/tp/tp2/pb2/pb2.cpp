/// Programme qui change la couleur de la DEL en fonction du nombre d'appuis et de relâchements du bouton interrupt
/// \author Misha Krieger-Raynauld (1952515) et Simon Gauvin (1951457)
/// \file probleme2.cpp (TP2)
/// \date 2019-01-18
/// Créé le 2019-01-18

// La broche A1 se connecte à la broche positive de la DEL,
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

#include <avr/io.h>

#define F_CPU 8000000
#include <util/delay.h>

/// Enum to contain the different states of the FSM
enum class State : uint8_t
{
    Red1,
    Amber,
    Green1,
    Red2,
    Off,
    Green2
};

/// Enum to represent the different button states
enum class ButtonState : uint8_t
{
    Bounced,
    Pressed,
    Released
};

/// Initialize port modes (input/output)
void setupPins()
{
    DDRA = 0b11; // Output mode for LED on A0 and A1
    DDRD = 0; // Input mode for interrupt button on D2
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

int main()
{
    setupPins();
    
    State currentState = State::Red1;
    
    const uint8_t ledMask = 0b11;
    const uint8_t ledRed = 0b01;
    const uint8_t ledGreen = 0b10;
    
    while (true)
    {
        ButtonState buttonState = getInterruptButtonState();
        switch (currentState)
        {
        case State::Red1:
            PORTA = ledRed;
            if (buttonState == ButtonState::Pressed)
            {
                currentState = State::Amber;
            }
            break;
        case State::Amber:
            PORTA ^= ledMask; // Toggle two leastmost significant bits
            if (buttonState == ButtonState::Released)
            {
                currentState = State::Green1;
            }
            break;
        case State::Green1:
            PORTA = ledGreen;
            if (buttonState == ButtonState::Pressed)
            {
                currentState = State::Red2;
            }
            break;
        case State::Red2:
            PORTA = ledRed;
            if (buttonState == ButtonState::Released)
            {
                currentState = State::Off;
            }
            break;
        case State::Off:
            PORTA = 0;
            if (buttonState == ButtonState::Pressed)
            {
                currentState = State::Green2;
            }
            break;
        case State::Green2:
            PORTA = ledGreen;
            if (buttonState == ButtonState::Released)
            {
                currentState = State::Red1;
            }
            break;
        }
    }
}