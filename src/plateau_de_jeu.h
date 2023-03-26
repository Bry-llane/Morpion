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

void clear_screen();
board CreateBoard (int ligne, int colonne, int nbPionWin, int joueur, int tour);
void PrintBoardBis(board b);
bool TestEmpty (board b, int lig, int col);
pion RecupPion (board b, int lig, int col);
bool PutPion (board b, pion p, int lig, int col);
int* UnCoup (board b, pion p);
int* MachineJoue (board b, pion p);

bool ExisteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int SommeQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec);
int NbPiont (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteDir (board b, int lig, int col, int Ligdirec, int Coldirec);
int NoteCarreau (board b, int lig, int col);
int* MeilleureNote (board b);
bool WinDir (board b, int lig, int col, int Ligdirec, int Coldirec);
bool HaveWin(board b, int lig, int col);


#endif
