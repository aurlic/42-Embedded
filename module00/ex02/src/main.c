#include <avr/io.h>
#include <util/delay.h>

int main(void) {
  // Set PB0 as output
  DDRB |= (1 << DDB0);
  // Set PD2 as input (which is its default state) (cf. schema)
  DDRD |= (0 << DDD2);

  while (1) {
    // Check that the switch is pressed (PD2 on LOW) (cf. 14.4.10 and schema)
    if (!(PIND & (1 << PIND2))) {
      // Turn on the LED
      PORTB |= (1 << PORTB0);
      // Delay to avoid spam
      _delay_ms(100);
    } else {
      // Invert the bits (to keep track of previous state)
      PORTB &= ~(1 << PORTB0);
    }
  }

  return 0;
}