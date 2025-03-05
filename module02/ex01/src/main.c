#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define UBRR_VAL ((16000000 / (8UL * 115200)) - 1UL)

static void uart_tx(unsigned char data) {
  /* Check transmit buffer is empty*/
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

static void uart_printstr(char *s) {
  // Transmit character until NULL is reached
  while (*s > 0)
    uart_tx(*s++);
}

static void uart_init() {
  // Double transfer rate (cf. page 201)
  UCSR0A |= (1 << U2X0);

  // Set baud rate
  UBRR0H = (unsigned char)(UBRR_VAL >> 8);
  UBRR0L = (unsigned char)UBRR_VAL;

  // Enable transmitter (cf. page 202)
  UCSR0B |= (1 << TXEN0);

  // Set frame format: 8data, 2stop (cf. page 203)
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main(void) {
  uart_init();

  // Configure timer 1 for CTC mode
  TCCR1B |= (1 << WGM12);

  // Enable CTC interrupt
  TIMSK1 |= (1 << OCIE1A);

  // Enable global interrupt
  sei();

  // Set CTC compare value to 2Hz at 16 MHz AVR clock , with a prescaler of 1024
  OCR1A = 31249;

  // Start timer at Fcpu /1024
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);

  while (1) {
  }
}

ISR(TIMER1_COMPA_vect) { uart_printstr("Hello World!\n\r"); }