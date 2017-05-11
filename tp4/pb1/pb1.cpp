/*
 * Equipe no71
 * 
 * WILLIAM LABERGE      1852751
 * COLIN STEPHENNE      1852773
 * SÉBASTIEN CADORETTE  1734603
 * 
 * 26 janvier 2017
 * 
 * TRAVAIL PRATIQUE 4 - INF1995
 * Probleme 1
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PORT_SORTIE 0xff
#define ETEINT 0x00
#define VERT 0x01
#define ROUGE 0x02

void delai_us(uint16_t nombreUs);           //Prototype de la fonction delai_us.

int main() {
    DDRD = PORT_SORTIE;                     //Port D en sortie.
    
    //Initialisation des variables locales.
    uint16_t force = 3000;                  
    uint16_t a, b;
    uint8_t couleur = ROUGE;
    
    b = 250;                                //"b" correspond a 1 ms en us.
    a = b;                                  //Pleine puissance au debut.
    
    //PWM
    for(;;) {
        if(a > 0)                           //UTILE SI ON NE FAIT PAS FLASHER LA DEL SANS ARRET.  SINON CE NEST PAS UTILE.
            PORTD = couleur;   
        delai_us(a);
        PORTD = ETEINT;   
        delai_us(b-a);
        
        
        if(force > 0) {
            if(force % 3 == 0 && a > 0)     //"a" sera decremente seulement a chaque 3 decrementation de "force".  Donc "force" 
                a--;                        //decrementera de 3 pendant que "a" decrementera de 1.  De plus, "a" sera decremente
            force--;                        //seulement si la force est plus grande que 0, question de ne pas decremente "a" inutilement.
        }
        
        if(a <= 0) {                        //Quand "a" atteint 0, on change la couleur de la DEL et on remet "a" et "force" a leur valeur initiale.
            if(couleur == ROUGE)
                couleur = VERT;
            else
                couleur = ROUGE;
            
            a = b;                          //"a" reprendra sa valeur initiale de 1000.
            force = 3000;                   //"force" reprendra sa valeur initiale de 3000.
        }
        
        /*
        if(a > 0)                         
            PORTD = couleur;   //Enable;
        delai_us(a);
        PORTD = ETEINT;   //Disable
        delai_us(b-a);
        
        
        if(force > 0 && !incremente) {
            if(force % 12 == 0 && a > 0)     //"a" sera décrémenté seulement a chaque 3 décrémentation de "force".  Donc "force" 
                a--;                        //décrémentera de 3 pendant que "a" décrémentera de 1.  De plus, "a" sera decremente
            force--;                        //seulement si la force est plus grande que 0, question de ne pas decremente "a" inutilement.
        }
        
        if(force < 3000 && incremente) {
            if(force % 12 == 0 && a < 250)     //"a" sera décrémenté seulement a chaque 3 décrémentation de "force".  Donc "force" 
                a++;                        //décrémentera de 3 pendant que "a" décrémentera de 1.  De plus, "a" sera decremente
            force++;                        //seulement si la force est plus grande que 0, question de ne pas decremente "a" inutilement.
        }
        
        if(a <= 1 && !incremente) {                        //Quand "a" atteint 0, on change la couleur de la DEL et on remet "a" et "force" a leur valeur initiale.
            if(couleur == ROUGE)
                couleur = VERT;
            else
                couleur = ROUGE;
            //if(incremente) {
            //a = b;                          //"a" reprendra sa valeur initiale de 1000.
            //force = 3000;                   //"force" reprendra sa valeur initiale de 3000.
            //}
            incremente = !incremente;
        }
        if(a >=  249 && incremente)
        {
            incremente = !incremente;
        }
        */
    }
    
}

/*                      CECI EST LE CODE QUI PERMET DE FAIRE FONCTIONNER LA DEL SANS ARRET EN INCREMENTATION ET EN DESINCREMENTATION.
 *                                                Mettre tout cela dans la boucle for si on veut l'utiliser.
    if(a > 0)                         
            PORTD = couleur;   //Enable;
        delai_us(a);
        PORTD = ETEINT;   //Disable
        delai_us(b-a);
        
        
        if(force > 0 && !incremente) {
            if(force % 6 == 0 && a > 0)     //"a" sera décrémenté seulement a chaque 3 décrémentation de "force".  Donc "force" 
                a--;                        //décrémentera de 3 pendant que "a" décrémentera de 1.  De plus, "a" sera decremente
            force--;                        //seulement si la force est plus grande que 0, question de ne pas decremente "a" inutilement.
        }
        
        if(force < 3000 && incremente) {
            if(force % 6 == 0 && a < 500)     //"a" sera décrémenté seulement a chaque 3 décrémentation de "force".  Donc "force" 
                a++;                        //décrémentera de 3 pendant que "a" décrémentera de 1.  De plus, "a" sera decremente
            force++;                        //seulement si la force est plus grande que 0, question de ne pas decremente "a" inutilement.
        }
        
        if(a <= 1 && !incremente) {                        //Quand "a" atteint 0, on change la couleur de la DEL et on remet "a" et "force" a leur valeur initiale.
            if(couleur == ROUGE)
                couleur = VERT;
            else
                couleur = ROUGE;
            //if(incremente) {
            //a = b;                          //"a" reprendra sa valeur initiale de 1000.
            //force = 3000;                   //"force" reprendra sa valeur initiale de 3000.
            //}
            incremente = !incremente;
        }
        if(a >=  499 && incremente)
        {
            incremente = !incremente;
        }
 */




/***************************************************************************
 * Fonction :    delai_us
 * Description : Nous avons créé une fonction afin de nous faciliter un peu
 *               la tache pour gerer les delais.  Nous avons utilisé
 *               _delay_loop_2 pour créer un delai, mais nous devons modifié
 *               la valeur du parametre pour pouvoir avoir un nombre de us
 *               bien précis.  Nous avons donc créé une fonction afin que cela
 *               soit geré a part de notre main.
 *  Parametres : - (unint16_t) nombreUs : Contient le nombre de microsecondes
 *               durant lesquels on veut un délai.
 *  Retour :     Aucun.
 ***************************************************************************/
void delai_us(uint16_t nombreUs) {
    _delay_loop_2(2 * nombreUs); //car 2 x 4 cycles a 8 MHz donne 1 us.
}
