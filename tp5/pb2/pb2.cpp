/*
 * Équipe no71
 * 
 * WILLIAM LABERGE      1852751
 * COLIN STÉPHENNE      1852773
 * SÉBASTIEN CADORETTE  1734603
 * 
 * 9 février 2017
 * 
 * TRAVAIL PRATIQUE 5 - INF1995
 * Problème 2
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

enum State {INIT,CLIGN, DEBUT, REUSSI, ECHEC};

volatile State etat = INIT;  
volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

bool etatBoutton();                 //Prototype dpour la fonction etatBoutton.



ISR(INT0_vect) {
        _delay_ms(10);                 //Delai avant de confirmer la reponse du bouton-poussoir.
        if(etatBoutton() && minuterieExpiree == 0 && etat == DEBUT)
            etat = REUSSI;
        EIFR |= (1 << INTF0);       
        
}
ISR(TIMER1_COMPA_vect) {
    minuterieExpiree = 1;
    
    if(etat == DEBUT)
        etat = ECHEC;
}

void initialisation(void){
    cli();
    DDRA = 0xff;                    //Port A est en mode sortie.
    DDRD = 0x00;                    //Port D est en mode entrée.
    EIMSK |= _BV(INT0);
    EICRA |= 3;                     //Rising edge
    sei();
}

void partirMinuterie ( uint16_t duree ) {

    minuterieExpiree = 0;

    TCCR1A = 0x80;  // Clear on compare match (low level)
    TCCR1B = 5;     // mode CTC du timer 1 avec horloge divisee par 1024
    TCCR1C = 0;     // 
    TCNT1 = 0;      //
    OCR1A = duree;  //
    TIMSK1 = 2;     //
}




int main() {
    initialisation();
    bool minuterieDemarree = false;
    for(;;) {
         switch(etat) {
            case INIT :  
                _delay_ms(10000);   //Delai de 10 secondes
                etat = CLIGN;
                break;
            case CLIGN:
                PORTA = ROUGE;
                _delay_ms(100);
                PORTA = ETEINTE;
                etat = DEBUT;
                break;
            case DEBUT :
                if(!minuterieDemarree) {
                    partirMinuterie(7812);
                    minuterieDemarree = true;
                }
                break;
            case REUSSI :
                PORTA = VERTE;       //Lumiere verte
                break;
            case ECHEC :
                PORTA = ROUGE;       //Lumiere rouge
                break;
         }     
    }
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
bool etatBoutton(){
    bool etatBtn = false;
    if(PIND & 0x04) {           //Début portion de code pour l'anti-rebond.
        _delay_ms(10);          //Délai de 10ms .
        if(PIND & 0x04)         
            etatBtn = true;   
    }                           //Fin portion de code pour l'anti-rebond.   
    return etatBtn;
}


