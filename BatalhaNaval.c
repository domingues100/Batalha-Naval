/*
  AUTHOR(S): DOMINGUES, G.M.;
             TEMPESTA,  H.H.;

  2020, Projeto Batalha Naval
	SSC0304 USP São Carlos
*/

/*---- LIBRARIES ----*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*---- MACROS ----*/
#define SIZE        16
#define EMPTY_CELL  255
#define SHOT_CELL   158
#define SUCCESS     0
#define ERROR       1
#define RESET       -1
#define BOOTSCREEN  0
#define HELPMENU    2
#define ENDGAME     3
#define SURRENDER   4

/*---- GLOBAL VARIABLES ----*/
int show_ships;

/*---- STRUCTS ----*/
struct player {
	unsigned char match[SIZE][SIZE];
	unsigned remaining[5];
	unsigned long time;
	unsigned score;
	unsigned id;
};

time_t timer0;

/*---- FUNCTION PROTOTYPES ----*/
struct player setPlayer (int id);                                                        //Fills the starting data to each player
short aux_getFunction (char *str);                                                       //Identifies a string and return a correspondent value
void  aux_lowerCaseOnly (char *str);                                                     //Turns a alphabetic string into its lowercase only version
short aux_optionMenu (char str[]);                                                       //Creates a simples Yes/No menu with a given question
void  aux_printMatchBorder (char chr0, char chr1, char chr2);                            //(printMatch - aux) Prints the borders
void  endGame (struct player *winner);                                                   //Ending screen
void  homeScreen (void);                                                                 //Starting screen
short gameMain (struct player *curPlayer, struct player *stdbyPlayer);                   //Calls out all the game functionalities
short gameFunc_Acaso (struct player *playerA, struct player *playerB);                   //Replace all the ships, resets the shots, but keeps score and time
short gameFunc_Ajuda (struct player *playerA);                                           //Displays a menu with information on the whole game
short gameFunc_Carregar (struct player *playerA, struct player *playerB);                //Loads a saved game from a given external file
short gameFunc_Gravar (struct player *playerA, struct player *playerB, char printAfter); //Save the current state of game in a external given file
short gameFunc_Pow (struct player *playerA);                                             //"Shots" in a given position
short gameFunc_Reset (struct player *playerA, struct player *playerB);                   //Starts a whole new game
short gameFunc_Revelar (struct player *playerA);                                         //CHEAT - Reveals or hides the ships positions
short gameFunc_Sair (struct player *playerA, struct player *playerB);                    //Exit the game
short gameFunc_Surrender (struct player *playerA, struct player *playerB);               //Surrender and opponent wins
short gameFunc_Win (struct player *playerA);                                             //CHEAT - Makes a player instantly win a game
short genWatership (struct player *playerA, char *blueprint, char content);              //(randMatch - aux) Puts a given kind of ship into a random position
void  printLogo (int typeLogo);                                                          //Displays a given custom logotype
void  printMatch (struct player *playerA);                                               //Displays the game board of a given player
void  randMatch (struct player *playerA);                                                //Randomizes the game board of a given player

/*---- GAME  ----*/
int main (void) {

	struct player player[2];
	srand (time (NULL));

	homeScreen ();

	short main_feedback, //main_feedback, will receive the returned value by game_main witch will then be interpreted
	      turn;          //turn, oscillates between 0 and 1, defining which player plays on that round

	//Main loop
	do {
		//Inserts all starting data into each struct
		player[0] = setPlayer (1);
		player[1] = setPlayer (2);

		main_feedback = SUCCESS; //main_feedback starts as SUCCESS by default
		turn          = 0;       //the player1 always plays first
		show_ships    = 0;       //By default, a match always starts with hidden ships

		timer0 = clock();        //Sets timer0 as the current time, right before the match stars, it'll be used as a time reference

		//A match continues while the opponent of the current player haven't won and if noboby asked for reset
		while (main_feedback != RESET && player[!turn].remaining[0] > 0) {

			printMatch (&player[turn]); //The board is first printed on the screen

			//A turn continues while the player didn't shoot yet or if nobody asked for reset
			do main_feedback = gameMain (&player[turn], &player[!turn]);
			while (main_feedback == ERROR);

			if (main_feedback != SURRENDER) //if the last player didn't surrender
			  turn = !turn;                 //At the end, the turn switches, meaning is time to the other player to play
		}

		//if the match was ended by a reset, it starts again
		//in case one of the players actually won, the loop is broken
	} while (main_feedback == RESET);

	endGame (&player [!turn]);

	return 0;
}

