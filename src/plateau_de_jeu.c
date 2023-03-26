#include <stdio.h>
#include <stdlib.h>

#include "plateau_de_jeu.h"

///=======================================================

void clear_screen()
{
    system("cls");
}

///========================================================

board CreateBoard (int ligne, int colonne, int nbPiontWin, int joueur, int tour)
{
    board B = (board)malloc(sizeof(struct board));

    piont **p = (piont **)malloc(ligne*sizeof(piont*));
    for (int i=0; i<ligne; i++){
        p[i] = (piont*)malloc(colonne*sizeof(piont));
    }

    for(int i = 0; i < ligne; i++){
        for(int j = 0; j < colonne; j++){
            p[i][j] = 0;
        }
    }

    B->ligne = ligne;
    B->colonne = colonne;
    B->nbPiontWin = nbPiontWin;
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
            } else if(b->pl[i][j] == b->nbPiontWin+1){
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

piont RecupPiont (board b, int lig, int col)
{
    piont p = b->pl[lig][col];

    return p;
}

///=========================================================

bool PutPiont (board b, piont p, int lig, int col)
{
    if (TestEmpty(b, lig, col)){
        b->pl[lig][col] = p;
        return true;
    }
    return false;
}

///=========================================================

void gotoxy(int x, int y)
{
    printf("\x1b[%d;%dH", y+1, x+1);
}

///=========================================================

int* UnCoup (board b, piont p)
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

    PutPiont (b, p, lig, col);
    int* cord = (int*) malloc(2*sizeof(int));
    cord[0] = lig;
    cord[1] = col;

    return cord;
}

///=========================================================
//coup avec touche directionnel
int* Coup (board b, piont p)
{
    int lig = 0;
    int col = 0;
    int enter;
    int x = 2, y = 1;
    char c;
    int* cord = (int) malloc(sizeof(int)*2);

    while(true)
    {
        clear_screen();
        PrintBoardBis(b);
        gotoxy(x, y);
        enter = 0;
        while(enter != 1){
            c = getch();
            switch(c){
            case 80:
                if (lig == b->ligne - 1) {
                    lig = 0;
                    y = 1;
                } else {
                    lig++;
                    y = y+2;
                }
                gotoxy(x, y);
                break;
            case 72:
                if (lig == 0) {
                    lig = b->ligne - 1;
                    y = (b->ligne * 2) - 1;
                } else {
                    lig--;
                    y = y-2;
                }
                gotoxy(x, y);
                break;
            case 75:
                if (col == 0) {
                    col = b->colonne-1;
                    x = (b->colonne *4) - 2;
                } else {
                    col--;
                    x = x-4;
                }
                gotoxy(x, y);
                break;
            case 77:
                if (col == b->colonne - 1) {
                    col = 0;
                    x = 2;
                } else {
                    col++;
                    x = x+4;
                }
                gotoxy(x, y);
                break;
            case 13:
                enter = 1;
                break;
            case 115:
                clear_screen();
                cord[0] = -1;
                enter = 1;
                break;
            default :
                break;
            }
        }

        if(TestEmpty (b, lig, col) || cord[0] == -1) break;
    }

    if(cord[0] == -1) return cord;

    PutPiont (b, p, lig, col);
    cord[0] = lig;
    cord[1] = col;
    clear_screen();
    return cord;
}

///=========================================================
//la machine joue
int* MachineJoue (board b, piont p)
{
    int* best = MeilleureNote(b);
    PutPiont (b, p, best[0], best[1]);
    printf("La machine a joue en (%d; %d)\n", best[1]+1, best[0]+1);
    //sleep(1);
    return best;
}

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
