#include <avr/io.h>

int main(void) {
  // Set PB1 as an output
  DDRB |= (1 << DDB1);

  // Configure Timer1 for fast PWM mode (cf. page 142)
  // This mode allows using ICR1 as TOP value (total period)
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11);

  // Configure compare mode to "Clear OC1A on Compare Match, set OC1A at BOTTOM"
  // Allows precise control of LED duty cycle
  TCCR1A |= (1 << COM1A1);

  // Sets the total signal period
  // 31249 corresponds to a 2 Hz frequency with 256 prescaler
  ICR1 = 31249;

  // Sets the ON time to 10% of the total period
  // 3124 is 10% of 31249
  OCR1A = 3124;

  // Configure prescaler to 256
  // Divides CPU clock frequency by 256 to slow down the timer (cf. page 143)
  TCCR1B |= (1 << CS12);

  while (1) {
    ;
  }
}