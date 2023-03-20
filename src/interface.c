#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "plateau_de_jeu.h"
#include "quintuplets.h"
#include "interface.h"

#define RAYON 15
#define SIZE 25
#define TAILLE_CASE 35
#define TAILLE_SEPARATION 2
#define TAILLE_T (TAILLE_CASE+TAILLE_SEPARATION)
#define CENTRE_CASE (((TAILLE_CASE-1)/2)+TAILLE_SEPARATION)


void destroy_window_and_renderer(SDL_Window* window, SDL_Renderer* renderer)
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

///=========================================================

void manche(SDL_Renderer* renderer, board b, int tour, int match_null)
{
    SDL_Color rond = { 255, 0, 0, 255 };
    SDL_Color croix = { 0, 0, 255, 255 };
    SDL_bool program_launched = SDL_TRUE;
    int x = 5;
    int y = 5;
    int xpos = 0;
    int ypos = 0;
    int* coup;
    bool ajouer = false;

    while (program_launched){
        SDL_Event event;

        if(b->nbJoueur == 1){
            if(tour != b->pos){

                if(tour == 1){
                    coup = MachineJoue(b, b->nbPiontWin+1);
                    draw_x(renderer, (coup[1]*TAILLE_T)+CENTRE_CASE, (coup[0]*TAILLE_T)+CENTRE_CASE, SIZE, croix);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }

                } else {
                    coup = MachineJoue(b, 1);
                    draw_circle(renderer, (coup[1]*TAILLE_T)+CENTRE_CASE, (coup[0]*TAILLE_T)+CENTRE_CASE, RAYON, rond);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }
                }

                match_null--;
                if(match_null == 0) {
                    printf("Fin de partie : match null");
                    program_launched = SDL_FALSE;
                }

                if(tour == 1) {
                    tour++;
                } else {
                    tour--;
                }
            }
        }

        while(SDL_PollEvent(&event))
            {
                switch(event.type)
                {
                    case SDL_KEYDOWN:
                        switch(event.key.keysym.sym)
                        {
                            case SDLK_s:
                                save_game_binaire(b, tour, match_null);
                                program_launched = SDL_FALSE;
                                break;

                            default:
                                continue;

                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:

                        while(((x+TAILLE_T) < event.button.x) || (x > event.button.x)){
                            x = x + TAILLE_T;
                            xpos++;
                        }

                        while( (y+TAILLE_T < event.button.y) || (y > event.button.y)){
                            y = y + TAILLE_T;
                            ypos++;
                        }

                        if(tour == 1){
                            if (PutPiont(b, b->nbPiontWin+1, ypos, xpos) == true){
                                ajouer = true;
                                draw_x(renderer, (xpos*TAILLE_T)+CENTRE_CASE, (ypos*TAILLE_T)+CENTRE_CASE, SIZE, croix);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 1 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }

                        } else {
                            if (PutPiont(b, 1, ypos, xpos) == true){
                                ajouer = true;
                                draw_circle(renderer, (xpos*TAILLE_T)+CENTRE_CASE, (ypos*TAILLE_T)+CENTRE_CASE, RAYON, rond);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 2 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }
                        }

                        x = 5;
                        y = 5;
                        xpos = 0;
                        ypos = 0;

                        if(ajouer == true){
                            match_null--;
                            if(match_null == 0) {
                                printf("Fin de partie : match null");
                                program_launched = SDL_FALSE;
                            }
                            if(tour == 1) {
                                tour++;
                            } else if (tour == 2){
                                tour--;
                            }
                        }
                        ajouer = false;
                        break;

                    case SDL_QUIT:
                        program_launched = SDL_FALSE;
                        break;

                    default:
                        continue;
                }
        }
        free(coup);
    }
}

///=========================================================

void draw_morpion(SDL_Renderer* renderer, board b)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectangle;
    for (int x = 0; x <= (b->colonne*TAILLE_T)+TAILLE_SEPARATION; x+=TAILLE_T){
        rectangle.x = x;
        rectangle.y = 0;
        rectangle.w = TAILLE_SEPARATION;
        rectangle.h = (b->ligne*TAILLE_T)+TAILLE_SEPARATION;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    for (int y = 0; y <= (b->ligne*TAILLE_T)+TAILLE_SEPARATION; y+=TAILLE_T){
        rectangle.x = 0;
        rectangle.y = y;
        rectangle.w = (b->colonne*TAILLE_T)+TAILLE_SEPARATION;
        rectangle.h = TAILLE_SEPARATION;

        SDL_RenderFillRect(renderer, &rectangle);
    }
}

