/// Lecture et écriture en mémoire

// Connexions
//
// LED:
// La broche A1 se connecte à la broche positive de la LED
// et la broche A2 se connecte à la broche négative de la LED

#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8'000'000
#include <util/delay.h>
#include <string.h>

#include "memoire_24.h"

// Constants
namespace
{
    const uint16_t msDelayLoopMultiplier = 2000; // 8'000'000 / 1000 / 4 = 2000 : coefficient for generating a 1 ms delay for _delay_loop_2
}

/// Initialize initial program state
void initialize()
{
    cli();

    // Setup pins
    DDRA = 0b11; // Output mode for LED on A0 and A1

    sei();
}

int main()
{
    initialize();

    Memoire24CXXX memoryInterface;

    const char* writeData = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    memoryInterface.ecriture(0x00, reinterpret_cast<const uint8_t*>(writeData), strlen(writeData) + 1);

    char readBuffer[strlen(writeData) + 1];
    memoryInterface.lecture(0x00, reinterpret_cast<uint8_t*>(readBuffer), strlen(writeData) + 1);

    const uint8_t ledRed = 0b01;
    const uint8_t ledGreen = 0b10;
    if (strcmp(writeData, readBuffer) == 0)
    {
        PORTA |= ledGreen;
    }
    else
    {
        PORTA |= ledRed;
    }
}