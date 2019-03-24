#include "moteur.h"

Moteur::Moteur() {
    	DDRD |= 0xf0; //Les 4 dernieres pins du port D en sortie
	directionRoueDroite = AVANCER;
	directionRoueGauche = AVANCER;
}

void Moteur::avancer(uint8_t pourcentage) {
    cli();
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);       // Fast PWM 8 bits
    TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);         	// Clear OCnA/OCnB on Compare Match, set OCnA/OCnB at BOTTOM 
                                                            //(non-inverting mode)
    TCCR1C |= 0;						// Prescaler CLK/1024
    TCNT1 = 0;   						// Compteur a 0
	//Calcul du pourcentage
	uint8_t top = (uint8_t)(255*(pourcentage/100));		// Le 255 vient du PWM 8 bits
	if(pourcentage > 100) {
		top = 255;
    	OCR1A = top;
    	OCR1B = top;
    }
	//On met la direction vers l'avant
	changerDirection(AVANCER, ROUE_DROITE);
	changerDirection(AVANCER, ROUE_GAUCHE);
    sei();
}

void Moteur::reculer(uint8_t pourcentage) {
	cli();
    TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);       // Fast PWM 8 bits
    TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);         	// Clear OCnA/OCnB on Compare Match, set OCnA/OCnB at BOTTOM 
								                            //(non-inverting mode)
   	TCCR1C |= 0;						                    // Prescaler CLK/1024
    TCNT1 = 0;              				                // Compteur a 0
	//Calcul du pourcentage
	uint8_t top = (uint8_t)(255*(pourcentage/100));		    // Le 255 vient du PWM 8 bits
	if(pourcentage > 100) {
		top = 255;
    	OCR1A = top;
    	OCR1B = top;
    }
	//On met la direction vers l'arriere
	changerDirection(RECULER, ROUE_DROITE);
	changerDirection(RECULER, ROUE_GAUCHE);
    sei();
}

void Moteur::ajustementPWM(uint8_t signalPWMB, uint8_t signalPWMA) {
    cli();
    TCCR1A |= 0xA1;         // Clear on compare match (low level)
    TCCR1B |= 0x02;         // mode CTC du timer 1 avec horloge divisee par 8
    TCCR1C |= 0;
    TCNT1 = 0;              //Compteur a 0
    OCR1A = signalPWMA;
    OCR1B = signalPWMB;
    sei();
}

void Moteur::changerDirection(uint8_t avancerOuReculer, uint8_t roueDroiteOuGauche) {
	if((roueDroiteOuGauche == ROUE_DROITE) || (roueDroiteOuGauche == ROUE_GAUCHE)) {
		if(avancerOuReculer == 1)
			PORTD |= 0x00 | (1 << roueDroiteOuGauche);
		else if(avancerOuReculer == 0)
			PORTD &= 0xff & (0 << roueDroiteOuGauche);
		//on prend en note les directions
		if(roueDroiteOuGauche == ROUE_DROITE)
			directionRoueDroite = avancerOuReculer;
		else if(roueDroiteOuGauche == ROUE_GAUCHE)
			directionRoueGauche = avancerOuReculer;
	}
}

uint8_t Moteur::getDirectionRoue(uint8_t roueDroiteOuGauche) {
	if(roueDroiteOuGauche == ROUE_DROITE)
		return directionRoueDroite;
	else
		return directionRoueGauche;
}