///=========================================================

SDL_Window* create_window(const char* title, board b)
{
    SDL_Window* window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              (b->colonne*TAILLE_T)+TAILLE_SEPARATION,
                              (b->ligne*TAILLE_T)+TAILLE_SEPARATION,
                              SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("Erreur lors de la création de la fenêtre SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return window;
}

///=========================================================

SDL_Renderer* create_renderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "Erreur : impossible de créer le renderer SDL\n");
        return NULL;
    }
    return renderer;
}

///=========================================================

void draw_circle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color)
{
    // Dessine le cercle pixel par pixel en utilisant la fonction SDL_RenderDrawPoint()
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = -radius; w <= radius; w++)
    {
        for (int h = -radius; h <= radius; h++)
        {
            if (w * w + h * h <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int w = -10; w <= 10; w++)
    {
        for (int h = -10; h <= 10; h++)
        {
            if (w * w + h * h <= 10 * 10)
            {
                SDL_RenderDrawPoint(renderer, x + w, y + h);
            }
        }
    }
}

///=========================================================

void draw_x(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color)
{
    // Dessine la première ligne diagonale de l'X
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, x - size / 2, y - size / 2, x + size / 2, y + size / 2);

    // Dessine la deuxième ligne diagonale de l'X
    SDL_RenderDrawLine(renderer, x + size / 2, y - size / 2, x - size / 2, y + size / 2);
}

///=========================================================

int isInt()
{
    int num;
    char c;

    while(true){
        if(scanf("%d", &num)!= 1){
            printf("Veuillez rentrer un chiffre : ");
            while((c = getchar()) != '\n' && c != EOF);
        } else {
           break;
        }
    }

    return num;
}

///=========================================================

bool existeFile(char* file)
{
    FILE *fichier = NULL;
    fichier = fopen(file, "r");

    if (fichier == NULL) {
        return false;
    } else {
        fclose(fichier);
        return true;
    }
}

///=========================================================

void rename_save()
{
    printf("\n");

    if(existeFile("./save/save1.txt")){
        printf("1 : Sauvegarde 1\n");
    } else {
        printf("1 : Empty \n");
    }

    if(existeFile("./save/save2.txt")){
        printf("2 : Sauvegarde 2\n");
    } else {
        printf("2 : Empty \n");
    }

    if(existeFile("./save/save3.txt")){
        printf("3 : Sauvegarde 3\n");
    } else {
        printf("3 : Empty \n\n");
    }

    int choix;
    bool boucle = true;

    while (boucle){
        printf("Attente du numero choisi : ");
        choix = isInt();

        switch(choix){
            case 1:
                if(existeFile("./save/save1.txt")){
                    remove("./save/save1.txt");
                }
                rename("./src/save.txt", "./save/save1.txt");
                boucle = false;
                break;

            case 2:
                if(existeFile("./save/save2.txt")){
                    remove("./save/save2.txt");
                }
                rename("./src/save.txt", "./save/save2.txt");
                boucle = false;
                break;

            case 3:
                if(existeFile("./save/save3.txt")){
                    remove("./save/save3.txt");
                }
                rename("./src/save.txt", "./save/save3.txt");
                boucle = false;
                break;

            case 4:
                boucle = false;
                break;

            default:
                break;
        }
    }
}

///=========================================================

void save_game_texte(board b, int tour, int nbcaseRestant)
{
    FILE* fichier = NULL;
    fichier = fopen("./src/save.txt", "w+");

    fprintf(fichier, "%d\n", b->colonne);
    fprintf(fichier, "%d\n", b->ligne);
    fprintf(fichier, "%d\n", b->nbJoueur);
    fprintf(fichier, "%d\n", b->pos);
    fprintf(fichier, "%d\n", b->nbPiontWin);
    fprintf(fichier, "%d\n", tour);
    fprintf(fichier, "%d\n", nbcaseRestant);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fprintf(fichier, "%d ", b->pl[y][x]);
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
    rename_save();
}

///=========================================================