struct player setPlayer (int id) {

	struct player buffer;

	randMatch (&buffer);
	buffer.id    = id;
	buffer.score = 0;
	buffer.time  = 0;

	unsigned k;
	const int content[5] = {84, 11, 20, 21, 32};
	for (k = 0; k < 5; k++)
		buffer.remaining[k] = content[k];

	return buffer;
}

short aux_getFunction (char *str) {
	short k;
	const char functionTitle[][10] = {"pow", "ajuda", "reset", "sair", "acaso", "gravar", "carregar", "revelar", "win", "desistir"};

	aux_lowerCaseOnly (str);

	for (k = 0; k < 10 && strcmp (str, functionTitle[k]); k++);

	return k;
}

void  aux_lowerCaseOnly (char *str) {
	short k;
	for (k = 0; k < strlen(str); k++)
		if (str[k] >= 'A' && str[k] <= 'Z')
			str[k] += 32;
}

short aux_optionMenu (char str[]) {
	while (1) {
		printf ("    -%s? [S/N] ", str);
		fflush (stdin);
		char buffer = getchar();

		if (buffer == 'S' || buffer == 's') return 1;
		else if (buffer == 'N' || buffer == 'n') return 0;
		else printf ("    -Entrada inv%clida!\n", 160);
	}
}

void  aux_printMatchBorder (char chr0, char chr1, char chr2) {
	short j;
	printf ("\n    %c", chr0);
	for (j = 0; j < SIZE; j++)
		printf ("%c%c%c%c", 205, 205, 205, (j == SIZE - 1) ? chr1 : chr2);
}

void  aux_waitForEnter (char str[]) {
	printf ("\n    Pressione ENTER para %s", str);
	fflush (stdin); //Clears the keyboard bufffer;
	getchar (); //Wais for the user to press
}

void  endGame (struct player *winner) {

	system ("cls");
	printLogo (ENDGAME);

	printf ("\n\t    O JOGADOR %d venceu a partida!\n", winner->id);
	printf ("\t\t  Pontua%c%co: %6d\n", 135, 198, winner->score);

	long long unsigned time = difftime (clock(), timer0) / 1000;
	unsigned char sec = time % 60,
	              min = time / 60,
	              hor = time / 3600;

	printf ("\n\t      Tempo de Partida: %02d:%02d:%02d\n\n\n", hor, min, sec);

	fflush (stdin);
	getchar ();
}

void  homeScreen (void) {

	int recognized = 1;
	char user[10];

	do {
		printLogo (BOOTSCREEN);

		if (recognized == 0)
			printf ("\tComando n%co reconhecido...\n\n", 198);

		printf ("\t---> ");

		scanf ("%s", user);
		fflush (stdin);

		aux_lowerCaseOnly (user);

		if (recognized = !strcmp (user, "ajuda"))
			gameFunc_Ajuda (NULL);

		system ("cls");

	} while (strcmp (user, "iniciar"));
}

short gameMain (struct player *curPlayer, struct player *stdbyPlayer) {

	printf ("\n    Bombardeio de player %d: ", curPlayer->id);

	char func[10];
	fflush (stdin);
	scanf ("%s", func);

	//Original Functions
	switch (aux_getFunction (func)) {
		case 0 : return gameFunc_Pow (curPlayer);                    break;
		case 1 : return gameFunc_Ajuda (curPlayer);                  break;
		case 2 : return gameFunc_Reset (curPlayer, stdbyPlayer);     break;
		case 3 : return gameFunc_Sair (curPlayer, stdbyPlayer);      break;
		case 4 : return gameFunc_Acaso (curPlayer, stdbyPlayer);     break;
		case 5 : return gameFunc_Gravar (curPlayer, stdbyPlayer, 1); break;
		case 6 : return gameFunc_Carregar (curPlayer, stdbyPlayer);  break;
		case 7 : return gameFunc_Revelar (curPlayer);                break;
    		case 8 : return gameFunc_Win (curPlayer);                    break;
    		case 9 : return gameFunc_Surrender (curPlayer, stdbyPlayer); break;

		//Function wasnt recognized
		default : {
			printf ("     -Comando n%co reconhecido... ", 198);
			return ERROR;
			break;
		}
	}
}

