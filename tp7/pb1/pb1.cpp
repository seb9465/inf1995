/*
 * Équipe no71
 * 
 * WILLIAM LABERGE      1852751
 * COLIN STÉPHENNE      1852773
 * SÉBASTIEN CADORETTE  1734603
 * 
 * 28 février 2017
 * 
 * TRAVAIL PRATIQUE 7 - INF1995
 * Problème 1
 */

/*
 *                          SYNOPSIS
 * 1. Le robot est en stand-by.  Il attend que l'utilisateur enfonce le boutton.
 * 2. Quand le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé. 
 * 3. Quand le bouton est relâché ou lorsque le compteur atteint 120, la lumière clignote verte pendant 0,5 seconde.
 * 4. Ensuite, la carte mère ne fait rien. 
 * 5. Puis, deux secondes plus tard, la lumière rouge s'allume. Elle devra clignoter (compteur / 2) fois
 *    au rythme de 2 fois par seconde. 
 * 6. Ensuite, la lumière tourne au vert pendant une seconde. 
 * 7. Finalement, elle s'éteint et le robot revient à son état original.
 * 
 */

/*
 *        DISPOSITION DES FILS
 * Fils bleu et noir    :   PORTD
 *      Bleu   :            Broche 3
 *      Noir   :            Broche 4
 * 
 * Fils rouge et noir   :   PORTD
 *      Rouge  :            Broche Vcc
 *      Noir   :            Broche Grnd
 * 
 * Fils jaune et orange :   PORTA       -   DEL
 *      Jaune  :            Broche 2    -   Gauche
 *      Orange :            Broche 1    -   Droite
 * 
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ETEINTE 0x00
#define VERTE   0x01
#define ROUGE   0x02

enum Etat{INIT, CPT, CLIGN_VERT, CLIGN_ROUGE, LUM_VERTE};

//Déclaration des variables volatiles (variables globales modifiées dans les interruption).
volatile uint8_t compteur;
volatile Etat etat = INIT;

//Prototypes de nos fonctions.
void initialisation();
void demarrerCompteur();
void arreterCompteur();
bool etatBoutton();
void clignotementRouge(uint8_t cpt);

/*
 * Il y aura une interruption lorsque le timer sera rendu a la même
 * valeur que OCR1A.  A ce moment, notre compteur sera incremente.
 * Se référer a la page 61 de la documentation Atmel pour plus d'info.
 *
 * - TIMER_COMPA_vect : Timer/Counter1 Compare Match A
 */
ISR(TIMER1_COMPA_vect){ 
    compteur++;
}

/*
 * Il y aura une interruption lorsqu'un changement sera detecte sur les PINs.
 * Par changement, on entend ici un changement dans l'etat du boutton poussoir.
 * 1. Si le boutton est enfonce et que nous sommes dans l'etat intial (INIT), il
 *    y aura demarrage du compteuret nous changerons pour le prochain etat.
 * 2. Sil le boutton est relache et que nous somme dans l'etat compteur (CPT), il
 *    y aura l'arret du compteur et nous changerons pour le prochain etat.
 *
 * - PCINT3_vect : Pin Change Interrupt Request 3
 */
ISR(PCINT3_vect) {
    _delay_ms(30);                              //Delai avant de confirmer la reponse du boutton poussoir.
   
    if(etat == INIT) {
        if(etatBoutton()) {                     //Verifie si le boutton est presse.
            demarrerCompteur();                 //Demarrage du fonctionnement du timer.
            etat = CPT;                         //Changement pour l'etat suivant.
        }
    } else if (etat == CPT) {
        if(!etatBoutton()){                     //Verifie si le boutton est relache.
            arreterCompteur();                  //Arrete du timer.
            etat = CLIGN_VERT;                  //Changement pour l'etat suivant.
        }
    }
}

int main() {
    initialisation();
    
    for(;;){                                    //Boucle infinie.
        switch(etat) {
            case INIT:                          //Ici, le changement pour le prochain etat se fait dans l'interruption ci-haut.
                PORTA = ETEINTE;                //La lumiere est etteinte.
                break;
            case CPT:
                if(compteur >= 120) {           //Verifie si le compteur est plus petit que 120.
                    arreterCompteur();          //Arrete le timer.
                    etat = CLIGN_VERT;          //Changement pour l'etat suivant.
                }
                break;
            case CLIGN_VERT:
                PORTA = VERTE;                  //Affichage de la lumiere verte pendant une demie seconde.
                _delay_ms(500);
                PORTA = ETEINTE;                //Exctinction de la lumiere.
                etat = CLIGN_ROUGE;             //Changement pour l'etat suivant.
                break;
            case CLIGN_ROUGE:
                clignotementRouge(compteur);
                etat = LUM_VERTE;               //Changement pour l'etat suivant.
                break;
            case LUM_VERTE:
                PORTA = VERTE;                  //Affichage de la lumiere verte pendant une seconde.
                _delay_ms(1000);
                etat = INIT;                    //Retour a l'etat initial.
                break;
        }
    }
}


