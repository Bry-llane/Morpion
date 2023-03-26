#include <stdio.h>
#include <stdlib.h>
#include "plateau_de_jeu.h"

///=======================================================

void clear_screen()
{
    system("clear");
}

///========================================================

board CreateBoard (int ligne, int colonne, int nbPionWin, int joueur, int tour)
{
    board B = (board)malloc(sizeof(struct board));

    pion **p = (pion **)malloc(ligne*sizeof(pion*));
    for (int i=0; i<ligne; i++){
        p[i] = (pion*)malloc(colonne*sizeof(pion));
    }

    for(int i = 0; i < ligne; i++){
        for(int j = 0; j < colonne; j++){
            p[i][j] = 0;
        }
    }

    B->ligne = ligne;
    B->colonne = colonne;
    B->nbPionWin = nbPionWin;
    B->pl = p;
    B->nbJoueur = joueur;
    B->pos = tour;

    return B;
}

///=========================================================
//print non numerote
void PrintBoardBis(board b)
{
    printf("+");
    for(int i = 0; i < b->colonne; i++){
        for(int x = 0; x < 3; x++){
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    for(int i = 0; i < b->ligne; i++){
        for(int j = 0; j < b->colonne; j++){
            if(b->pl[i][j] == 0){
                printf("|   ");
            } else if(b->pl[i][j] == 1){
                printf("| O ");
            } else if(b->pl[i][j] == b->nbPionWin+1){
                printf("| X ");
            }
        }
        printf("|\n");

        printf("+");
        for(int i = 0; i < b->colonne; i++){
            for(int x = 0; x < 3; x++){
                printf("-");
            }
            printf("+");
        }
        printf("\n");
    }
    printf("\n");
}

///=========================================================

bool TestEmpty (board b, int lig, int col)
{
    if(b->pl[lig][col] == 0) {
        return true;
    } else {
        return false;
    }
}

///=========================================================

pion RecupPion (board b, int lig, int col)
{
    pion p = b->pl[lig][col];

    return p;
}

///=========================================================

bool PutPion (board b, pion p, int lig, int col)
{
    if (TestEmpty(b, lig, col)){
        b->pl[lig][col] = p;
        return true;
    }
    return false;
}

///=========================================================

int* UnCoup (board b, pion p)
{
    int lig, col;
    while(true)
    {
        printf("Entrez la ligne : ");
        scanf("%d", &lig);
        printf("Entrez la colonne : ");
        scanf("%d", &col);
        lig = lig - 1;
        col = col - 1;

        if(TestEmpty (b, lig, col)) break;
        clear_screen();
        PrintBoardBis(b);
        printf("Erreur, rentrez une cordonnee libre\n");
    }

    PutPion (b, p, lig, col);
    int* cord = (int*) malloc(2*sizeof(int));
    cord[0] = lig;
    cord[1] = col;
    free(cord);
    return cord;
}

///=========================================================
//la machine joue
int* MachineJoue (board b, pion p)
{
    int* best = MeilleureNote(b);
    PutPion (b, p, best[0], best[1]);
    printf("La machine a joue en (%d; %d)\n", best[1]+1, best[0]+1);
    //sleep(1);
    return best;
}

///=========================================================
//Test l'existence d'un quintuplet

bool ExisteQuintu (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    for(int i = 0; i < b->nbPionWin; i++){
        if(lig < 0 || lig >= b->ligne || col < 0 || col >= b->colonne) return false;
        lig += Ligdirec;
        col += Coldirec;
    }

    return true;
}

///=======================????==================================
//Somme des pions d'un quintuplet

int SommeQuintu (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int sum = 0;

    if (!(ExisteQuintu (b, lig, col, Ligdirec, Coldirec))) {
        return -1;
    }

    for(int i = 0; i < b->nbPionWin; i++){
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
    } else if (sumQ > b->nbPionWin && (sumQ%(b->nbPionWin+1)) != 0){
        return 0;
    } else if (sumQ == -1) {
        return 0;
    }

    if (sumQ <= b->nbPionWin) {
        note += 4;
        for(int i = 1; i < sumQ; i++){
            note *= 10;
        }
    } else if (sumQ > b->nbPionWin && (sumQ%(b->nbPionWin+1)) == 0) {
        int nb = sumQ/(b->nbPionWin+1);
        note += 9;
        for(int i = 1; i < nb; i++){
            note *= 10;
        }
    }

    return note;
}

///=========================================================
//Nombre de pion

int NbPion (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int sumQ = SommeQuintu (b, lig, col, Ligdirec, Coldirec);

    if (sumQ == 0) {
        return 0;
    } else if (sumQ > b->nbPionWin && (sumQ%(b->nbPionWin+1)) != 0){
        return -1;
    } else if (sumQ == -1) {
        return -1;
    }

    if (sumQ <= b->nbPionWin) {
        if(sumQ == b->nbPionWin){
            return b->nbPionWin;
        } else {
            return sumQ;
        }
    } else if (sumQ > b->nbPionWin) {
        if(sumQ/(b->nbPionWin+1) == b->nbPionWin){
            return b->nbPionWin;
        } else {
            return sumQ/(b->nbPionWin+1);
        }
        return b->nbPionWin;
    }
}

///=========================================================
//Note dans une direction

int NoteDir (board b, int lig, int col, int Ligdirec, int Coldirec)
{
    int note = 0;
    int A = lig - (Ligdirec * (b->nbPionWin-1));
    int B = col - (Coldirec * (b->nbPionWin-1));

    for (int i = 0; i < b->nbPionWin; i++) {
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
    int A = lig - (Ligdirec * (b->nbPionWin-1));
    int B = col - (Coldirec * (b->nbPionWin-1));

    for (int i = 0; i < b->nbPionWin; i++) {
        if (NbPion (b, A, B, Ligdirec, Coldirec) == b->nbPionWin) return true;
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