short gameFunc_Acaso (struct player *playerA, struct player *playerB) {

	if (aux_optionMenu ("Deseja realmente reposicionar os barcos")) {
		randMatch (playerA);
		randMatch (playerB);

		printMatch (playerA);

		printf ("\n    -Os barcos foram reposicionados!");
		aux_waitForEnter ("finalizar... ");
	}

	printMatch (playerA);

	return 1;
}

short gameFunc_Ajuda (struct player *playerA) {

	int loop;

	do {
		system ("cls");
		char inp;
		printf ("\n");
		printLogo (HELPMENU);
		printf("  1-Jogo\n  2-Comandos\n  3-Navios\n  4-Retornar\n\n  Digite o n%cmero correspondente a uma das op%c%ces: ", 163, 135, 228);

		fflush (stdin);
		scanf("%c", &inp);

		switch (inp) {
			case '1' : {//jogo
		    		printf("\n O jogo %c constitu%cdo de 2 tabuleiros gerados de maneira aleat%cria e o objetivo %c acertar todas as embarca%c%ces do oponente para vencer.\n", 130, 161, 162, 130, 135, 228);
				printf(" Para informa%c%ces sobre os formatos dos barcos, v%c em ajuda e depois em embarca%c%ces. E para ver a lista de comandos, v%c em ajuda e depois em comandos.\n", 135, 228, 160, 135, 228, 160);
		    		aux_waitForEnter ("voltar... ");
				loop = 1;
				break;
			}

			case '2' : {
				printf("\n  -POW:      Faz o bombardeio em uma posi%c%ces. Ex: pow 2H.\n", 135, 228);
			 	printf("  -ACASO:    Muda as posi%c%ces dos barcos, mas mant%cm as pontua%c%ces.\n",135, 228, 130, 135, 228);
				printf("  -AJUDA:    Concede informa%c%ces ao jogador a qualquer momento da partida\n", 135, 228);
				printf("  -RESET:    Inicia uma nova partida, apagando todos os dados da atual.\n");
				printf("  -GRAVAR:   Armazena a partida em andamento, permitindo continuar depois.\n");
				printf("  -CARREGAR: Recarrega uma partida salva.\n");
				printf("  -REVELAR:  Revela ou oculta as posi%c%ces de todas as embarca%c%ces.\n", 135, 228, 135, 228);
				printf("  -SAIR:     Encerra o jogo.\n");
				printf("  -DESISTIR: Abandona a partida, concedendo a vit%cria ao advers%crio.\n", 162, 160);
				aux_waitForEnter ("voltar... ");
				loop = 1;
				break;
			}

			case '3': { //Explains and prints all the ships
				printf("\n  Porta-avi%ces:   Coura%cados:   Torpedeiros:   Hidroavi%ces:\n", 228, 135, 228);
				printf("      P   P          C           T T T T T          H\n");
				printf("    P P P P P      C C C C         T   T        H H H H H\n");
				printf("      P   P        C C C C C                        H\n");
				printf("      P   P                                         H\n\n");
				printf("  O jogo cont%cm 1 Porta-avi%ces, 2 Coura%cados, 3 Torpedeiros e 4 Hidroavi%ces\n",130, 228, 135, 228);
				aux_waitForEnter ("voltar... ");
				loop = 1;
				break;
		  	}

			case '4': {
				loop = 0;
				aux_waitForEnter ("sair... ");
				break; // sair
			}

			default : {
				printf ("     -Comando n%co reconhecido...\n", 198);
				aux_waitForEnter ("seguir ");
				loop = 1;
				break;
			}
		}
	}
	while (loop);

	if (playerA != NULL)
		printMatch (playerA);

	return 1;
}

