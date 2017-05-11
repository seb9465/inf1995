/*
 * Équipe no71
 * 
 * WILLIAM LABERGE      1852751
 * COLIN STÉPHENNE      1852773
 * SÉBASTIEN CADORETTE  1734603
 * 
 * 26 janvier 2017
 * 
 * TRAVAIL PRATIQUE 3 - INF1995
 * Problème 1
 */

/*
 *                    TABLEAU D'ÉTATS
 * 
 * ÉTAT PRÉSENT |   ENTRÉE  |   ÉTAT FUTUR  |   SORTIE 
 * -----------------------------------------------------     
 *     INIT     |     0     |      INIT     |    ÉTEINTE   
 *     INIT     |     1     |       E1      |    ÉTEINTE
 *      E1      |     0     |       E1      |    ÉTEINTE
 *      E1      |     1     |       E2      |    ÉTEINTE
 *      E2      |     0     |       E2      |    ÉTEINTE
 *      E2      |     1     |       E3      |    ÉTEINTE
 *      E3      |     0     |       E3      |    ÉTEINTE
 *      E3      |     1     |       E4      |    ÉTEINTE
 *      E4      |     0     |       E4      |    ÉTEINTE  
 *      E4      |     1     |       E5      |    ÉTEINTE
 *      E5      |     0     |       E5      |    ROUGE
 *      E5      |     1     |      INIT     |    ROUGE
 */

/*
 *                      SYNOPSIS
 * Notre DEL libre est connectée sur les branches 1 et 2 du port A.
 * Notre interrupteur est conmecté au port D.
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define ETEINTE 0x00
#define ROUGE   0x02

enum State {INIT,E1, E2, E3, E4, E5};

int main() {
    DDRA = 0xff;                            //Port A est en mode sortie.
    DDRD = 0x00;                            //Port D est en mode entrée.
    
    State etatPresent = INIT;               //Initialisation des états.
    State etatSuivant = INIT;
        
    bool btnPresse = false;                 //Initialisation des deux variables nous permettant de
    bool dejaUtilise = false;               //gérer l'état du boutton sur le robot.
    
    
    for(;;) {                               //Boucle infinie.
        btnPresse = false;                  //Réinitialisation de la variable btnPressé.
                                    
        if(PIND & 0x04) {                   //Début portion de code pour l'anti-rebond.
            _delay_ms(10);                  //Délai de 10ms .
            if(PIND & 0x04)                 //Vrai si le boutton est pressé.
                btnPresse = true;   
        } else 
            dejaUtilise = false;            //Fin portion de code pour l'anti-rebond.
        
        if(!dejaUtilise) {                  //Empeche d'entrer dans le SWITCH s'il n'est pas relaché.
            switch(etatPresent) {
                case INIT   :
                    PORTA = ETEINTE;
                    if(btnPresse)           //Si le boutton est pressé, on change d'état.
                        etatSuivant = E1;
                    break;
                case E1     :
                    if(btnPresse)           //Si le boutton est pressé, on change d'état.
                        etatSuivant = E2;
                    break;
                case E2     :
                    if(btnPresse)           //Si le boutton est pressé, on change d'état.
                        etatSuivant = E3;
                    break;
                case E3     :
                    if(btnPresse)           //Si le boutton est pressé, on change d'état.
                        etatSuivant = E4;
                    break;
                case E4     :
                    if(btnPresse)           //Si le boutton est pressé, on change d'état.
                        etatSuivant = E5;
                    break;
                case E5     :
                    PORTA = ROUGE;
                    _delay_ms(1000);        //La DEL sera allumée pour une seconde.
                    etatSuivant = INIT;
                    break;
            }
            
            if(etatSuivant != etatPresent){
                dejaUtilise = true;
                etatPresent = etatSuivant;
            }
        }
    }
}

