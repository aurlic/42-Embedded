#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define UBRR_VAL ((16000000 / (8UL * 115200)) - 1UL)
#define BACKSPACE 127
#define ENTER 13
#define BUFF_MAX 32

const char *user = "aurlic";
const char *password = "sup3rsafepass!";

char input_user[BUFF_MAX];
char input_pass[BUFF_MAX];
uint8_t input_index = 0;
/*
0: prompt username
1: entering username
2: prompt password
3: entering password
4: validate
*/
uint8_t state = 0;

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

static void uart_init() {
  UCSR0A |= (1 << U2X0);

  UBRR0H = (unsigned char)(UBRR_VAL >> 8);
  UBRR0L = (unsigned char)UBRR_VAL;

  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

static void blink_leds() {
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

  PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB4));

  while (1) {
    PORTB |= (1 << PORTB0);
    _delay_ms(50);
    PORTB &= ~(1 << PORTB0);

    PORTB |= (1 << PORTB1);
    _delay_ms(50);
    PORTB &= ~(1 << PORTB1);

    PORTB |= (1 << PORTB2);
    _delay_ms(50);
    PORTB &= ~(1 << PORTB2);

    PORTB |= (1 << PORTB4);
    _delay_ms(50);
    PORTB &= ~(1 << PORTB4);
  }
}

int main(void) {
  uart_init();
  sei();

  state = 0;

  while (1) {
    switch (state) {
    case 0:
      uart_printstr("Enter your login:\n\r");
      uart_printstr("\tUsername: ");
      memset(input_user, 0, sizeof(input_user));
      input_index = 0;
      state = 1;
      break;

    case 2:
      uart_printstr("\n\r\tPassword: ");
      memset(input_user, 0, sizeof(input_user));
      input_index = 0;
      state = 3;
      break;

    case 4:
      if (strcmp(input_user, user) == 0 && strcmp(input_pass, password) == 0) {
        uart_printstr("\n\rHello aurlic!\n\rShall we play a game?");
        blink_leds();
        return (0);
      } else {
        uart_printstr("\n\rBad combinaison username/password\n\n\r");
      }
      state = 0;
      break;
    }

    _delay_ms(50);
  }
}

ISR(USART_RX_vect) {
  uint8_t data = UDR0;

  if (state == 1) {
    if (data == ENTER) {
      input_user[input_index] = '\0';
      state = 2;
    } else if (data == BACKSPACE && input_index > 0) {
      input_index--;
      uart_printstr("\b \b");
    } else if (input_index < sizeof(input_user) - 1 && data != BACKSPACE) {
      input_user[input_index++] = data;
      uart_tx(data);
    }
  } else if (state == 3) {
    if (data == ENTER) {
      input_pass[input_index] = '\0';
      state = 4;
    } else if (data == BACKSPACE && input_index > 0) {
      input_index--;
      uart_printstr("\b \b");
    } else if (input_index < sizeof(input_pass) - 1 && data != BACKSPACE) {
      input_pass[input_index++] = data;
      uart_tx('*');
    }
  }
}