short gameFunc_Carregar (struct player *playerA, struct player *playerB) {

	printf ("    -Digite o nome do arquivo: ");

	char file_name[50];
	char playerEnd;

	fflush (stdin);
	gets(file_name);
	strcat(file_name, ".txt");

	FILE *arq;
	arq = fopen (file_name, "r");

	if (arq == NULL)
		printf ("     -Nao foi poss%cvel encontrar o arquivo... ", 161);

	else {
		fread (playerA->id == 1 ? playerA : playerB, sizeof(struct player), 1, arq);
		fscanf (arq, "%c", &playerEnd);

		fread (playerA->id == 2 ? playerA : playerB, sizeof(struct player), 1, arq);
		fscanf (arq, "%c", &playerEnd);

		fclose (arq);

		printMatch (playerA);
		printf ("\n    -Arquivo foi aberto com sucesso!\n");
	}

	aux_waitForEnter ("para continuar... ");
	printMatch (playerA);

	return ERROR;
}

short gameFunc_Gravar (struct player *playerA, struct player *playerB, char printAfter) {

	char file_name[32];
	time_t now;
	now = time (NULL);

  	//Saves current time in this format tabuleiro-yyyy.mm.dd_hhmmss.txt
	strftime (file_name, sizeof(file_name), "tabuleiro-%Y.%m.%d_%H%M%S.txt", localtime(&now));

	FILE *arq;
	arq = fopen (file_name, "w");

	if (arq == NULL)
		printf ("    -O arquivo n%co pode ser gravado...\n", 198);

	else {
		fwrite (playerA->id == 1 ? playerA : playerB, sizeof(struct player), 1, arq);
		fprintf (arq, "\n");

		fwrite (playerA->id == 2 ? playerA : playerB, sizeof(struct player), 1, arq);
		fprintf (arq, "\n");

		fclose (arq);

		printf ("    -O arquivo foi salvo com sucesso!\n");
	}

	if (printAfter == 1) {
		aux_waitForEnter ("para continuar... ");
		printMatch (playerA);
	}

	return ERROR;
}

short gameFunc_Pow (struct player *playerA) {

	int row, error = 0;
	unsigned char col;
	scanf(" %d%c", &row, &col);

	if ((--row) > 16) //Row offset, at program level, based on 0
		error = 1;

	if (col >= 'a' && col <= 'p')      col -= 97; //Checks for command in lowercase
	else if (col >= 'A' && col <= 'P') col -= 65; //Checks for command in uppercase
	else                               error = 2; //Command syntax not recognized

	if (error > 0) { //Checks if the input data is valid
		printf ("     -Entrada inv%clida!", 160);
		return ERROR;
	}

	unsigned char buffer = playerA->match[row][col]; //Creates a buffer with the pointed value

	if ((buffer >= 'A' && buffer <= 'Z') || (buffer == SHOT_CELL)) { //Checks if the pointed position wasnt shot already
		printf ("     -Voc%c j%c atirou nesse lugar!", 136, 160);
		return ERROR;
	}

	int scoreMod;	//Variable that will store the score modifier

	if (buffer >= 'a' && buffer <= 'z') {     //if the position contains a ship
		playerA->match[row][col] -= 32;       //uptade value from lowercase to uppercase
		playerA->remaining[0] -= 1;           //uptades remaining shots to player to win

		const int bloksOcp[] = {11, 10, 7, 8};	//Cosntant, stores the amount of blocks each ship takes
		short tShip; 					                  //Temporary buffer that stores the type of ship hit

		switch (buffer) {												//Checks for type of ship hit
			case 'p' : tShip = 0; break;
			case 'c' : tShip = 1; break;
			case 't' : tShip = 2; break;
			case 'h' : tShip = 3; break;
		}

		scoreMod = 1000 * (float)(bloksOcp[tShip] * playerA->remaining[tShip + 1])/(SIZE * SIZE); //Score increment

		playerA->remaining[tShip]--; //uptades remaining blcoks of that type of ship
	}

	else { //if the position in empty, uptade it with an 'x'
		playerA->match[row][col] = SHOT_CELL;

		//Score decrement
		scoreMod = -5 * (1.0 - (float)(playerA->remaining[0])/(SIZE * SIZE));

		//Satures player score at minimum 0
		if (playerA->score < abs(scoreMod)) scoreMod = 0;
	}

	playerA->score += scoreMod;	//Updates the player score

	printMatch (playerA); //Displays the shot feedback to the player

	aux_waitForEnter ("finalizar a jogada...");

	return SUCCESS;
}

short gameFunc_Reset (struct player *playerA, struct player *playerB) {

  	if (aux_optionMenu ("Deseja realmente resetar o jogo?"))
    		return RESET;

	return ERROR;
}

