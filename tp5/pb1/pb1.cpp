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
 * Problème 2
 */

/*
 *                    TABLEAU D'ÉTATS
 * 
 * ÉTAT PRÉSENT |   ENTRÉE  |   ÉTAT FUTUR  |   SORTIE 
 * -----------------------------------------------------     
 *     INIT     |     0     |      INIT     |    ROUGE   
 *     INIT     |     1     |       E1      |    ROUGE
 *      E1      |     0     |       E2      |    AMBRÉE
 *      E1      |     1     |       E1      |    AMBRÉE
 *      E2      |     0     |       E2      |    VERTE
 *      E2      |     1     |       E3      |    VERTE  
 *      E3      |     0     |       E4      |    ROUGE
 *      E3      |     1     |       E3      |    ROUGE
 *      E4      |     0     |       E4      |    ÉTEINTE  
 *      E4      |     1     |       E5      |    ÉTEINTE
 *      E5      |     0     |      INIT     |    VERTE     
 *      E5      |     1     |       E5      |    VERTE          
 */

/*
 *                      SYNOPSIS
 * Notre DEL libre est connectée sur les branches 1 et 2 du port A.
 * Notre interrupteur est conmecté au port D.
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ETEINTE 0x00
#define VERTE 0x01
#define ROUGE 0x02

enum State {INIT,E1, E2, E3, E4, E5};

volatile State etat = INIT;  

bool etatBoutton();                 //Prototype dpour la fonction etatBoutton.

void afficherAmbre();               //Prototype dpour la fonction afficherAmbre.

ISR(INT0_vect) {
        _delay_ms(30);                 //Delai avant de confirmer la reponse du bouton-poussoir.
        if(etatBoutton()) {
            switch(etat) {                      //Changement d'etats
                case INIT :
                    etat = E1;
                    break;
                case E1 :
                    etat = E2;
                    break;
                case E2 :
                    etat = E3;
                    break;
                case E3 :
                    etat = E4;
                    break;
                case E4 :
                    etat = E5;
                    break;
                case E5 :
                    etat = INIT;
                    break;
            }
            EIFR |= (1 << INTF0);
        }
}

void initialisation(void) {
    cli();
    DDRA = 0xff;                    //Port A est en mode sortie.
    DDRD = 0x00;                    //Port D est en mode sortie.
    EIMSK |= _BV(INT0);
    EICRA |= 3;
    sei();
}

int main() {
    initialisation();
    
    for(;;) {
         switch(etat) {
            case INIT :
                 PORTA = ROUGE;
                break;
            case E1 :
                afficherAmbre();
                break;
            case E2 :
                PORTA = VERTE;
                break;
            case E3 :
                PORTA = ROUGE;
                break;
            case E4 :
                PORTA = ETEINTE;
                break;
            case E5 :
                PORTA = VERTE;
                break;
         }     
    }
}

/*
 * Fonction afficherAmbre.
 * Permet d'afficher la couleur ambre sur la DEL.
 * 
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void afficherAmbre() {
    PORTA = ROUGE;
    _delay_ms(1);
    PORTA = VERTE;
    _delay_ms(3);
}

/*
 * Fonction etatBoutton.
 * 
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - etatBtn (bool) : Renvoie l'etat du boutton.  Si le boutton est pressé,
 *        etatBtn sera a TRUE.  Si le boutton n'est pas pressé, etatBtn sera à
 *        FALSE.
 */
bool etatBoutton() {
    bool etatBtn = false;
    if(PIND & 0x04) {           //Début portion de code pour l'anti-rebond.
        _delay_ms(10);          //Délai de 10ms .
        if(PIND & 0x04)         
            etatBtn = true;   
    }                           //Fin portion de code pour l'anti-rebond.   
    return etatBtn;
}