void save_game_binaire(board b, int tour, int nbcaseRestant)
{
    FILE* fichier = NULL;
    fichier = fopen("./src/save.txt", "wb");

    fwrite(&b->colonne, sizeof(int), 1, fichier);
    fwrite(&b->ligne, sizeof(int), 1, fichier);
    fwrite(&b->nbJoueur, sizeof(int), 1, fichier);
    fwrite(&b->pos, sizeof(int), 1, fichier);
    fwrite(&b->nbPiontWin, sizeof(int), 1, fichier);
    fwrite(&tour, sizeof(int), 1, fichier);
    fwrite(&nbcaseRestant, sizeof(int), 1, fichier);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fwrite(&b->pl[y][x], sizeof(int), 1, fichier);
        }
    }

    fclose(fichier);
    rename_save();
}

///=========================================================

void load_game_texte(char* save)
{
    int col, lig, nbj, pos, nbpiontwin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "r");

    fscanf(fichier, "%d %d %d %d %d %d %d", &col, &lig, &nbj, &pos, &nbpiontwin, &tour, &match_null);

    board b = CreateBoard(lig, col, nbpiontwin, nbj, tour);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = create_window("Morpion", b);
    renderer = create_renderer(window);
    draw_morpion(renderer, b);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fscanf(fichier, "%d", &(b->pl[y][x]));
        }
    }

    SDL_Color oo = { 255, 0, 0, 255 };
    SDL_Color xx = { 0, 0, 255, 255 };

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                draw_x(renderer, (x*TAILLE_T)+CENTRE_CASE, (y*TAILLE_T)+CENTRE_CASE, SIZE, xx);
            } else if(b->pl[y][x] == (b->nbPiontWin +1)) {
                draw_circle(renderer, (x*TAILLE_T)+CENTRE_CASE, (y*TAILLE_T)+CENTRE_CASE, RAYON, oo);
            }
        }
    }

    fclose(fichier);
    remove(save);

    SDL_RenderPresent(renderer);
    manche(renderer, b, tour, match_null);

    destroy_window_and_renderer(window, renderer);
    SDL_Quit();
}

///=========================================================

void load_game_binaire(char* save)
{
    int col, lig, nbj, pos, nbpiontwin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "rb");

    fread(&col, sizeof(int), 1, fichier);
    fread(&lig, sizeof(int), 1, fichier);
    fread(&nbj, sizeof(int), 1, fichier);
    fread(&pos, sizeof(int), 1, fichier);
    fread(&nbpiontwin, sizeof(int), 1, fichier);
    fread(&tour, sizeof(int), 1, fichier);
    fread(&match_null, sizeof(int), 1, fichier);

    board b = CreateBoard(lig, col, nbpiontwin, nbj, tour);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = create_window("Morpion", b);
    renderer = create_renderer(window);
    draw_morpion(renderer, b);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fread(&(b->pl[y][x]), sizeof(int), 1, fichier);
        }
    }

    SDL_Color oo = { 255, 0, 0, 255 };
    SDL_Color xx = { 0, 0, 255, 255 };

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                draw_x(renderer, (x*TAILLE_T)+CENTRE_CASE, (y*TAILLE_T)+CENTRE_CASE, SIZE, xx);
            } else if(b->pl[y][x] == (b->nbPiontWin +1)) {
                draw_circle(renderer, (x*TAILLE_T)+CENTRE_CASE, (y*TAILLE_T)+CENTRE_CASE, RAYON, oo);
            }
        }
    }

    fclose(fichier);
    remove(save);

    SDL_RenderPresent(renderer);
    manche(renderer, b, tour, match_null);

    destroy_window_and_renderer(window, renderer);
    SDL_Quit();
}

///=========================================================

