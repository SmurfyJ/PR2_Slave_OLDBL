#include <avr/interrupt.h>

void init_spi();
void init_pwm();
void spi_rx();

uint16_t rx;

int main() {

    init_spi();
    init_pwm();

    while (1) {

        spi_rx();

    }
}

/**
 * Useful function
 */
void init_spi() {

    SPCR = (1 << SPE);                          // enable SPI

}

/**
 * Initiate PWM registers
 */
void init_pwm() {

    DDRB |= (1 << PB1);                         // set D9 (OC1A) to output
    TCCR1A = TCCR1B = 0;                        // reset timer 1

    /* mode: fast PWM 10 bit */
    TCCR1A |= (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM12);

    TCCR1B |= (1 << CS11) | (1 << CS10);        // prescale: 64
    TCCR1A |= (1 << COM1A1);                    // N-INV

}

/**
 * Recieve new PWM value
 */
void spi_rx() {

    while (!(SPSR & (1 << SPIF)));
    rx = SPDR;

    if (rx == (uint8_t) 'V') {

        while (!(SPSR & (1 << SPIF)));
        rx = SPDR;
        OCR1A = (uint16_t) (rx / 255. * 0x03FF);

    }

}