#include "GestionnaireMoteurs.h"

/***************************************************************************
 * Fonction              : GestionnaireMoteurs()
 * Description           : Constructeur par defaut.  Initialisation des deux
 *                         moteurs (gauche/droite).
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
GestionnaireMoteurs::GestionnaireMoteurs(){
    DDRD |= 0xFF;                       //Initialisation du port D en sortie.
    PORTD = 0x00;                       //Toutes les broches seront initialisees a 0.
    moteurGauche_ = Moteur(GAUCHE);     //Creation du moteur gauche.
    moteurDroite_ = Moteur(DROITE);     //Creation du moteur droite.
}

/***************************************************************************
 * Fonction              : ~GestionnaireMoteurs()
 * Description           : Destructeur de la classe.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
GestionnaireMoteurs::~GestionnaireMoteurs() {}

void GestionnaireMoteurs::avancer(uint8_t vitesseGauche, uint8_t vitesseDroite) {
    if(moteurDroite_.getDirection() == AVANT) {
        moteurDroite_.ajusterVitesse(vitesseDroite);
    } else {
        moteurDroite_.setDirection(AVANT);
        moteurDroite_.demarrerMoteur(vitesseDroite);
    }

    if(moteurGauche_.getDirection() == AVANT) {
        moteurGauche_.ajusterVitesse(vitesseGauche);
    } else {
        moteurGauche_.setDirection(AVANT);
        moteurGauche_.demarrerMoteur(vitesseGauche);
    }
    
}

/***************************************************************************
 * Fonction              : reculer
 * Description           : Cette fonction regle la direction du moteur droite
 *                         et du moteur gauche afin de permettre au robot de
 *                         reculer. La fonction demande aussi a chaque moteur
 *                         de demarrer.
 * Parametres d'entree   : 
 * 		- valeurMax (uint8_t) :
 *                         Valeur dictant la force des moteurs.  Cette valeur
 *                         doit etre entre 0 et 255.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
void GestionnaireMoteurs::reculer(uint8_t vitesseGauche, uint8_t vitesseDroite) {
    if(moteurDroite_.getDirection() == ARRIERE) {
        moteurDroite_.ajusterVitesse(vitesseDroite);
    } else {
        moteurDroite_.setDirection(ARRIERE);
        moteurDroite_.demarrerMoteur(vitesseDroite);
    }

    if(moteurGauche_.getDirection() == ARRIERE) {
        moteurGauche_.ajusterVitesse(vitesseGauche);
    } else {
        moteurGauche_.setDirection(ARRIERE);
        moteurGauche_.demarrerMoteur(vitesseGauche);
    }
}

/***************************************************************************
 * Fonction              : tournerDroite
 * Description           : Cette fonction regle la direction du moteur droite
 *                         et du moteur gauche afin de permettre au robot de
 *                         tourner a droite de 90 degre. La fonction demande
 *                         a chaque moteur de demarrer pour un temps fixe, et
 *                         les moteurs seront ensuite arretes.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
void GestionnaireMoteurs::tournerDroite() {
    moteurDroite_.setDirection(ARRIERE);
    moteurGauche_.setDirection(AVANT);

    uint8_t uneValeur = 100;
    
    moteurDroite_.demarrerMoteur(uneValeur);
    moteurGauche_.demarrerMoteur(uneValeur);
    
    delai_ms(1600);             //Permet environ 90 degres.
    
    arreter();
}

/***************************************************************************
 * Fonction              : tournerGauche
 * Description           : Cette fonction regle la direction du moteur droite
 *                         et du moteur gauche afin de permettre au robot de
 *                         tourner a gauche de 90 degre. La fonction demande
 *                         a chaque moteur de demarrer pour un temps fixe, et
 *                         les moteurs seront ensuite arretes.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
void GestionnaireMoteurs::tournerGauche(){
    moteurDroite_.setDirection(AVANT);
    moteurGauche_.setDirection(ARRIERE);

    uint8_t uneValeur = 100;
    moteurDroite_.demarrerMoteur(uneValeur);
    moteurGauche_.demarrerMoteur(uneValeur);

    delai_ms(1400);             //Permet environ 90 degres.

    arreter();
}

/***************************************************************************
 * Fonction              : arreter
 * Description           : Cette fonction arrete les deux moteurs du robot.
 * Parametres d'entree   : 
 * 		  Aucun.
 * Parametres de sortie  :     
 * 		  Aucun.
 ***************************************************************************/
void GestionnaireMoteurs::arreter() {
    moteurDroite_.arreterMoteur();
    moteurGauche_.arreterMoteur();
}

void GestionnaireMoteurs::ajusterVitesseDroite(uint8_t vitesse) {
    moteurDroite_.ajusterVitesse(vitesse);

}

void GestionnaireMoteurs::ajusterVitesseGauche(uint8_t vitesse) {
    moteurGauche_.ajusterVitesse(vitesse);

}