void load_game_binaire_sans_SDL(char* save)
{
    int col, lig, nbj, pos, nbpiontwin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "rb");

    fread(&col, sizeof(int), 1, fichier);
    fread(&lig, sizeof(int), 1, fichier);
    fread(&nbj, sizeof(int), 1, fichier);
    fread(&pos, sizeof(int), 1, fichier);
    fread(&nbpiontwin, sizeof(int), 1, fichier);
    fread(&tour, sizeof(int), 1, fichier);
    fread(&match_null, sizeof(int), 1, fichier);

    board b = CreateBoard(lig, col, nbpiontwin, nbj, tour);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fread(&(b->pl[y][x]), sizeof(int), 1, fichier);
        }
    }

    fclose(fichier);
    remove(save);

    int* coup;
    bool boucle = true;

    while (boucle) {
        if (b->nbJoueur == 1) {
            if (tour != b->pos) {
                if (tour == 1) {
                    coup = MachineJoue(b, b->nbPiontWin+1);
                } else {
                    coup = MachineJoue(b, 1);
                }

                if (HaveWin(b, coup[0], coup[1])) {
                    printf("IA win\n");
                    free(coup);
                    boucle = false;
                }

                match_null--;
                if (match_null == 0) {
                    printf("Fin de partie : match null");
                    free(coup);
                    boucle = false;
                }

                if (tour == 1) {
                    tour++;
                } else {
                    tour--;
                }
            }
        }

        if(tour == 1){
            coup = Coup(b, b->nbPiontWin+1);
        } else {
            coup = Coup(b, 1);
        }

        if (coup[0] == -1){
            save_game_binaire(b, tour, match_null);
            boucle = false;
        } else {
            if (HaveWin(b, coup[0], coup[1])) {
                printf("Player %d win\n", tour);
                free(coup);
                boucle = false;
            }

            match_null--;
            if (match_null == 0) {
                printf("Fin de partie : match null");
                free(coup);
                boucle = false;
            }

            if (tour == 1) {
                tour++;
            } else {
                tour--;
            }
        }
        free(coup);
    }
}
///=========================================================

void new_game(board b)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    window = create_window("Morpion", b);
    renderer = create_renderer(window);

    draw_morpion(renderer, b);

    SDL_RenderPresent(renderer);
    manche(renderer, b, 1, b->colonne * b->ligne);

    destroy_window_and_renderer(window, renderer);
    SDL_Quit();
}

///=========================================================

void new_game_sans_SDL(board b)
{
    int tour = 1;
    int match_null = b->colonne * b->ligne;
    int* coup;
    bool boucle = true;

    while (boucle) {
        if (b->nbJoueur == 1) {
            if (tour != b->pos) {
                if (tour == 1) {
                    coup = MachineJoue(b, b->nbPiontWin+1);
                } else {
                    coup = MachineJoue(b, 1);
                }

                if (HaveWin(b, coup[0], coup[1])) {
                    printf("IA win\n");
                    free(coup);
                    boucle = false;
                }

                match_null--;
                if (match_null == 0) {
                    printf("Fin de partie : match null");
                    free(coup);
                    boucle = false;
                }

                if (tour == 1) {
                    tour++;
                } else {
                    tour--;
                }
            }
        }

        if(tour == 1){
            coup = Coup(b, b->nbPiontWin+1);
        } else {
            coup = Coup(b, 1);
        }

        if (coup[0] == -1){
            save_game_binaire(b, tour, match_null);
            boucle = false;
        } else {
            if (HaveWin(b, coup[0], coup[1])) {
                printf("Player %d win\n", tour);
                free(coup);
                boucle = false;
            }

            match_null--;
            if (match_null == 0) {
                printf("Fin de partie : match null");
                free(coup);
                boucle = false;
            }

            if (tour == 1) {
                tour++;
            } else {
                tour--;
            }
        }
        free(coup);
    }
}

///=========================================================

void menu_sauvegarde()
{
    clear_screen();
    printf("*****************************\n");
    printf("*         Sauvegarde        *\n");
    printf("*****************************\n");
    printf("\n");

    if(existeFile("./save/save1.txt")){
        printf("1 : Sauvegarde 1\n");
    } else {
        printf("1 : Empty \n");
    }

    if(existeFile("./save/save2.txt")){
        printf("2 : Sauvegarde 2\n");
    } else {
        printf("2 : Empty \n");
    }

    if(existeFile("./save/save3.txt")){
        printf("3 : Sauvegarde 3\n");
    } else {
        printf("3 : Empty \n");
    }

    printf("4 : retour\n\n");

    int choix;
    bool boucle = true;

    while (boucle){
        printf("Attente du numero choisi : ");
        choix = isInt();

        switch(choix){
            case 1:
                if(existeFile("./save/save1.txt")){
                    load_game_binaire("./save/save1.txt");
                    boucle = false;
                }
                break;

            case 2:
                if(existeFile("./save/save2.txt")){
                    load_game_binaire("./save/save2.txt");
                    boucle = false;
                }
                break;

            case 3:
                if(existeFile("./save/save3.txt")){
                    load_game_binaire("./save/save3.txt");
                    boucle = false;
                }
                break;

            case 4:
                boucle = false;
                break;

            default:
                break;
        }
    }
}

