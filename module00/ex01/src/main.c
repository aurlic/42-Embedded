#include <avr/io.h>

int main(void) {
	// PORTB est le registre de sortie pour le port B.
	// En mettant un bit à 1 dans ce registre, on met la broche correspondante à l'état HIGH (5V).
	// (1 << PORTB0) décale le bit 1 vers la position PORTB0, ce qui correspond à la broche PB0.
	// L'opérateur |= effectue une opération OU binaire, ce qui permet de mettre PB0 à HIGH sans affecter les autres broches.
	PORTB |= (1 << PB0);

	return 0;
}