#ifndef PLATEAU_DE_JEU_H
#define PLATEAU_DE_JEU_H

#include <stdbool.h>

///====================Pion==============================

typedef unsigned int pion;

///====================Board_game=========================

typedef struct board {
    int ligne;
    int colonne;
    int nbPionWin;
    pion **pl;
    int nbJoueur;
    int pos;
} *board;

///====================Fonctions==========================

void Clear_screen();
board CreateBoard (int ligne, int colonne, int nbPionWin, int joueur, int tour);
pion RecupPion (board b, int lig, int col);
bool TestEmpty (board b, int lig, int col);
bool PutPion (board b, pion p, int lig, int col);
void FreeBoard(board B);
void PrintBoardBis(board b);

bool ExisteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int SommeQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int NbPion (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteDir (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteCarreau (board b, int lig, int col);
bool WinDir (board b, int lig, int col, int Ligdirec, int Coldirec);
bool HaveWin(board b, int lig, int col);
int* MeilleureNote (board b);
int* UnCoup (board b, pion p);
int* MachineJoue (board b, pion p);

#endif