/*
 * Fonction etatBoutton.
 *
 * Cette fonction permet de retourner l'etat du boutton poussoir.
 * 
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - etatBtn (bool) : Renvoie l'etat du boutton.  Si le boutton est pressé,
 *        etatBtn sera a TRUE.  Si le boutton n'est pas pressé, etatBtn sera à
 *        FALSE.  
 */
bool etatBoutton(){
    bool etatBtn = false;
    if(!(PIND & 0x04)) {                    //Début portion de code pour l'anti-rebond.
        _delay_ms(30);                      //Délai de 30ms .
        if(!(PIND & 0x04))         
            etatBtn = true;   
    }                                       //Fin portion de code pour l'anti-rebond.
    return etatBtn;
}

/*
 * Fonction clignotementRouge.
 *
 * Cette fonction permet de faire clignoter la DEL rouge un certain nombre de fois.
 * Ce nombre est le resultat du parametre recu de la fonction, divise par 2.  La DEL
 * doit clignoter a un rythme de deux clignotements par seconde.
 *
 * INPUT :
 *      - cpt (uint8_t) : Variable indiquant le nombre de fois (divise par deux) que la lumiere rouge
 *                        doit clignoter.
 * OUTPUT :
 *      - Aucun output. 
 */
void clignotementRouge(uint8_t cpt) {
    for(int i = cpt; i >= 0; i--){          
        PORTA = ROUGE;
        _delay_ms(250);
        PORTA = ETEINTE;
        _delay_ms(250);
    }
}

/*
 * Fonction initialisation.
 *
 * Cette fonction permet d'initialiser les ports en entrée ou en sortie, et les
 * registres pour la gestion du timer et des interruptions.
 *
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void initialisation(){
    cli();                                  //Empecher les interruptions.
    
    //Initialisation des ports.
    DDRA = 0xff;                            //Port A en sortie pour allumer la DEL.
    DDRD = 0x00;                            //Port D en entree pour le boutton.
    PORTD = 0x00;
    
    //Initialisation des registres pour notre TIMER.
    
    //TCCR1A : Timer/Counter 1 Control Register A.
    TCCR1A |= 0x00;
    
    //TCCR1B : Timer/Counter 1 Control Register B.
    //         WGM12    :   Waveform generation mode (CTC avec OCRn pour TOP).
    //         CS12     :   Clock Select (clk/1024).
    //         CS10     :   Clock Select (clk/1024).
    TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10) ;
    
    //TCCR1C : Timer/Counter 1 Control Register C.
    TCCR1C |= 0x00;
    
    //OCR1A  : Output Compare Register1 A.
    OCR1A = 781;                            // = (8 000 000 / 1024) / 10
    
    //Initialisation des registres pour le PIN CHANGE.
    
    //PCICR  : Pin Change Interrupt Control Register.
    //         PCIE3    :   Pin Change Interrupt 3 (PCINT31:24).
    PCICR |= _BV(PCIE3);
    
    //PCMSK3 : Pin Change Mask Register 3.
    //         PCINT26  :   Pin Change Enable Mask PCINT26 (notre PIN).
    PCMSK3 |= _BV(PCINT26);
    
    sei();                                  //Permet les interruptions.
}

/*
 * Fonction demarrerCompteur.
 *
 * Cette fonction permet de demarrer le fonctionnement du timer.  Initialise aussi
 * notre compteur a 0.
 *
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void demarrerCompteur() {
    compteur = 0x00;
    TIMSK1 |= _BV(OCIE1A);
    TCNT1 = 0x00;
}

/*
 * Fonction arreterCompteur.
 *
 * Cette fonction permet d'arreter le fonctionnement du timer.
 *
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void arreterCompteur() {
    TIMSK1 ^= _BV(OCIE1A);                  //Enleve le masque a l'aide du ou exclusif.
}
