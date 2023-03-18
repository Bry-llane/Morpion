#ifndef QUINTUPLETS_H
#define QUINTUPLETS_H

#include <stdbool.h>
#include "plateau_de_jeu.h"

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
