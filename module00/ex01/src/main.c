#include <avr/io.h>

int main(void) {
  /*
  DDRB is the Data Direction Register for port B.
  (1 << DDB0) shifts the bit 1 to the DDB0 position, and sets it as an output.

  (cf. datasheet 14.2, 14.4)
  */
  DDRB |= (1 << DDB0);

  /*
  PORTB is the Output Register for port B.
  (1 << PORTB0) shifts the bit 1 to the PORTB0 position, and sets it to HIGH.

  (cf. datasheet 14.4)
  */
  PORTB |= (1 << PORTB0);

  return 0;
}