short gameFunc_Revelar (struct player *playerA) {

	int _show_ships = show_ships;

	if (show_ships == 0 && aux_optionMenu ("Deseja revelar os barcos")) show_ships = 1;

	else if (show_ships == 1 && aux_optionMenu ("Deseja ocultar os barcos")) show_ships = 0;

	if (_show_ships != show_ships) {
		printMatch (playerA);
		printf ("\n    -Os barcos foram %s!", show_ships == 1 ? "revelados" : "ocultados");
		aux_waitForEnter ("continuar... ");
	}

	printMatch (playerA);

	return 1;
}

short gameFunc_Sair (struct player *playerA, struct player *playerB) {

	if (aux_optionMenu ("Deseja realmente sair do jogo")) {
		if (aux_optionMenu ("Deseja salvar o jogo antes de sair"))
		  gameFunc_Gravar (playerA, playerB, 0);

		exit(1);
	}
	else {
		system ("cls");
		printMatch (playerA);
	}

	return ERROR;
}

short gameFunc_Surrender (struct player *playerA, struct player *playerB) {
  	if (aux_optionMenu ("Deseja realmente desistir")) {
	    	playerB->remaining[0] = 0;
   			return SURRENDER;
	}
	else {
		system ("cls");
		printMatch (playerA);
		return 1;
	}
}

short gameFunc_Win (struct player *playerA) {

  char password [10];

	printf ("    -Digite a senha pra utilizar essa fun%c%co: ", 135, 198);

	scanf("%s", password);
	fflush (stdin);

	aux_lowerCaseOnly (password);

	if (!strcmp (password, "ueyama")) {
		playerA->score = 999999;
		playerA->remaining[0] = 0;

		return 0;
  	}

	else {
		printf ("    -Senha incorreta!\n");
		aux_waitForEnter ("para voltar...");

		printMatch (playerA);

		return 1;
	}
}

short genWatership (struct player *playerA, char *blueprint, char content) {

	short x, y, height, i, j;

	height = strlen(blueprint) / 5;
	x = rand() % (SIZE + 1 - height); // Randomizes a position to place the ship
	y = rand() % (SIZE - 4);

	if (x > (SIZE - height)) //Checks for battleship 'hitbox'
		return 0;

	for (i = 0; i < height; i++) //Checks for clean path/area
		for (j = 0; j < 5; j++)
			if (blueprint[i * 5 + j] == '1' && playerA->match[x+i][y+j] != EMPTY_CELL)
				return 0;

	for (i = 0; i < height; i++) //Writes the ship in place
		for (j = 0; j < 5; j++)
			if (blueprint[i * 5 + j] == '1')
				playerA->match[x+i][y+j] = content;

	return 1;
}

void  printLogo (int typeLogo) {

	printf ("\n");

	short data_line, spaceID, space_feed;
	short k, i, j; //Counting variabels
	char cBuffer;  //Buffer

	const long long btScr[5][5] = { //Texts generation seeds
	{0xFE7EFF7EC0C37E, 0xC3C318C3C0C3C3, 0xFEC318C3C0FFC3, 0xC3FF18FFC0C3FF, 0xFEC318C3FFC3C3},  //BATALHA
	{0x00C37EC37EC000, 0x00F3C3C3C3C000, 0x00CFC3C3C3C000, 0x00C3FF66FFC000, 0x00C3C318C3FF00},  //NAVAL
	{0x7E0FC3FE7E0000, 0xC303C3C3C30000, 0xC303C3C3C30000, 0xFFC3C3C3FF0000, 0xC37E7EFEC30000},  //AJUDA
	{0xFFFFC30FEFF000, 0xC018E70C3C0000, 0xFC18FF0C3FF000, 0xC018DB0C3C0000, 0xC0FFC30FEFF000},  //FIM DE
	{0x00F7E7E7E00000, 0x003C3C0C300000, 0x003C3CFC300000, 0x0C3C3C3C300000, 0x07E7E7E7E00000}}; //JOGO

  	const char spaces[5][7] = {
	{55, 47, 39, 31, 23, 15, 7},  //BATALHA
	{55, 47, 39, 31, 23, 15, 7},  //NAVAL
	{55, 47, 39, 31, 23, 15, 0},  //AJUDA
	{55, 47, 39, 27, 19,  0, 0},  //FIM DE
	{55, 52, 43, 35, 27, 19, 0}}; //JOGO

	switch (typeLogo) {
		case BOOTSCREEN : spaceID = 0, data_line = 2; break;
		case HELPMENU   : spaceID = 2, data_line = 3; break;
		case ENDGAME    : spaceID = 3, data_line = 5; break;
	}

	for (k = typeLogo; k < data_line; k++, spaceID++) {                                     //To each bootscreen text row
		for (i = 0; i < 5; i++) {                                                       //To each terminal line
			for (space_feed = 0, j = -1 + sizeof (long long) * 7; j >= 0; j--) {    //To each column

				if (spaces[spaceID][space_feed] == j && j != 0) { //Space between Letters
					space_feed++;
					printf ("  ");
				}

				cBuffer = (btScr[k][i] >> j) % 2;   //Isolates the j_th bit on the variable brScr[k][i]
				printf ("%c", cBuffer ? 219 : 255); //Prints a char 219 case it is set and a char 255 case its not
			}
			printf ("\n"); //Line feed to each cycle
		}
		printf ("\n"); //Space between logo rows
	}

	if (typeLogo == BOOTSCREEN) {
		printf("                 By Gabriel Domingues & Hadler Tempesta");
		printf ("\n\n        Digite INICIAR para prosseguir ou AJUDA para informa%c%ces\n\n", 135, 228); //carregar etc
	}
}

