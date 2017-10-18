#ifndef DEF_UART
#define DEF_UART

#include "constantes.h"

class UART {
public:
	UART();
	void transmissionUART(uint8_t donnee);
	uint8_t receptionUART();
	void initialisationUART();
};

#endif
