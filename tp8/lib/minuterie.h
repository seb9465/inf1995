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
void delai_us(uint16_t nombreUs);

#endif
