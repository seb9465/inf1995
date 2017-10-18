//#define F_CPU 8000000UL
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>

//Tous les include des fichiers qu'on a dans notre librairie.
#include "lib/bouton.h"
#include "lib/can.h"
#include "lib/del.h"
#include "lib/memoire_24.h"
#include "lib/minuterie.h"
#include "lib/moteur.h"
#include "lib/uart.h"
//Fin des include des fichiers qu'on a dans notre librairie.

int main() {
    Moteur moteur;
    moteur.avancer(10);
    
    
    
    
    for(;;) {
        
    }
}


