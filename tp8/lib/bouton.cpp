#include "bouton.h"

bool etatBoutton(Ports port, uint8_t pin){
    bool etatBtn = false;
	switch(port) {
		case A:
			if(PINA & pin) {                     //Debut portion de code pour l'anti-rebond.
				_delay_ms(10);               //Délai de 10ms.
				if(PINA & pin)         
					etatBtn = true;   
			}                                    //Fin portion de code pour l'anti-rebond.
			break;
		case B:
			if(PINB & pin) {
				_delay_ms(10);
				if(PINB & pin)         
					etatBtn = true;   
			}
			break;
		case C:
			if(PINC & pin) {
				_delay_ms(10);
				if(PINC & pin)         
					etatBtn = true;   
			}
			break;
		case D:
			if(PIND & pin) {
				_delay_ms(10);
				if(PIND & pin)         
					etatBtn = true;   
			}
			break;
	}
			
    return etatBtn;
}
