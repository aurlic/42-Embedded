#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB |= (1 << DDB0);

  while (1) {
    // Check that the switch is pressed (PD2 on LOW)
    if (!(PIND & (1 << PIND2))) {
      // XOR operation to invert LED state
      PORTB ^= (1 << PB0);
      // Wait for button release
      while (!(PIND & (1 << PIND2))) {
        ;
      }
      // Avoid bounce with a small delay
      _delay_ms(50);
    }
  }
}