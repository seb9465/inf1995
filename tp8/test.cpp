//Tous les include des fichiers qu'on a dans notre librairie.
#include "lib/bouton.h"
#include "lib/can.h"
#include "lib/del.h"
#include "lib/memoire_24.h"
#include "lib/minuterie.h"
#include "lib/moteur.h"
#include "lib/uart.h"

int main() {
    Moteur moteur;
    moteur.avancer(10);
    
    for(;;) {
        
    }
}


