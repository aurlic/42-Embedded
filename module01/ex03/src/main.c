#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define MAX_CYCLE 10
#define MIN_CYCLE 1

uint8_t cycle_speed = 1;

static void handle_press(int pin) {
  // Check that a switch is pressed
  if (!(PIND & (1 << pin))) {
    // If SW1 is pressed and we're not yet at max cycle speed, increment cycle
    // speed
    if (pin == PIND2 && cycle_speed < MAX_CYCLE) {
      cycle_speed++;
      OCR1A = 6249 * cycle_speed;
    }

    // If SW2 is pressed and we're not yet at max cycle speed, decrement cycle
    // speed
    else if (pin == PIND4 && cycle_speed > MIN_CYCLE) {
      cycle_speed--;
      OCR1A = 6249 * cycle_speed;
    }

    // Wait for switch release
    while (!(PIND & (1 << pin))) {
      ;
    }
    _delay_ms(50);
  }
}

int main(void) {
  // Set inputs and outputs for LED and Switches
  DDRB |= (1 << DDB1);
  DDRD |= (0 << DDD2);
  DDRD |= (0 << DDD4);

  // Configure Timer1 for fast PWM mode (cf. page 142)
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  // Configure compare mode to "Clear OC1A on Compare Match, set OC1A at BOTTOM"
  TCCR1A |= (1 << COM1A1);

  // Sets the total signal period
  ICR1 = 62499;

  // Sets the ON time to 10% of the total period (default)
  OCR1A = 6249;

  // Configure prescaler to 256
  TCCR1B |= (1 << CS12);

  while (1) {
    handle_press(PIND2);
    handle_press(PIND4);
  }
}