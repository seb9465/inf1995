#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initialisation(void){
    cli();
    //DDRA = 0xff;                    //Port A est en mode sortie.
    DDRD = 0xFF;                    //Port D est en mode entrée.
    PORTD = 0x00;
    //EIMSK |= _BV(INT0);
    //EICRA |= 3;
    sei();
}

void ajustementPWM(uint8_t signalPWMB, uint8_t signalPWMA) {
    TCCR1A |= 0xA1;     // Clear on compare match (low level)
    TCCR1B |= 0x02;     // mode CTC du timer 1 avec horloge divisee par 8
    TCCR1C |= 0;
    //TCNT1 = 0;
    OCR1A = signalPWMA;
    OCR1B = signalPWMB;
}




int main() {
    initialisation();
    for(;;) {
        ajustementPWM(64, 191);      //25%
        _delay_ms(2000);
        ajustementPWM(128, 255);     //50%
        _delay_ms(2000);
        ajustementPWM(191, 0);     //75%
        _delay_ms(2000);
        ajustementPWM(255, 64);     //100%
        _delay_ms(2000);
        ajustementPWM(0, 128);       //0%
        _delay_ms(2000);
    }
}


