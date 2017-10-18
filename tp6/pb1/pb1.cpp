
#define F_CPU 8000000UL

#define ETEINTE 0x00
#define VERTE 0x01
#define ROUGE 0x02

#include <util/delay.h>
#include "memoire_24.h"


int main() {
    DDRA = 0xff;
    
    Memoire24CXXX unObjet;          
    
    uint16_t adresse = 0x00;
    
#pragma region Ecriture et lecture d'un tableau de caractere
    uint8_t tableauEcriture[] = {"E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*"};		//Tableau contenent ce qu'il faut ecrire dans la mémoire externe.
    uint8_t sizeTableau = sizeof(tableauEcriture) / sizeof(uint8_t);			//Taille du tableau d'ecriture.
    uint8_t tableauLecture[sizeTableau];										//Il nous faut preciser la taille de ce tableau pour qu'il aie
																				//la même taille que le tableau d'écriture.

	//On passe en parametre l'adresse de départ, le tableau d'ecriture
	//ainsi que la taille de ce tableau.
    unObjet.ecriture(adresse, tableauEcriture, sizeTableau);					//Fonction permettant l'ecriture dans la mémoire externe.
    
	//On passe en parametre l'adresse de depart, le tableau dans lequel
	//on veut mettre les donnees ainsi que sa taille.
    unObjet.lecture(adresse, tableauLecture, sizeTableau);						//Fonction permettant la lecture de ce qu'on a ecrit dans la memoire externe.

    //On parcourt nos deux tableaux afin de les comparer.  Dès que
	//deux donnes ne sont pas les memes, on quitte la boucle immediatement.
    bool donneePareil = true;
    for(uint8_t i = 0; i < sizeTableau && donneePareil; i++) {					//Les deux tableaux sont de même tailles.  On les parcours pour les comparer.
		if (tableauEcriture[i] != tableauLecture[i])
			donneePareil = false;;												//Si les donnees sont differentes, on change pour false.  Utile plus bas.
    }

    //Sur la DEL libre, le fil orange a droite.  Le fil jaune a gauche.
    //Sur la PIN, le fil orange en haut.  Le fil jaune en bas.
    //De cette facon, on permet que la lumiere verte soit sur le signal 0x02 et
    //la lumiere rouge sur le signal 0x01
    if(donneePareil) {
        PORTA = 0x02;       //LUMIERE VERTE
    }
    else
        PORTA = 0x01;       //LUMIERE ROUGE
    
}
#pragma endregion 
    
#pragma region Ecriture d'un seul caractere
    
 //   uint8_t valeurEcriture = 'e';
 //   uint8_t valeurLecture;
 //   
 //   unObjet.ecriture(adresse, valeurEcriture);
 //   _delay_ms(5);
 //   unObjet.lecture(adresse, &valeurLecture);

	//if(valeurEcriture == valeurLecture) {
	//PORTA = 0x02;       //LUMIERE VERTE
	//}
	//else
	//PORTA = 0x01;       //LUMIERE ROUGE
    
#pragma endregion
    




