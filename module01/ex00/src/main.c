#include <avr/io.h>

static void delay_ms(uint16_t ms) {
  for (volatile uint16_t i = 0; i < ms; i++) {
    // Wait for about 1ms
    for (volatile uint16_t j = 0; j < 1000; j++) {
    }
  }
}

int main(void) {
  // Set PB1 as an output
  DDRB |= (1 << DDB1);

  while (1) {
    // Invert LED state
    PORTB ^= (1 << PORTB1);

    // Wait for about 500ms
    delay_ms(500);
  }
}