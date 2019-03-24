#ifndef DEF_MOTEUR
#define DEF_MOTEUR

#include "constantes.h"

class Moteur {
public:
    	Moteur();
    	void avancer(uint8_t pourcentage);
    	void reculer(uint8_t pourcentage);
    	void ajustementPWM(uint8_t signalPWMB, uint8_t signalPWMA);
    	void changerDirection(uint8_t avancerOuReculer, uint8_t roueDroiteOuGauche);
    	uint8_t getDirectionRoue(uint8_t roueDroiteOuGauche);
	
private:
    	uint8_t directionRoueDroite;
	    uint8_t directionRoueGauche;
};

#endif
