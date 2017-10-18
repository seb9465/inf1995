/***************************************************************************
 * Fichier :     main.cpp
 *
 * Description : Fichier main, contenant le coeur du programme.
 *               Permet la gestion du programme.
 *
 * Auteur :      Mathieu Chateauvert
 *               Colin Stéphenne
 *               William LaBerge
 *               Son-Thang Pham
 *               Sébastien Cadorette
 *
 * Groupe :      6771
 *
 * Date :        19 avril 2017
 ***************************************************************************/

#include "Parcours.h"

enum Etat {ASSIGNATION_COULEUR, CHANGEMENT_ETAT, DEBUT_PARCOURS, STATIONNEMENT_TROIS_LIGNES, LOI_NORMALE,
            ROTATION_540, GUIDAGE_LUMIERE, LIGNE_POINTILLE, STATIONNEMENT_FINAL, FIN_ENUM_ETATS_MAIN};

int main() {
    bool parcoursFini = false, changerEtat = false, estClique = false;
    Parcours parcours = Parcours();
    Bouton boutonEtat = Bouton(&PIND, 2, false);
    CouleurParcours couleurParcours = PARCOURS_VERT;
    uint8_t etatPresent = ASSIGNATION_COULEUR;

    while(!parcoursFini){
        switch(etatPresent){
           
            /*Assignation couleur permet de choisir la couleur de parcours
            et choisir de debuter le parcours ou daller vers la selection d'etat*/
            case ASSIGNATION_COULEUR :
                couleurParcours = parcours.assignerCouleurParcours(changerEtat);
                if(changerEtat)
                    etatPresent = CHANGEMENT_ETAT;
                else
                    etatPresent = DEBUT_PARCOURS;
                break;
            
            /*Changement etat permet de selectionner un etat particulier en
            appuyant un certain nombre de fois sur le bouton interrupt et de 
            confirmer ce choix en maintenant le bouton enfonce*/
            case CHANGEMENT_ETAT :
                while(!estClique) {
                    if(boutonEtat.etatBouton()) {
                        delai_ms(800);
                        if(boutonEtat.etatBouton()) {
                            estClique = true;
                        }
                        else {
                            etatPresent++;
                            if(etatPresent == FIN_ENUM_ETATS_MAIN)
                                etatPresent = DEBUT_PARCOURS;
                        }
                    }
                }
                break;
            
            /*Debut parcours se rend du depart jusqu'au segment AB*/
            case DEBUT_PARCOURS :
		        parcours.debutParcours();
                etatPresent = STATIONNEMENT_TROIS_LIGNES;
                break;
            
            /*Stationnement trois ligne effectue le parcours du segment AB jusqu'au segment BC*/       
            case STATIONNEMENT_TROIS_LIGNES :
                parcours.stationnementTroisLignes(couleurParcours);
                etatPresent = LOI_NORMALE;
                break;
            
            /*Loi normale effectue le parcours du segment BC A MODIFIER*/
            case LOI_NORMALE :
                parcours.loiNormale(couleurParcours);
                etatPresent = ROTATION_540;
                break;
            
           /*Rotation 540 effectue le parcours du segment BC jusqu'au segment E*/ 
            case ROTATION_540 :
                parcours.cinqQuarante();
                etatPresent = GUIDAGE_LUMIERE;
                break;
            
            /*Guidage lumiere effectue le point E et F*/
            case GUIDAGE_LUMIERE :
                parcours.touchdown();
                etatPresent = LIGNE_POINTILLE;
                break;
            
            /*Ligne pointillee effectue la ligne du pointillee*/
            case LIGNE_POINTILLE :
                parcours.pointille();
                etatPresent = STATIONNEMENT_FINAL;
                break;
            
            /*Stationnement final effectue le parcours du point G au stationnement final*/
            case STATIONNEMENT_FINAL :
                parcours.stationnementFin(couleurParcours);
                parcoursFini = true;
                break;
        }
    }
    return 0;
}
