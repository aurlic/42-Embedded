#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

// 144Hz calculated with a prescaller of 8
#define TOP 14285
#define STEP (TOP / 50) // Adjusts OCR1A in 50 steps (1s up, 1s down)

volatile int16_t duty_cycle = 0;
volatile bool increasing = true;

void init_interrupt() { sei(); }

void init_pins() { DDRB |= (1 << PB1); }

void init_timer1() {
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  TCCR1A |= (1 << WGM11);
  TCCR1A |= (1 << COM1A1);

  ICR1 = TOP;

  OCR1A = 0;
}

void init_timer0() {
  TCCR0A |= (1 << WGM01);
  TCCR0B |= (1 << CS02) | (1 << CS00);

  OCR0A = 156; // 20ms at 16MHz / 1024
  TIMSK0 = (1 << OCIE0A);
}

int main(void) {
  init_pins();
  init_interrupt();
  init_timer1();
  init_timer0();

  while (1) {
  }
}

ISR(TIMER0_COMPA_vect) {
  if (increasing) {
    duty_cycle += STEP;
    if (duty_cycle >= TOP) {
      duty_cycle = TOP;
      increasing = false;
    }
  } else {
    duty_cycle -= STEP;
    if (duty_cycle <= 0) {
      duty_cycle = 0;
      increasing = true;
    }
  }
  OCR1A = duty_cycle; // Update LED brightness
}