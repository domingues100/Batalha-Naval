#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 16
//int SIZE = 16;

struct player {
	char match[SIZE][SIZE];
	unsigned long time;
	int score;
};

struct player player1, player2;

void printMatch (struct player *point);

char generateAircraft (struct player *point, short x, short y) {

		if (x > 13 || x < 1 || y > 11)
				return 0;

		short i, j;

		for (j = y; j < y + 5; j++)
				point->match[x][j] = 'P';

		for (j = y + 1; j < y + 4; j += 2)
				for (i = x - 1; i < x + 3; i++)
						point->match[i][j] = 'P';

		return 1;
}

char generateHidro (struct player *point, short x, short y) {

		short i, j;

		if (x < 2 || x > 13 || y > 12)
				printf ("ERRADO\n");

 		for (j = y; j < y + 4; j++)
				point->match[x][j] = 'H';

		for (j = y + 1 + (rand() % 2), i = x - 2; i < x + 3; i++)
				point->match[i][j] = 'H';

		return 1;
}

void randMatch (struct player *point) {

		srand(time(NULL));

		short i, j;

		//Resets all the positions in the matriz
		for (i = 0; i < SIZE; i++)
				for (j = 0; j < SIZE; j++)
						point->match[i][j] = 0;

		//Randomizes 10 coordenates
		short coord_x[10], coord_y[10];
		for (i = 0; i < 10; i++) {
			  coord_x[i] = (rand() % SIZE);
			  coord_y[i] = (rand() % SIZE);
		}

		short x, y;
		for (i = 0; i < 1; i++) {
				x = coord_x[i];
				y = coord_y[i];

				//generateAircraft (&player1, x, y);
				generateHidro (&player1, x, y);
		}
}

int main (void) {

		//¹-251  ²-253
		//struct player player1, player2;

		randMatch (&player1);
		printMatch (&player1);

		//printf ("    Tempo de Partida: 01:02:37\n\n");

		return 0;
}

void printMatch (struct player *point) {

	short i, j;

	//To each row i
	for (i = 0; i < SIZE; i++) {

		//Only happens int the first line
		if (i == 0) {
			//Prints the letters above the columns
			printf ("   ");
			for (j = 0; j < SIZE; j++)
				printf ("   %c", 65 + j);

			//Prints the first border
			printf ("\n    %c", 201);
			for (j = 0; j < SIZE; j++)
				printf ("%c%c%c%c", 205, 205, 205, j == SIZE - 1 ? 187 : 203);
		}

		//Print the middle of the line
		printf ("\n%s%d %c", i >= 9 ? " " : "  ", i + 1, 186);
		unsigned char buffer;
		for (j = 0; j < SIZE; j++) {

				buffer = point->match[i][j];
				buffer == 0 ? 255 : 65;

				printf (" %c%c%c", buffer, 255, 186);
		}
		//Tiro 254, 158

		if (i == SIZE - 1) {
			//Prints the last border
			printf ("\n    %c", 200);
			for (j = 0; j < SIZE; j++)
				printf ("%c%c%c%c", 205, 205, 205, j == SIZE - 1 ? 188 : 202);
		}
		else {
			//Other line borders
			printf ("\n    %c", 204);
			for (j = 0; j < SIZE; j++)
				printf ("%c%c%c%c", 205, 205, 205, j == SIZE - 1 ? 185 : 206);
		}
	}
	printf ("\n");
}
