#ifndef PLATEAU_DE_JEU_H
#define PLATEAU_DE_JEU_H

#include <conio.h>
#include <stdbool.h>

///====================Piont==============================

typedef unsigned int piont;

///====================Board_game=========================

typedef struct board {
    int ligne;
    int colonne;
    int nbPiontWin;
    piont **pl;
    int nbJoueur;
    int pos;
} *board;

///====================Fonctions==========================

void clear_screen();
board CreateBoard (int ligne, int colonne, int nbPiontWin, int joueur, int tour);
void PrintBoardBis(board b);
bool TestEmpty (board b, int lig, int col);
piont RecupPiont (board b, int lig, int col);
bool PutPiont (board b, piont p, int lig, int col);
void gotoxy(int x, int y);
int* UnCoup (board b, piont p);
int* Coup (board b, piont p);
int* MachineJoue (board b, piont p);

#endif
