/// Programme qui effectue une conversion analogique-numérique pour déterminer la luminosité détectée par une photorésistance
/// \file pb2.cpp (TP6)
/// \author Misha Krieger-Raynauld (1952515) et Simon Gauvin (1951457)
/// \date 2019-02-15
/// Créé le 2019-02-15

// Connexions
//
// Photorésistance:
// La broche A1 (port A0) se connecte au fil de données (le fil bleu du câble brun-bleu)
// Connecter le fil rouge à l'alimentation (Vcc) et les deux fils noirs au ground (GND)
//
// LED:
// La broche B1 (port B0) se connecte à la broche positive de la LED
// et la broche B2 (port B1) se connecte à la broche négative de la LED

#include <avr/io.h>
#define F_CPU 8'000'000
#include <util/delay.h>
#include "can.h"

/// Initialize initial program state
void initialize()
{
    // Setup pins
    DDRA = 0; // Input mode for photoresistor on A0
    DDRB = 0b11; // Output mode for LED on B0 and B1
}

int main()
{
    initialize();
    
    can can;
    
    constexpr uint8_t ledMask = 0b11;
    constexpr uint8_t ledRed = 0b01;
    constexpr uint8_t ledGreen = 0b10;
    
    constexpr uint8_t minAmbientLightLevel = 190;
    constexpr uint8_t minBrightLightLevel = 240;
    
    PORTB = ledGreen; // Default value to enable proper bit toggling in ambient mode
    while (true)
    {
        uint8_t lightValue = can.lecture(PA0) >> 2;

        if (lightValue > minBrightLightLevel) // Bright mode
        {
            PORTB = ledRed;
        }
        else if (lightValue > minAmbientLightLevel) // Ambient mode
        {
            PORTB ^= ledMask; // Flip bits to switch color
        }
        else // Dark mode
        {
            PORTB = ledGreen;
        }
    }
}