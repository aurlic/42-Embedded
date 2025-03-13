#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

volatile bool is_toggled = false;

void init_interrupt() {
  // Rising edge on INT0 triggers interrupt
  EICRA |= (1 << ISC01) | (1 << ISC00);

  // Enable INT0 interrupt
  EIMSK |= (1 << INT0);

  // Enable global interrupts
  sei();
}

void init_pins() {
  DDRB |= (1 << PB0);
  PORTD |= (1 << PD2);
}

int main(void) {
  init_pins();
  init_interrupt();

  while (1) {
    if (is_toggled) {
      PORTB ^= (1 << PB0);
      _delay_ms(250);
      is_toggled = false;
    }
  }
}

// Interrupt Service Routine (ISR) for INT0
ISR(INT0_vect) { is_toggled = true; }