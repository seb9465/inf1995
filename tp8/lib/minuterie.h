#ifndef DEF_MINUTERIE
#define DEF_MINUTERIE

#include "constantes.h"

class Minuterie {
public:
	Minuterie();
	void partirMinuterie(uint16_t duree);
	void arreterMinuterie();
	bool getMinuterieExpiree();
	void setMinuterieExpiree(bool expiree);
	
private:
	bool minuterieExpiree_;
};

/***************************************************************************
 * Fonction :    delai_us
 * Description : Nous avons cr�� une fonction afin de nous faciliter un peu
 *               la tache pour gerer les delais.  Nous avons utilis�
 *               _delay_loop_2 pour cr�er un delai, mais nous devons modifi�
 *               la valeur du parametre pour pouvoir avoir un nombre de us
 *               bien pr�cis.  Nous avons donc cr�� une fonction afin que cela
 *               soit ger� a part de notre main.
 *  Parametres : - (unint16_t) nombreUs : Contient le nombre de microsecondes
 *               durant lesquels on veut un d�lai.
 *  Retour :     Aucun.
 ***************************************************************************/
void delai_us(uint16_t nombreUs);

#endif