void  printMatch (struct player *playerA) {

	system ("cls");

	short i, j;

	printf("\n\t\t      CAMPO DE BATALHA DO JOGADOR %c\n\n", (playerA->id == 1) ? '2' : '1');

	printf ("   "); //Prints the letters above the columns
	for (j = 0; j < SIZE; j++) printf ("   %c", 65 + j);

	aux_printMatchBorder (201, 187, 203); //Prints the top border

	for (i = 0; i < SIZE; i++) { //To each row i
		printf ("\n%s%d %c", i >= 9 ? " " : "  ", i + 1, 186); //Print the actual content
		for (j = 0; j < SIZE; j++) {

			unsigned char buffer1 = playerA->match[i][j], buffer0 = EMPTY_CELL;

			if (buffer1 >= 'a' && buffer1 <= 'z' && show_ships == 0)
				buffer1 = EMPTY_CELL;

			if (buffer1 >= 'A' && buffer1 <= 'Z')
				buffer0 = (playerA->id == 1 ? 253 : 251);

			printf ("%c%c%c%c", 255, buffer1, buffer0, 186);
		}

		if (i == SIZE - 1)
			aux_printMatchBorder (200, 188, 202); //Prints the last border
		else
			aux_printMatchBorder (204, 185, 206); //Other line borders
	}

	//------- TIME -------
	long long unsigned time = difftime (clock(), timer0) / 1000;

	unsigned char sec, min, hor;
	sec = time % 60;
	min = time / 60;
	hor = time / 3600;

	printf ("\n    Tempo de Partida: %02d:%02d:%02d", hor, min, sec);

	//------- SCORE -------
	i = playerA->score;
	j = 1;

	while (i /= 10) j++;
	for (i = 0; i < 16 - j; i++) printf (" ");

	printf ("Pontua%c%co do player %d: %d\n", 135, 198, playerA->id, playerA->score);
}

void  randMatch (struct player *playerA) {

	char 	//Ships blueprints
	aircraft[]   = "01010111110101001010",	//aircraft form, represented by P
	torped[]     = "1111101010",            //torped form, represented by T
	hidroplane[] = "00100111110010000100",  //hidroplane form, represented by H
	battleship[] = "010001111011111";       //battleship form, represented by B

	short i, j;

	for (i = 0; i < SIZE; i++) //Resets all the positions in the matriz
		for (j = 0; j < SIZE; j++)
			playerA->match[i][j] = EMPTY_CELL;

	//Loops to generate all the ships
	while (!genWatership (playerA, aircraft, 'p'));

	for (i = 0; i < 2; i++)
		while (!genWatership (playerA, battleship, 'c'));

	for (i = 0; i < 3; i++)
		while (!genWatership (playerA, torped, 't'));

	for (i = 0; i < 4; i++)
		while (!genWatership (playerA, hidroplane, 'h'));
}
