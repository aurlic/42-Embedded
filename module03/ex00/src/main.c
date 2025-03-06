#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Enum containing all the colors we want to loop through
typedef enum { RED, GREEN, BLUE, NUM_COLORS } Color;

const uint8_t color_code[] = {(1 << DDD5), (1 << DDD6), (1 << DDD3)};

static void display_color(Color color) {
  // Reset color
  PORTD = 0;

  // Flash correct color
  PORTD = color_code[color];
}

int main(void) {
  // Set LEDs as output
  DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);

  while (1) {
    for (Color color = RED; color < NUM_COLORS; color++) {
      display_color(color);
      _delay_ms(1000);
    }
  }
}