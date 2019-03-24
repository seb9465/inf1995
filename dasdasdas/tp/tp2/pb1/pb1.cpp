/// Programme qui allume une DEL durant une seconde chaque fois que le bouton interrupt est appuyé 5 fois
/// \author Misha Krieger-Raynauld (1952515) et Simon Gauvin (1951457)
/// \file probleme1.cpp (TP2)
/// \date 2019-01-18
/// Créé le 2019-01-16

// La broche A1 se connecte à la broche positive de la DEL,
// et la broche A2 se connecte à la broche négative de la DEL

// TABLEAU D'ÉTATS
// État présent |   Bouton interrupt    |   État suivant    |   Sortie LED
// LedOff0      |   0                   |   LedOff0         |   Éteinte
// LedOff0      |   1                   |   LedOff1         |   Éteinte
// LedOff1      |   0                   |   LedOff2         |   Éteinte
// LedOff1      |   1                   |   LedOff1         |   Éteinte
// LedOff2      |   0                   |   LedOff2         |   Éteinte
// LedOff2      |   1                   |   LedOff3         |   Éteinte
// LedOff3      |   0                   |   LedOff4         |   Éteinte
// LedOff3      |   1                   |   LedOff3         |   Éteinte
// LedOff4      |   0                   |   LedOff4         |   Éteinte
// LedOff4      |   1                   |   LedOff5         |   Éteinte
// LedOff5      |   0                   |   LedOff6         |   Éteinte
// LedOff5      |   1                   |   LedOff5         |   Éteinte
// LedOff6      |   0                   |   LedOff6         |   Éteinte
// LedOff6      |   1                   |   LedOff7         |   Éteinte
// LedOff7      |   0                   |   LedOff8         |   Éteinte
// LedOff7      |   1                   |   LedOff7         |   Éteinte
// LedOff8      |   0                   |   LedOff8         |   Éteinte
// LedOff8      |   1                   |   LedOff9         |   Éteinte
// LedOff9      |   0                   |   LedOn           |   Éteinte
// LedOff9      |   1                   |   LedOff9         |   Éteinte
// LedOn        |   X                   |   LedOff0         |   Rouge

#include <avr/io.h>

#define F_CPU 8000000
#include <util/delay.h>


/// Enum to contain the different states of the FSM
enum class State : uint8_t
{
    LedOff,
    LedOn
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
    static const int buttonPin = 2;
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

    State currentState = State::LedOff;

    static const uint16_t ledTimeOn = 1000;

    uint8_t pressCounter = 0;
    static const uint8_t maxPressReleases = 10;

    const uint8_t ledRed = 0b01;
    
    while (true)
    {
        ButtonState buttonState;
        switch (currentState)
        {
        case State::LedOff:
            PORTA = 0; // Turn led off

            buttonState = getInterruptButtonState();
            // If waiting for a button press (even counter) and button was pressed OR
            // if waiting for a button release (odd counter) and button was released
            if ((pressCounter % 2 == 0 && buttonState == ButtonState::Pressed) ||
                (pressCounter % 2 != 0 && buttonState == ButtonState::Released))
            {
                ++pressCounter;
            }

            // If the button was pressed and released the right number of times,
            // change state to ledOn
            if (pressCounter == maxPressReleases)
            {
                currentState = State::LedOn;
            }
            break;
        case State::LedOn:
            PORTA = ledRed; // Make led red
            _delay_ms(ledTimeOn); // Sleep for 1 second

            currentState = State::LedOff; // Change state to ledOff
            pressCounter = 0;
            break;
        }
    }
}