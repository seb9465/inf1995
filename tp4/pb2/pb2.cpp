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
 * Probleme 2
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define PORT_SORTIE 0xff
#define F_1 60
#define F_2 400
#define ETEINT 0x00
#define AVANCE 0x01

void delai_us(uint32_t nombreUs, int freq);                             //Prototype de la fonction delai_us.

void debug(){PORTD = 0x02; for(;;){}};                                  //Fonction utile pour debuguer.

int main() {
    
    DDRD = PORT_SORTIE;                                                 //Port D en sortie.
    const uint32_t force_F_1[5] = {0, 33333, 66666, 99999 , 133332};    //Nombre de cycle d'horloge du "a" pour la frequence 1.
    const uint32_t force_F_2[5] = {0, 5000, 10000, 15000, 20000};       //Nombre de cycle d'horloge du "a" pour la frequence 2.
    uint32_t compteurDeB = 0;
    uint32_t a, b = 0;
    

    b = force_F_1[4];                                                   //"b" correspond a ...
    a = 0;
    int frequence = F_1;
    PORTD = ETEINT;
    
    for(;;) {
        if(a > 0) { 
            PORTD = AVANCE;
            delai_us(a, frequence);
        }
        PORTD = ETEINT;   
        delai_us((b-a)+1, frequence);                                   //Il ne faut jamais faire un delai de 0, meme quand a = b.
        compteurDeB++;
        
        if(frequence == F_1 && compteurDeB >= 120) {                    //120 b par deux secondes a une frequence de 60Hz.
            uint8_t varTemp = 0;
            for(uint16_t i = 0; i < 5; i++) {
                if(a == force_F_1[i])
                    varTemp = i;
            }
            if(varTemp >= 4){
                a = force_F_2[0];
                frequence = F_2;
                b = force_F_2[4];
            } else
                a = force_F_1[++varTemp];
            compteurDeB = 0;
        } else if(frequence == F_2 && compteurDeB >= 800)  {            //800 b par deux secondes a une frequence de 400 Hz.
            
            uint8_t varTemp = 0;
            for(uint16_t i = 0; i < 5; i++) {
                if(a == force_F_2[i])
                    varTemp = i;
            }
            if(varTemp >= 4) {
                a = force_F_1[0];
                frequence = F_1;
                b = force_F_1[4]; 
            }
            else
                a = force_F_2[++varTemp];
            compteurDeB = 0;
        }
    }
    
}

/***************************************************************************
 * Fonction :    delai_us
 * Description : Nous avons créé une fonction afin de nous faciliter un peu
 *               la tache pour gerer les delais.  Nous avons utilisé
 *               _delay_loop_2 pour créer un delai, mais nous devons modifié
 *               la valeur du parametre pour pouvoir avoir un nombre de us
 *               bien précis.  Nous avons donc créé une fonction afin que cela
 *               soit geré a part de notre main.
 *  Parametres : - (unint16_t) nombreCycle : Contient le nombre de microsecondes
 *               durant lesquels on veut un délai.
 *  Retour :     Aucun.
 ***************************************************************************/
void delai_us(uint32_t nombreCycle, int freq) {
    if(freq == F_1) {
        nombreCycle /= 3;
        double nombreIterations = floor(nombreCycle/255);    //la fonction _delay_loop_1 prend 255 comme valeur maximale!!!
        for(int i=0; i<nombreIterations; i++)
            _delay_loop_1(255);
        _delay_loop_1(nombreCycle%255);
    }
    else {
        nombreCycle /= 4;
        _delay_loop_2(nombreCycle);
    }
}
