/// Contrôle des moteurs du robot par PWM

// La broche C3 se connecte à la broche E du connecteur JM1 du pont en H,
// et la broche C4 se connecte à la broche D du même connecteur

#include <avr/io.h>
#include <math.h>

#define F_CPU 8'000'000
#include <util/delay_basic.h>

namespace
{
    const double usCycleDuration = 1.0 / F_CPU * 1'000'000; // Duration of one clock cycle in microseconds
    const uint8_t cyclesPerDelayLoop = 4; // Number of CPU cycles done 'count' times in the _delay_loop_2 function
    const double usPerDelayLoop = usCycleDuration * cyclesPerDelayLoop;
}

/// Initialize port modes (input/output)
void setupPins()
{
    DDRC = 0xff; // Output mode for C port
    PORTC = 0; // Set D port on H bridge JM1 connector to 0
}

/// Return the period in microseconds for a given frequency in Hz
uint16_t freqToPeriod(uint16_t frequency)
{
    return 1.0 / frequency * 1'000'000;
}

/// Run the motor with PWM for a set amount of time
/// \param frequence The frequency at which the PWM operates
/// \param timeInSeconds The period in seconds
/// \param intensity From 0 to 1, the proportion of the period where the output is +5v
void runMotor(uint16_t frequence, uint16_t timeInSeconds, double intensity)
{
    uint16_t b = freqToPeriod(frequence);
    uint16_t a = b * intensity;
    uint16_t ticks = frequence * timeInSeconds;

    for (uint16_t i = 0; i < ticks; i++)
    {
        if (a != 0)
        {
            // Enable motor for a
            PORTC = 0b111;
            _delay_loop_2(ceil(a / usPerDelayLoop)); // Sleep for pulse duration / delay for each loop
        }
        
        if (b - a != 0)
        {
            // Disable motor for b (length of period) - a
            PORTC = 0;
            _delay_loop_2(ceil((b - a) / usPerDelayLoop));
        }
    }
}

int main()
{
    setupPins();

    while (true)
    {
        runMotor(60, 2, 0.0);
        runMotor(60, 2, 0.25);
        runMotor(60, 2, 0.50);
        runMotor(60, 2, 0.75);
        runMotor(60, 2, 1.0);
        runMotor(400, 2, 0.0);
        runMotor(400, 2, 0.25);
        runMotor(400, 2, 0.50);
        runMotor(400, 2, 0.75);
        runMotor(400, 2, 1.0);
    }  
}