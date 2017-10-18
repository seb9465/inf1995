#include "del.h"

Del::Del(){}			// Constructeur par defaut.

// Constructeur par parametres.
// Les quatres choix de Ports disponibles sont : 
//
//		A, B, C, D 
//	
// Les numeros des Ports disponibles pour chacun des Ports sont:
//
//		1 3 5 7				(disposition)
//		0 2 4 6				(disposition)
//
//		7 6 5 4 3 2 1 0		(numeros des Ports)
//		x x x x x x x x		(binaire)
//
// Il est a noter que le fils pourra juste etre lier a la verticale.
// Pour eviter toute confusion, nous utiliserons le numeroPortUn pour
// la ligne en bas et le numeroPortDeux pour la ligne en haut.
Del::Del(Ports lettrePort, uint8_t numeroPortUn, uint8_t numeroPortDeux){
	
	lettrePort_ = lettrePort;
	numeroPortUn_ = numeroPortUn;
	numeroPortDeux_ = numeroPortDeux;
	switch (lettrePort_){		// Dependamment de de la lettre du Port utilise 
		case A:
				DDRA |= (1 << numeroPortUn_) | (1 << numeroPortDeux_);
				break;
		case B:
				DDRB |= (1 << numeroPortUn_) | (1 << numeroPortDeux_);
				break;
		case C:
				DDRC |= (1 << numeroPortUn_) | (1 << numeroPortDeux_);
				break;
		case D:
				DDRD |= (1 << numeroPortUn_) | (1 << numeroPortDeux_);
				break;
	}
}


Del::~Del(){}				// Destructeur


void Del::afficherVert() {	// Afficher la couleur vert
	
    etat_ = VERTE;			// Decimale = 1, valeur binaire = 0b01

	switch (lettrePort_){	// Dependamment de de la lettre du Port utilise 
		case A:
				PORTA &= ~(1 << numeroPortDeux_);			// Enlever le bit voulu
				PORTA |= (1 << numeroPortUn_);				// Rajouter le bit voulu
				break;
		case B:
				PORTB &= ~(1 << numeroPortDeux_);
				PORTB |= (1 << numeroPortUn_);
				break;
		case C:
				PORTC &= ~(1 << numeroPortDeux_);
				PORTC |= (1 << numeroPortUn_);
				break;
		case D:
				PORTD &= ~(1 << numeroPortDeux_);
				PORTD |= (1 << numeroPortUn_);
				break;
	}
}

// Afficher la couleur rouge
void Del::afficherRouge() {
	
	etat_ = ROUGE;			// Decimale = 2, valeur binaire = 0b10

	switch (lettrePort_){	// Dependamment de de la lettre du Port utilise 
		case A:
				PORTA &= ~(1 << numeroPortUn_);				// Enlever le bit voulu
				PORTA |= (1 << numeroPortDeux_);			// Rajouter le bit voulu
				break;
		case B:
				PORTB &= ~(1 << numeroPortUn_);
				PORTB |= (1 << numeroPortDeux_);
				break;
		case C:
				PORTC &= ~(1 << numeroPortUn_);
				PORTC |= (1 << numeroPortDeux_);
				break;
		case D:
				PORTD &= ~(1 << numeroPortUn_);
				PORTD |= (1 << numeroPortDeux_);
				break;
	}
}

// Afficher la couleur ambre
void Del::afficherAmbre(){
    etat_ = AMBRE;
    afficherRouge();
    _delay_ms(1);
    afficherVert();
    _delay_ms(3);
}

// Afficher aucune couleur
void Del::eteindre() {
	
	etat_ = ETEINTE;		// Decimale = 0, valeur binaire = 0b00

	switch (lettrePort_){	// Dependamment de de la lettre du Port utilise 
		case A:
				PORTA &= ~((1 << numeroPortUn_) | (1 << numeroPortDeux_));		// Enlever les bits voulus
				break;
		case B:
				PORTB &= ~((1 << numeroPortUn_) | (1 << numeroPortDeux_));		// Enlever les bits voulus
				break;
		case C:
				PORTC &= ~((1 << numeroPortUn_) | (1 << numeroPortDeux_));		// Enlever les bits voulus
				break;
		case D:
				PORTD &= ~((1 << numeroPortUn_) | (1 << numeroPortDeux_));		// Enlever les bits voulus
				break;
	}
}

// Methodes d'acces
uint8_t Del::obtenirEtat() {
    return etat_;
}

Ports Del::obtenirLettrePort() {
    return lettrePort_;
}
uint8_t Del::obtenirNumeroPortUn() {
    return numeroPortUn_;
}

uint8_t Del::obtenirNumeroPortDeux() {
    return numeroPortDeux_;
}
