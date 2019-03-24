/// Atténuation d'une DEl avec l'appui d'un bouton

// La broche A1 se connecte à la broche positive de la DEL,
// et la broche A2 se connecte à la broche négative de la DEL

#include <avr/io.h>
#include <math.h>

#define F_CPU 8'000'000
#include <util/delay_basic.h>

/// Initialize port modes (input/output)
void setupPins()
{
    DDRA = 0b11; // Output mode for LED on A0 and A1
}

int main()
{
    setupPins();
    
    const double usCycleDuration = 1.0 / F_CPU * 1'000'000; // Duration of one clock cycle in microseconds
    const uint8_t cyclesPerDelayLoop = 4; // Number of CPU cycles done 'count' times in the _delay_loop_2 function
    const double usPerDelayLoop = usCycleDuration * cyclesPerDelayLoop;
    
    const uint16_t msFadeDuration = 3000; // Duration of the fade in milliseconds
    const uint16_t b = 1000; // Duration of one period in microseconds
    
    while (true)
    {
        for (uint8_t i = 0; i < 2; i++)
        {
            uint8_t ledColor = (i == 0 ? 0b01 : 0b10);
            for (uint16_t j = msFadeDuration - 1; j > 0; j--) // (msFadeDuration - 1) so that (b - a) never reaches 0, causing a delay_loop_2(0)
            {
                // Duration of pulse in microseconds
                double a = static_cast<double>(j) / msFadeDuration * b;

                // Keep LED on for a
                PORTA = ledColor;
                _delay_loop_2(ceil(a / usPerDelayLoop)); // Sleep for pulse duration / delay for each loop
                
                // Keep LED off for b (length of period) - a
                PORTA = 0;
                _delay_loop_2(ceil((b - a) / usPerDelayLoop));

            }
        }
    }
}