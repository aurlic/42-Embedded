#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void init_rgb() {
  // Configure Timer1 for fast PWM 8bit mode (cf. page 142)
  TCCR1B |= (1 << WGM12);
  TCCR1A |= (1 << WGM10);

  // Sets the total signal period
  ICR1 = 255;

  // Configure Timer0 for Fast PWM and set OCOB for up couting (cf. page 115)
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  // Set no prescalling
  TCCR0B |= (1 << CS00);
  // Configure Timer2 for Fast PWM (cf. page 163)
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  // Set no prescalling
  TCCR2B |= (1 << CS20);

  // Set bottoms
  OCR0A = 0;
  OCR0B = 0;
  OCR2B = 0;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
  OCR0A = g;
  OCR0B = r;
  OCR2B = b;
}

void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    set_rgb(255 - pos * 3, 0, pos * 3);
  } else if (pos < 170) {
    pos = pos - 85;
    set_rgb(0, pos * 3, 255 - pos * 3);
  } else {
    pos = pos - 170;
    set_rgb(pos * 3, 255 - pos * 3, 0);
  }
}

int main(void) {
  // Set LEDs as output
  DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
  init_rgb();

  while (1) {
    for (uint8_t pos = 0; pos <= 255; pos++) {
      wheel(pos);
      _delay_ms(50);
    }
  }
}