///=========================================================

void menu_delete()
{
    clear_screen();
    printf("*****************************\n");
    printf("*           Delete          *\n");
    printf("*****************************\n");
    printf("\n");

    if(existeFile("./save/save1.txt")){
        printf("1 : Sauvegarde 1\n");
    } else {
        printf("1 : Empty \n");
    }

    if(existeFile("./save/save2.txt")){
        printf("2 : Sauvegarde 2\n");
    } else {
        printf("2 : Empty \n");
    }

    if(existeFile("./save/save3.txt")){
        printf("3 : Sauvegarde 3\n");
    } else {
        printf("3 : Empty \n");
    }

    printf("4 : retour\n\n");

    int choix;
    bool boucle = true;

    while (boucle){
        printf("Attente du numero choisi : ");
        choix = isInt();

        switch(choix){
            case 1:
                if(existeFile("./save/save1.txt")){
                    remove("./save/save1.txt");
                    boucle = false;
                }
                break;

            case 2:
                if(existeFile("./save/save2.txt")){
                    remove("./save/save2.txt");
                    boucle = false;
                }
                break;

            case 3:
                if(existeFile("./save/save3.txt")){
                    remove("./save/save3.txt");
                    boucle = false;
                }
                break;

            case 4:
                boucle = false;
                break;

            default:
                break;
        }
    }
}

///=========================================================

int min(int a, int b)
{
    return a < b ? a : b;
}

///=========================================================

void menu_newgame()
{
    clear_screen();
    printf("*****************************\n");
    printf("*          NewGame          *\n");
    printf("*****************************\n");
    printf("\n");

    int col, lig, nbj, pos, nbpiontwin;

    printf("Nombre de ligne : ");
    while(true){
        lig = isInt();
        if(lig > 2) {
            break;
        } else {
            printf("Le nombre doit etre superieur a 2 (Nb > 2) : ");
        }
    }
    printf("\n");

    printf("Nombre de colonne : ");
    while(true){
        col = isInt();
        if(col > 2) {
            break;
        } else {
            printf("Le nombre doit etre superieur a 2 (Nb > 2) : ");
        }
    }
    printf("\n");

    printf("Nombre de joueur : ");
    while(true){
        nbj = isInt();
        if((nbj == 1) || (nbj == 2)){
            break;
        } else {
            printf("En attente d'un 1 ou 2 : ");
        }
    }
    printf("\n");

    if(nbj == 1){
        printf("1 pour jouer en premier 2 sinon : ");
        while(true){
            pos = isInt();
            if((pos == 1) || (pos == 2)){
                break;
            } else {
                printf("En attente d'un 1 ou 2 : ");
            }
        }
        printf("\n");
    } else {
        pos = 0;
    }


    printf("Nombre de piont gagnant : ");
    while(true){
        nbpiontwin = isInt();
        if((nbpiontwin <= min(lig, col)) && (nbpiontwin != 1) && (nbpiontwin != 2) && (nbpiontwin != 0)) {
            break;
        } else {
            printf("Nb <= min(ligne, colonne) et Nb > 2 : ");
        }
    }
    printf("\n");

    board b = CreateBoard(lig, col, nbpiontwin, nbj, pos);

    new_game(b);

    for(int y = 0; y < b->ligne; y++){
        free(b->pl[y]);
    }
}

///=========================================================

void menu_principale()
{
debut:
    clear_screen();
    int choix;

    printf("*****************************\n");
    printf("*            Menu           *\n");
    printf("*****************************\n");
    printf("\n");

    printf("1 : New game\n");
    printf("2 : Load save\n");
    printf("3 : Delete save\n");
    printf("4 : Quitter\n\n");

    bool boucle = true;

    while (boucle){
        printf("Attente du numero choisi : ");
        choix = isInt();

        switch (choix){
            case 1:
                menu_newgame();
                sleep(2);
                goto debut;
                break;

            case 2:
                menu_sauvegarde();
                goto debut;
                break;

            case 3:
                menu_delete();
                goto debut;
                break;

            case 4:
                boucle = false;
                break;

            default:
                break;
        }
    }
}
