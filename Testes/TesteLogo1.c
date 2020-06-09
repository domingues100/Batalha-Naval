#include <stdio.h>

void printBootScreen () {

		//Counting variabels
		short k, i, j;

		//Buffer
		char cBuffer;

		//Bootscreen text constants
		long long btScr[2][5] = {{0xFC3CFF3CC0C33C, 0xC3C318C3C0C3C3, 0xFCC318C3C0FFC3, 0xC3FF18FFC0C3FF, 0xFCC318C3FFC3C3},
								 						 {0x00C33CC33CC000, 0x00F3C3C3C3C000, 0x00CFC3C3C3C000, 0x00C3FF66FFC000, 0x00C3C318C3FF00}};

		//To each bootscreen text row
		for (k = 0; k < 2; k++) {
				//To each terminal line
				for (i = 0; i < 5; i++) {
						//To each column
						for (j = -1 + sizeof(long long) * 7; j >= 0; j--) {
								//Space between Letters
								if ((j + 1) % 8 == 0)
										printf ("  ");

								//Isolates the j_th bit on the variable brScr[k][i]
								cBuffer = (btScr[k][i] >> j) % 2;

								//Prints a char 219 case it is set and a char 255 case its not
								printf ("%c", cBuffer ? 219 : 255);
						}
						//Line feed to each cycle
						printf ("\n");
				}
				//Space between bootscreen rows
				printf ("\n");
		}
}

int main () {

		printBootScreen();

		return 0;
}
