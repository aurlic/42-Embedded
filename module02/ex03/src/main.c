#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define UBRR_VAL ((16000000 / (8UL * 115200)) - 1UL)

static void uart_tx(unsigned char data) {
  /* Check transmit buffer is empty*/
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

unsigned char uart_rx() {
  /* Wait for data to be received */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Get and return received data from buffer */
  return UDR0;
}

static void uart_init() {
  // Double transfer rate (cf. page 201)
  UCSR0A |= (1 << U2X0);

  // Set baud rate
  UBRR0H = (unsigned char)(UBRR_VAL >> 8);
  UBRR0L = (unsigned char)UBRR_VAL;

  // Enable receiver, transmitter and interrupt (cf. page 202)
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  // Set frame format: 8data, 2stop (cf. page 203)
  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

int main(void) {
  uart_init();

  // Enable global interrupt
  sei();

  // Generate a receive complete interrupt (cf. page 200)
  UCSR0A |= (1 << RXC0);

  while (1)
    ;
}

#include <ctype.h>

ISR(USART_RX_vect) {
  // Store buffer
  uint8_t data = UDR0;

  // Invert and write char case
  if (isupper(data))
    data = tolower(data);
  if (islower(data))
    data = toupper(data);

  uart_tx(data);
}