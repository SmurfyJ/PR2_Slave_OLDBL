#include <avr/interrupt.h>
#include <Arduino.h>

uint16_t rx;
uint16_t tx;

void init_spi();
void init_pwm();
void spi_rx();
void spi_tx();

int main()
{
    // Serielle Schnittstelle für Debug-Ausgaben initialisieren
    init();
    Serial.begin(9600);



    init_spi();
    init_pwm();

    while (1) {

        spi_rx();

    }
}

void init_spi() {

    // Set MISO output (alle anderen Eingänge)
    DDRB = (1 << DDB4);
    // Enable SPI
    SPCR = (1 << SPE);

}


// siehe Timer-Übungsaufgabe
void init_pwm() {

    DDRB |= (1 << PB1);                         // set D9 (OC1A) to output

    TCCR1A = TCCR1B = 0;                        // reset timer 1

    /* mode: fast PWM 10 bit */
    TCCR1A |= (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS11)   | (1 << CS10);      // prescale: 64
    TCCR1A |= (1 << COM1A1);                    // N-INV

    OCR1A = 255;

}

void spi_rx() {

    // Warten bis SPI Byte empfangen wurde
    while (!(SPSR & (1 << SPIF)));

    rx = SPDR;
    Serial.println("Got one:");

    if(rx == (uint8_t) 'V') {

        while (!(SPSR & (1 << SPIF)));
        rx = SPDR;
        Serial.println(rx);
        Serial.println((rx / 255. * 0x03FF));

        OCR1A = (uint16_t) (rx / 255. * 0x03FF);

    }

}

void spi_tx() {

}