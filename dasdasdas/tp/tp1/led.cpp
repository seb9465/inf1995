#include <avr/io.h>

#define F_CPU 8000000
#include <util/delay.h>

int main()
{
    const int delay = 3000;
    DDRA = 0b11;

    while (true)
    {
        PORTA = 0b01;
        _delay_ms(delay);

        PORTA = 0b10;
        _delay_ms(delay);             
    
        for (int i = 0; i < delay; i++)
        {
            PORTA = ~PORTA & 0b11;
            _delay_ms(1);
        }
    }
}