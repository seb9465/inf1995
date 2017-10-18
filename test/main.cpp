#include "GestionnaireInstructions.h"



int main() {
	DDRA= 0xff;
	Cytron cytron;
	GestionnaireMoteurs moteurs_;
	moteurs_.avancer(100, 100);
	for(;;) { 
        if(!(cytron.Detection(GAUCHE2)) && !(cytron.Detection(DROITE2))) { //avancer
            moteurs_.avancer(100, 100);
        }
        else if(cytron.Detection(CENTRE) && cytron.Detection(GAUCHE1) && cytron.Detection(GAUCHE2)) {
        	_delay_ms(750);
            moteurs_.tournerGauche();
            moteurs_.avancer(100, 100);
	    }
        else if((cytron.Detection(GAUCHE2)) && !(cytron.Detection(DROITE2))) {
            moteurs_.avancer(100, 150);
        }
        else if(!(cytron.Detection(GAUCHE2)) && (cytron.Detection(DROITE2))) {
            moteurs_.avancer(150, 100);
        }
        else if(cytron.Detection(CENTRE) && cytron.Detection(GAUCHE2) && cytron.Detection(DROITE2)) {
            //moteurs_.arreter();
        }
	}
}

