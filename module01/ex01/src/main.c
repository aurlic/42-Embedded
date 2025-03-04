#include <avr/io.h>

int main(void) {
  // Set PB1 as an output
  DDRB |= (1 << DDB1);

  // Timer1 is using 16-bits (cf. page 120) so we need to set 2 registers

  // Config Timer1 for CTC (cf. page 141)
  TCCR1B |= (1 << WGM12);
  // Instruct to invert LED when signal is matched (cf. page 140)
  TCCR1A |= (1 << COM1A0);

  // Formula: OCR1A = (CPU_Clock / (Prescaler * Desired_Frequency)) - 1
  // Set CTC compare value to 0,5Hz at 16MHz, AVR clock, with a prescale of 256
  OCR1A = 31249;

  // Change clock source depending on our prescaler (cf. page 143)
  TCCR1B |= (1 << CS12);
  while (1) {
    ;
  }
}