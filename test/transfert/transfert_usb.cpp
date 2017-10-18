#include "../lib/uart.h"
#include "../lib/memoire_24.h"
#include <avr/io.h>

void transferer();
void testerTransfert();

int main() {
	DDRA = 0xff;
	PORTA = 0x00;
	transferer();
	//testerTransfert();
}

void transferer() {
	PORTA = 0x02;
	//aquisition des donnees depuis le PC
	UART uart;
	uart.initialisationUART();

	//la taille doit etre transmise le high byte en premier
	uint16_t taille = 0x00 | (uart.receptionUART() << 8);
	taille |= uart.receptionUART();
	taille -= 2; //pour pallier au fait que progmem est mal fait...

	PORTA = 0x00;

	uint8_t tableau[taille];
	for(uint8_t i=0; i<taille; i++) {
		tableau[i] = uart.receptionUART();
	}

	//transfert des donnees vers la memoire externe
	Memoire24CXXX memoire;
	memoire.ecriture(0x00, (uint8_t)(taille >> 8));
	memoire.ecriture(0x01, (uint8_t)(taille));
	memoire.ecriture(0x02, tableau, taille);

	PORTA = 0x01;
}

void testerTransfert() {
	PORTA = 0x02;
	//aquisition des donnees depuis la memoire externe
	Memoire24CXXX memoire;
	uint8_t getter;
	memoire.lecture(0x00, &getter);
	uint16_t taille = 0x0000 | (getter << 8);
	memoire.lecture(0x01, &getter);
	taille |= getter;

	uint8_t tableau[taille];
	memoire.lecture(0x02, tableau, taille);

	//transfert des donnees vers le PC
	UART uart;
	uart.initialisationUART();

	//la taille doit etre transmise le high byte en premier
	uart.transmissionUART((uint8_t)(taille >> 8));
	uart.transmissionUART((uint8_t)(taille));

	for(uint8_t i=0; i<taille; i++) {
		uart.transmissionUART(tableau[i]);
	}

	PORTA = 0x01;
}
