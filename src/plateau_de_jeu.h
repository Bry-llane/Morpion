#ifndef PLATEAU_DE_JEU_H
#define PLATEAU_DE_JEU_H

#include <stdbool.h>

///====================Piont==============================

//typedef unsigned int piont;

///====================Board_game=========================

typedef struct board {
    int ligne;
    int colonne;
    int nbPiontWin;
    int **pl;
    int nbJoueur;
    int pos;
} *board;

///====================Fonctions==========================

void clear_screen();
board CreateBoard (int ligne, int colonne, int nbPiontWin, int joueur, int tour);
void PrintBoardBis(board b);
bool TestEmpty (board b, int lig, int col);
int RecupPiont (board b, int lig, int col);
bool PutPiont (board b, int p, int lig, int col);
void gotoxy(int x, int y);
int* UnCoup (board b, int p);
int* Coup (board b, int p);
int* MachineJoue (board b, int p);

#endif
