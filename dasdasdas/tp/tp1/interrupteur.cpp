#include <avr/io.h>

#define F_CPU 8000000
#include <util/delay.h>

int main()
{
    DDRA = 0b11;
    DDRD = 0;

    uint8_t bufferPressed = 0;
    
    while (true)
    {
        _delay_ms(1);
        if (bufferPressed == 0b11111111)
        {
            if ((PORTA & 0b11) == 0) // If LED is closed
            {
                PORTA = 0b01; // GIve it a color
            }
            else
            {
                PORTA = ~PORTA & 0b11; // Flicker
            }
        }
        else
        {
            PORTA = 0;
        }

        // Shift bits to the left
        bufferPressed <<= 1;
        // Check if button is pressed, then put the bit in the least significant position of bufferPressed
        bufferPressed |= ((PIND & 0b100) >> 2);
    }
}