#include <stdio.h>
#include <stdlib.h>

#include "quintuplets.h"

///=========================================================
//Test l'existence d'un quintuplet

bool ExisteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    for(int i = 0; i < b->nbPiontWin; i++){
        if(lig < 0 || lig >= b->ligne || col < 0 || col >= b->colonne) return false;
        lig += Ligdirec;
        col += Coldirec;
    }

    return true;
}

///=======================????==================================
//Somme des pionts d'un quintuplet

int SommeQuintu (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int sum = 0;

    if (!(ExisteQuintu (b, lig, col, Ligdirec, Coldirec))) {
        return -1;
    }

    for(int i = 0; i < b->nbPiontWin; i++){
        sum += b->pl[lig][col];

        lig += Ligdirec;
        col += Coldirec;
    }
    return sum;
}

///=========================================================
//Note d'un quintuplet

int NoteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int note = 1;
    int sumQ = SommeQuintu (b, lig, col, Ligdirec, Coldirec);

    if (sumQ == 0) {
        return 1;
    } else if (sumQ > b->nbPiontWin && (sumQ%(b->nbPiontWin+1)) != 0){
        return 0;
    } else if (sumQ == -1) {
        return 0;
    }

    if (sumQ <= b->nbPiontWin) {
        note += 4;
        for(int i = 1; i < sumQ; i++){
            note *= 10;
        }
    } else if (sumQ > b->nbPiontWin && (sumQ%(b->nbPiontWin+1)) == 0) {
        int nb = sumQ/(b->nbPiontWin+1);
        note += 9;
        for(int i = 1; i < nb; i++){
            note *= 10;
        }
    }

    return note;
}

///=========================================================
//Nombre de piont

int NbPiont (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int sumQ = SommeQuintu (b, lig, col, Ligdirec, Coldirec);

    if (sumQ == 0) {
        return 0;
    } else if (sumQ > b->nbPiontWin && (sumQ%(b->nbPiontWin+1)) != 0){
        return -1;
    } else if (sumQ == -1) {
        return -1;
    }

    if (sumQ <= b->nbPiontWin) {
        if(sumQ == b->nbPiontWin){
            return b->nbPiontWin;
        } else {
            return sumQ;
        }
    } else if (sumQ > b->nbPiontWin) {
        if(sumQ/(b->nbPiontWin+1) == b->nbPiontWin){
            return b->nbPiontWin;
        } else {
            return sumQ/(b->nbPiontWin+1);
        }
        return b->nbPiontWin;
    }
}

///=========================================================
//Note dans une direction

int NoteDir (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int note = 0;
    int A = lig - (Ligdirec * (b->nbPiontWin-1));
    int B = col - (Coldirec * (b->nbPiontWin-1));

    for (int i = 0; i < b->nbPiontWin; i++) {
        note += NoteQuintu (b, A, B, Ligdirec, Coldirec);
        A+=Ligdirec;
        B+=Coldirec;
    }

    return note;
}

///=========================================================
//Note d'un carreau

int NoteCarreau (board b, int lig, int col)
{
    int diagGau = NoteDir (b, lig, col, 1, 1);
    int diagDro = NoteDir (b, lig, col, 1, -1);
    int horizon = NoteDir (b, lig, col, 1, 0);
    int vertica = NoteDir (b, lig, col, 0, 1);

    int note = diagGau + diagDro + horizon + vertica;
    return note;
}

///=========================================================
//Le carreau ayant la plus forte note

int* MeilleureNote (board b)
{
    int* carreau = (int*) malloc(2*sizeof(int));
    carreau[0] = -1;
    carreau[1] = -1;
    int bestNote = -1;
    int p;

    for(int i = 0; i < b->ligne; i++){
        for(int j = 0; j < b->colonne; j++){
            if(b->pl[i][j] == 0){
                p = NoteCarreau (b, i, j);
                if(p > bestNote){
                    bestNote = p;
                    carreau[0] = i;
                    carreau[1] = j;
                }
            }
        }
    }

    return carreau;
}

///=========================================================
//S'il y a un quintuplet gagnant dans une direction

bool WinDir (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int A = lig - (Ligdirec * (b->nbPiontWin-1));
    int B = col - (Coldirec * (b->nbPiontWin-1));

    for (int i = 0; i < b->nbPiontWin; i++) {
        if (NbPiont (b, A, B, Ligdirec, Coldirec) == b->nbPiontWin) return true;
        A+=Ligdirec;
        B+=Coldirec;
    }

    return false;
}

///=========================================================
//Pour detecter si un coup est gagnant

bool HaveWin(board b, int lig, int col)
{
    bool diagGau = WinDir (b, lig, col, 1, 1);
    bool diagDro = WinDir (b, lig, col, 1, -1);
    bool horizon = WinDir (b, lig, col, 1, 0);
    bool vertica = WinDir (b, lig, col, 0, 1);

    bool win = diagGau || diagDro || horizon || vertica;

    return win;
}


///=========================================================
