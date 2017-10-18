#include "minuterie.h"

Minuterie::Minuterie() {
	minuterieExpiree_ = false;
}

void Minuterie::partirMinuterie(uint16_t duree) {
    	minuterieExpiree_ = false;

    	TCCR1A = 0x80;  // Clear on compare match (low level)
    	TCCR1B = 5;     // mode CTC du timer 1 avec horloge divisee par 1024
    	TCCR1C = 0;     // 
    	TCNT1 = 0;      // Reset timer
    	OCR1A = duree;  // Set top value
    	TIMSK1 |= 0x02;    // Enable interrupt
}

void Minuterie::arreterMinuterie() {
	TIMSK1 &= 0xfd;    // Disable interrupt
}

bool Minuterie::getMinuterieExpiree() {
	return minuterieExpiree_;
}

void Minuterie::setMinuterieExpiree(bool expiree) {
	minuterieExpiree_ = expiree;
}

void delai_us(uint16_t nombreUs) {
    	_delay_loop_2(2 * nombreUs); //car 2 x 4 cycles a 8 MHz donne 1 us
}
