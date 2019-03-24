/// De la mémoire vers le PC par RS232

// Connexions
//
// RS232:
// Mettre en place le cavalier DbgEN
//
// Mémoire:
// Mettre en place le cavalier MemEN

#include <avr/io.h>
#define F_CPU 8'000'000
#include <util/delay.h>
#include "memoire_24.h"

/// Initialize USART0 for RS232 asynchronous communication
void initializeUsart()
{
    // USART baud rate register flags
    UBRR0H = 0;
    UBRR0L = 0xCF; // Set to 2400 bauds (207 with U2X0 set to 0 (default))
    
    // USART control and status register flags
    UCSR0A = 0;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Enable receiver and transmitter for USART0 by setting RXEN0 and TXEN0 bits to 1
    UCSR0C &= ~(1 << UPM01) & ~(1 << UPM00); // Set parity mode to disabled by setting UPM0 bits to 00
    UCSR0C &= ~(1 << USBS0); // Set stop bit select mode to 1-bit by setting USBS0 bit to 0
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // Set character size to 8 bits by setting UCSZ0 bits to 11
}

/// Initialize initial program state
void initialize()
{
    // Initialize USART
    initializeUsart();
}

/// Transmit data through USART. Refer to Atmel documentation, p.174
void usartTransmit(uint8_t data)
{
    // Wait for empty transmit buffer in USART0
    while ((UCSR0A & (1 << UDRE0)) == false)
    {
    }
    
    // Put data into buffer, sends the data
    UDR0 = data;
}

int main()
{
    initialize();
    
    Memoire24CXXX memoryInterface;
    
    // For testing without external memory
    uint8_t writeData[] = {'A', 'l', 'l', 'o', '!', '\n', 0xFF};
    memoryInterface.ecriture(0x00, writeData, sizeof(writeData));
    
    uint16_t address = 0;
    uint8_t readChar;
    while (true)
    {
        memoryInterface.lecture(address++, &readChar);
        if (readChar == 0xFF) // Stop transmitting when 0xFF is read
        {
            break;
        }
        usartTransmit(readChar);
    }
}