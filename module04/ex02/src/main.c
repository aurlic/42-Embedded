#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#define MAX_COUNT 15
#define MIN_COUNT 0

uint8_t count = 0;

volatile bool is_toggled = false;

static void display_count() {
  PORTB = 0;
  PORTB |= count;

  PORTB |= (((count & 0b1000) >> 3) << PORTB4);
}

static void handle_delay(int pin) {
  while (!(PIND & (1 << pin))) {
    ;
  }
  _delay_ms(20);
}

static void init_interrupt() {
  // Falling edge on INT0 triggers interrupt (when button is pressed)
  EICRA |= (1 << ISC01); // Set ISC01, clear ISC00 for falling edge

  // Enable PCINT20 (for PD4/SW2)
  PCMSK2 |= (1 << PCINT20);

  // Enable INT0 interrupt
  EIMSK |= (1 << INT0);

  // Enable PCINT2 interrupt
  PCICR |= (1 << PCIE2);

  // Enable global interrupts
  sei();
}

void init_pins() {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
  PORTD |= (1 << PD2) | (1 << PD4);
}

int main(void) {
  init_pins();
  init_interrupt();

  while (1) {
  }
}

ISR(INT0_vect) {
  _delay_ms(50);

  // Check if button is still pressed after debounce delay
  if (!(PIND & (1 << PD2))) {
    if (count < MAX_COUNT) {
      count++;
      display_count();
    }

    // Wait for button release to prevent multiple triggers
    while (!(PIND & (1 << PD2))) {
    }

    _delay_ms(50);
  }
}

ISR(PCINT2_vect) {
  _delay_ms(50);

  // Check if button is still pressed after debounce delay
  if (!(PIND & (1 << PD4))) {
    if (count > MIN_COUNT) {
      count--;
      display_count();
    }

    // Wait for button release to prevent multiple triggers
    while (!(PIND & (1 << PD4))) {
    }

    _delay_ms(50);
  }
}