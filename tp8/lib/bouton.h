#ifndef DEF_BOUTON
#define DEF_BOUTON

#include "constantes.h"

/*
 * Fonction etatBoutton.
 * 
 * INPUT :
 *      - port : le port du bouton
 *		- pin : la pin du bouton
 * OUTPUT :
 *      - etatBtn (bool) : Renvoie l'etat du boutton.  Si le boutton est presse,
 *        etatBtn sera a TRUE.  Si le boutton n'est pas presse, etatBtn sera a
 *        FALSE.
 */
bool etatBoutton(Ports port, uint8_t pin);

#endif
