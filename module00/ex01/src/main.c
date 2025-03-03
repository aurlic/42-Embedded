#include <avr/io.h>

int main(void) {
  /*
  DDRB is the Data Direction Register for port B.
  Each bit in this register corresponds to a pin on port B.
  Setting a bit to 1 configures the corresponding pin as an output.
  (1 << DDB0) shifts the bit 1 to the DDB0 position, which corresponds to pin
  PB0. The |= operator performs a bitwise OR operation, allowing PB0 to be set
  as an output without affecting other pins.
  */
  DDRB |= (1 << DDB0);

  /*
  PORTB is the Output Register for port B.
  Setting a bit to 1 in this register sets the corresponding pin to the HIGH
  state (5V). (1 << PORTB0) shifts the bit 1 to the PORTB0 position, which
  corresponds to pin PB0. The |= operator performs a bitwise OR operation,
  allowing PB0 to be set to HIGH without affecting other pins.
  */
  PORTB |= (1 << PB0);

  return 0;
}