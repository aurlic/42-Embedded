#include <avr/interrupt.h>
#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>

#define UBRR_VAL ((16000000 / (8UL * 115200)) - 1UL)
#define BACKSPACE 127
#define ENTER 13
#define BUFF_MAX 32

char input_user[BUFF_MAX];
uint8_t input_index = 0;
uint8_t new_data = 0;

static void uart_init() {
  UCSR0A |= (1 << U2X0);

  UBRR0H = (unsigned char)(UBRR_VAL >> 8);
  UBRR0L = (unsigned char)UBRR_VAL;

  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

static void uart_tx(unsigned char data) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;

  UDR0 = data;
}

unsigned char uart_rx() {
  while (!(UCSR0A & (1 << RXC0)))
    ;

  return UDR0;
}

static void uart_printstr(const char *s) {
  while (*s > 0)
    uart_tx(*s++);
}

void init_rgb() {
  TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
  TCCR0B |= (1 << CS00);
  TCCR2A |= (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
  TCCR2B |= (1 << CS20);

  OCR0A = 0;
  OCR0B = 0;
  OCR2B = 0;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
  OCR0A = g;
  OCR0B = r;
  OCR2B = b;
}

uint8_t hex_char_to_int(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return 255;
}

uint8_t parse_hex_color(const char *hex, uint8_t *r, uint8_t *g, uint8_t *b) {
  if (hex[0] == '#')
    hex++;
  if (strlen(hex) > 6)
    return 0;
  for (uint8_t i = 0; i < 6; i++)
    if (!isxdigit(hex[i]))
      return 0;

  *r = (hex_char_to_int(hex[0]) << 4) | hex_char_to_int(hex[1]);
  *g = (hex_char_to_int(hex[2]) << 4) | hex_char_to_int(hex[3]);
  *b = (hex_char_to_int(hex[4]) << 4) | hex_char_to_int(hex[5]);

  return 1;
}

int main(void) {
  DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
  uart_init();
  init_rgb();
  sei();

  uart_printstr("Enter HEX color (#RRGGBB or RRGGBB):\n\r");

  while (1) {
    if (new_data) {
      uint8_t r, g, b;
      if (parse_hex_color(input_user, &r, &g, &b)) {
        set_rgb(r, g, b);
        uart_printstr(
            "\n\rColor set!\n\rEnter HEX color (#RRGGBB or RRGGBB):\n\r");
      } else {
        uart_printstr("\n\rInvalid HEX. Try again:\n\r");
      }
      new_data = 0;
    }
    _delay_ms(50);
  }
}

ISR(USART_RX_vect) {
  char c = UDR0;

  if (c == ENTER) {
    input_user[input_index] = '\0';
    new_data = 1;
    input_index = 0;
  } else if (input_index < BUFF_MAX - 1) {
    input_user[input_index++] = c;
    uart_tx(c);
  }
}