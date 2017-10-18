#include "cytron.h"

/*
 *                      SYNOPSIS
 * Le cytron est très simple. Lorsqu'il capte une ligne foncée, la sortie est 1. 
 * Il possède 5 capteurs. Chacun est indépendant. Si un capteur voit quelque une ligne 
 * foncé, la sortie au port sera 1. On le capte au port 1.
 */

/***************************************************************************
 * Fonction              : Cytron()
 * Description           : Constructeur par defaut de la classe Cytron.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
Cytron::Cytron() {
	initialisation();
}

/***************************************************************************
 * Fonction              : Initialisation()
 * Description           : Initialisation de la classe Cytron.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
void Cytron::initialisation() {
	DDRC &= 0x03;  //PORTC<7..2> en entrée
	PCIFR |= (1<<PCIF2);
	PCMSK2 |= (1<<PCINT18) | (1<<PCINT19) | (1<<PCINT20) | (1<<PCINT21) | (1<<PCINT22);
}

void Cytron::debutDetection() {
	PCIFR |= (1<<PCIF2); //enable interrupts
}

void Cytron::arretDetection() {
	PCIFR &= ~(1<<PCIF2); //disable interrupts
}

/***************************************************************************
 * Fonction              : GetPin()
 * Description           : Obtenir la bonne Pin d'entree.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
int Cytron::getPin(Detecteur dect)
{
	switch(dect){
		case GAUCHE2:
			return PC2;
			break;
		case GAUCHE1:
			return PC3;
			break;
		case CENTRE:
			return PC4;
			break;
		case DROITE1:
			return PC5;
			break;
		case DROITE2:
			return PC6;
			break;
		default:
			return 0;
			break;
	}
}

/***************************************************************************
 * Fonction              : Detection()
 * Description           : Bool pour savoir si il y a detection.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
bool Cytron::Detection(Detecteur dect){                    
    if(PIN_CYTRON & (1<<getPin(dect))) {
        return true;
    }
	else {
        return false;
    }
}
