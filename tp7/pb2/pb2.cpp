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
 * Problème 2
 */

/*
 *                      SYNOPSIS
 * La valeur retournée par le convertisseur est de 10 bits. Par contre, les derniers 2 bits sont
 * sans signification et peuvent donc être négligés ce qui permet de réduire le format des données à 8 bits.
 * 
 * Il faut faire un robot pouvant se faire bronzer.  Nous avons trois choix possibles :
 *      1. Lorsque la lumiere est basse, la DEL prend la couleur verte.
 *      2. Lorsque la lumiere est de bon niveau, la DEL prend la couleur ambré.
 *      3. Lorsque la lumiere est trop forte, la DEL prend la couleur rouge.
 */

/*                 DISPOSITIONS DES FILS
 * Fils bleu et noir    :   PORTA
 *      Bleu   :            Broche 1
 *      Noir   :            Broche 2
 * 
 * Fils rouge et noir   :   PORTA
 *      Rouge  :            Broche Vcc
 *      Noir   :            Broche Grnd
 * 
 * Fils jaune et orange :   PORTB       -   DEL
 *      Jaune  :            Broche 2    -   Gauche
 *      Orange :            Broche 1    -   Droite
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "can.h"

#define ETEINTE 0x00
#define VERTE   0x01
#define ROUGE   0x02

#define VOLTAGE 5
#define PREMIER_SEUIL VOLTAGE * 17          //Premier seuil = 85
#define DEUXIEME_SEUIL VOLTAGE * 34         //Deuxieme seuil = 170

#define PIN_RESISTANCE 0x01                 //Notre photorésistance est connecté sur la pin 1.

//Prototypes de nos fonctions.
void afficherAmbre();
void initialisation();


int main() {
    uint8_t position = PIN_RESISTANCE;                       
    uint8_t valeur;         
    can uneConversion;
    
    for(;;) {
        valeur = uneConversion.lecture(position) >> 2;  //Décalage de 2 bits vers la droite pour un resultat a 8 bits.
        if(valeur <= PREMIER_SEUIL) {
            PORTB = VERTE;
        } else if(valeur > PREMIER_SEUIL && valeur < DEUXIEME_SEUIL) {
            afficherAmbre();
        } else {
            PORTB = ROUGE;
        }
    }
}

/*
 * Fonction afficherAmbre.
 * 
 * Permet d'afficher la couleur ambre sur la DEL.
 * 
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void afficherAmbre(){
    PORTB = ROUGE;
    _delay_ms(1);
    PORTB = VERTE;
    _delay_ms(3);
}

/*
 * Fonction initialisation.
 * 
 * Permet d'initialiser nos ports en entree ou en sortie.
 * 
 * INPUT :
 *      - Aucun input.
 * OUTPUT :
 *      - Aucun output.
 */
void initialisation() {
    DDRB = 0xff;                        //Port B en sortie.
    DDRA = 0x00;                        //Port A en entree.
}

