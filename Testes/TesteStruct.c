#include <stdio.h>

struct player {
    char partida[15][15];
    unsigned long time;
    unsigned score;
    unsigned shots;
};

void setTime (struct player *point, int a) {

    point->time = -137;

}

void setMatch (struct player *point) {

    point->partida[0][0] = 9;

}

int main (void) {

    struct player player1, player2;

    printf ("%d\t", player1.partida[0][0]);
    printf ("%d\n", player1.time);

    setTime (&player1, 0);
    setMatch (&player1);

    printf ("%d\t", player1.partida[0][0]);
    printf ("%d\n", player1.time);

    return 0;
}
