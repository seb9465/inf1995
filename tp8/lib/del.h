#ifndef DEF_DEL
#define DEF_DEL

#include "constantes.h"

class Del {
public:
    Del();
    ~Del();
    void afficherVert();
    void afficherRouge();
    void afficherAmbre();
    void eteindre();
    uint8_t getEtat();
    
private:
    uint8_t etat_;
};

#endif
