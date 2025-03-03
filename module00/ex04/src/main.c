#include <avr/io.h>
#include <util/delay.h>

#define MAX_COUNT 15
#define MIN_COUNT 0

uint8_t count = 0;

static void display_count() {
  // Set the first 3 bits of PORTB equal as those of count
  PORTB = 0;
  PORTB |= count;

  // Equivalent to PORTB = count:
  // PORTB |= ((count & 0b1) << PB0);
  // PORTB |= ((count & 0b10) << PB1);
  // PORTB |= ((count & 0b100) << PB2);

  // First check the state of count's 4th bit then shift it by 3 to set PB4 as 1
  PORTB |= (((count & 0b1000) >> 3) << PORTB4);
}

static void handle_count(int pin) {
  // Check that a switch is pressed
  if (!(PIND & (1 << pin))) {
    // If SW1 is pressed and we're not yet at max count, increment count
    if (pin == PIND2 && count < MAX_COUNT) {
      count++;
    }

    // If SW2 is pressed and we're not yet at max count, decrement count
    else if (pin == PIND4 && count > MIN_COUNT) {
      count--;
    }

    // Wait for switch release
    while (!(PIND & (1 << pin))) {
      ;
    }
    _delay_ms(50);
  }
}

int main() {
  // Set all LEDs as outputs
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

  while (1) {
    handle_count(PIND2);
    handle_count(PIND4);
    display_count();
  }
}