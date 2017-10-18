#ifndef CYTRON_H
#define CYTRON_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_CYTRON PINC

enum Detecteur {GAUCHE2, GAUCHE1, CENTRE, DROITE1, DROITE2};

class Cytron {
public:
    Cytron();
    //~Cytron();
    
    void initialisation();
    void debutDetection();
    void arretDetection();
    int getPin(Detecteur DECT);
    bool Detection(Detecteur dect);

private:
    Detecteur DECT_;
};

#endif
