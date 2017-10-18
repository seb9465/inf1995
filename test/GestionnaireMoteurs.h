#ifndef GESTIONNAIREMOTEURS_H
#define GESTIONNAIREMOTEURS_H

#include "lib/moteur.h"
#include "lib/minuterie.h"

class GestionnaireMoteurs {
public:
    GestionnaireMoteurs();
    ~GestionnaireMoteurs();
    
    void avancer(uint8_t vitesseGauche, uint8_t vitesseDroite);
    void reculer(uint8_t vitesseGauche, uint8_t vitesseDroite);
    void arreter();

    void ajusterVitesseDroite(uint8_t vitesse);
    void ajusterVitesseGauche(uint8_t vitesse);

    void tournerDroite();
    void tournerGauche();
private:
    Moteur moteurDroite_;
    Moteur moteurGauche_;
};

#endif
