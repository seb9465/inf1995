#include "del.h"

Del::Del(){}
Del::~Del(){}

void Del::afficherVert() {
    etat_ = VERTE;
    PORTA &= 0b11111101;
    PORTA |= 0b00000001;
}

void Del::afficherRouge() {
    etat_ = ROUGE;
    PORTA &= 0b11111110;
    PORTA |= 0b00000010;
}

void Del::eteindre() {
    etat_ = ETEINTE;
    PORTA &= 0b11111100;
}

void Del::afficherAmbre(){
    etat_ = AMBRE;
    afficherRouge();
    _delay_ms(1);
    afficherVert();
    _delay_ms(3);
}

uint8_t Del::getEtat() {
    return etat_;
}
