#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "plateau_de_jeu.h"
#include "interface.h"

#define TAILLE_CASE 40
#define TAILLE_SEPARATION 3
#define TAILLE_T (TAILLE_CASE+TAILLE_SEPARATION)


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

SDL_Texture* loadTexture(SDL_Renderer* renderer, char* path) {
    SDL_Surface* surface = SDL_LoadBMP(path); // Charger l'image
    SDL_Texture* texture = NULL; // Texture à retourner

    // Vérifier si l'image a bien été chargée
    if (surface == NULL) {
        printf("Erreur chargement image\n");
    }
    else {
        // Créer une texture à partir de la surface
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        // Vérifier si la texture a bien été créée
        if (texture == NULL) {
            printf("Erreur de création de la texture\n");
        }

        // Libérer la surface chargée
        SDL_FreeSurface(surface);
    }

    return texture; // Retourner la texture créée
}

///=========================================================

void manche(SDL_Renderer* renderer, board b, int tour, int match_null)
{
    SDL_Texture* cercle = loadTexture(renderer, "./images/Cercle.bmp");
    SDL_Texture* croix = loadTexture(renderer, "./images/Croix.bmp");

    SDL_Rect rect_croix, rect_cercle;

    SDL_QueryTexture(cercle, NULL, NULL, &rect_cercle.w, &rect_cercle.h);
    SDL_QueryTexture(croix, NULL, NULL, &rect_croix.w, &rect_croix.h);

    SDL_bool program_launched = SDL_TRUE;
    int x = TAILLE_SEPARATION;
    int y = TAILLE_SEPARATION;
    int xpos = 0;
    int ypos = 0;
    int* coup;
    bool ajouer = false;

    while (program_launched){
        SDL_Event event;

        if(b->nbJoueur == 1){
            if(tour != b->pos){

                if(tour == 1){
                    coup = MachineJoue(b, b->nbPionWin+1);
                    rect_croix.x = (coup[1]*TAILLE_T) + TAILLE_SEPARATION;
                    rect_croix.y = (coup[0]*TAILLE_T) + TAILLE_SEPARATION;
                    SDL_RenderCopy(renderer, croix, NULL, &rect_croix);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }

                } else {
                    coup = MachineJoue(b, 1);
                    rect_cercle.x = (coup[1]*TAILLE_T) + TAILLE_SEPARATION;
                    rect_cercle.y = (coup[0]*TAILLE_T) + TAILLE_SEPARATION;
                    SDL_RenderCopy(renderer, cercle, NULL, &rect_cercle);
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

        free(coup);

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
                            if (PutPion(b, b->nbPionWin+1, ypos, xpos) == true){
                                ajouer = true;
                                rect_croix.x = (xpos*TAILLE_T) + TAILLE_SEPARATION;
                                rect_croix.y = (ypos*TAILLE_T) + TAILLE_SEPARATION;
                                SDL_RenderCopy(renderer, croix, NULL, &rect_croix);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 1 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }

                        } else {
                            if (PutPion(b, 1, ypos, xpos) == true){
                                ajouer = true;
                                rect_cercle.x = (xpos*TAILLE_T) + TAILLE_SEPARATION;
                                rect_cercle.y = (ypos*TAILLE_T) + TAILLE_SEPARATION;
                                SDL_RenderCopy(renderer, croix, NULL, &rect_cercle);
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
    }


    FreeBoard(b);

    SDL_DestroyTexture(croix);
    SDL_DestroyTexture(cercle);
}

///=========================================================

void draw_morpion(SDL_Renderer* renderer, board b)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

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
    fprintf(fichier, "%d\n", b->nbPionWin);
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
    fwrite(&b->nbPionWin, sizeof(int), 1, fichier);
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
    int col, lig, nbj, pos, nbPionWin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "r");

    fscanf(fichier, "%d %d %d %d %d %d %d", &col, &lig, &nbj, &pos, &nbPionWin, &tour, &match_null);

    board b = CreateBoard(lig, col, nbPionWin, nbj, tour);

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

    SDL_Texture* cercle = loadTexture(renderer, "./images/Cercle.bmp");
    SDL_Texture* croix = loadTexture(renderer, "./images/Croix.bmp");

    SDL_Rect rect_croix, rect_cercle;

    SDL_QueryTexture(cercle, NULL, NULL, &rect_cercle.w, &rect_cercle.h);
    SDL_QueryTexture(croix, NULL, NULL, &rect_croix.w, &rect_croix.h);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                rect_cercle.x = (x*TAILLE_T) + TAILLE_SEPARATION;
                rect_cercle.y = (y*TAILLE_T) + TAILLE_SEPARATION;
                SDL_RenderCopy(renderer, cercle, NULL, &rect_cercle);
            } else if(b->pl[y][x] == (b->nbPionWin +1)) {
                rect_croix.x = (x*TAILLE_T) + TAILLE_SEPARATION;
                rect_croix.y = (y*TAILLE_T) + TAILLE_SEPARATION;
                SDL_RenderCopy(renderer, croix, NULL, &rect_cercle);
            }
        }
    }

    SDL_DestroyTexture(croix);
    SDL_DestroyTexture(cercle);
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
    int col, lig, nbj, pos, nbPionWin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "rb");

    fread(&col, sizeof(int), 1, fichier);
    fread(&lig, sizeof(int), 1, fichier);
    fread(&nbj, sizeof(int), 1, fichier);
    fread(&pos, sizeof(int), 1, fichier);
    fread(&nbPionWin, sizeof(int), 1, fichier);
    fread(&tour, sizeof(int), 1, fichier);
    fread(&match_null, sizeof(int), 1, fichier);

    board b = CreateBoard(lig, col, nbPionWin, nbj, tour);

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

    SDL_Texture* cercle = loadTexture(renderer, "./images/Cercle.bmp");
    SDL_Texture* croix = loadTexture(renderer, "./images/Croix.bmp");

    SDL_Rect rect_croix, rect_cercle;

    SDL_QueryTexture(cercle, NULL, NULL, &rect_cercle.w, &rect_cercle.h);
    SDL_QueryTexture(croix, NULL, NULL, &rect_croix.w, &rect_croix.h);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                rect_cercle.x = (x*TAILLE_T) + TAILLE_SEPARATION;
                rect_cercle.y = (y*TAILLE_T) + TAILLE_SEPARATION;
                SDL_RenderCopy(renderer, cercle, NULL, &rect_cercle);
            } else if(b->pl[y][x] == (b->nbPionWin +1)) {
                rect_croix.x = (x*TAILLE_T) + TAILLE_SEPARATION;
                rect_croix.y = (y*TAILLE_T) + TAILLE_SEPARATION;
                SDL_RenderCopy(renderer, croix, NULL, &rect_cercle);
            }
        }
    }

    SDL_DestroyTexture(croix);
    SDL_DestroyTexture(cercle);
    fclose(fichier);
    remove(save);

    SDL_RenderPresent(renderer);
    manche(renderer, b, tour, match_null);

    destroy_window_and_renderer(window, renderer);
    SDL_Quit();
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

    int col, lig, nbj, pos, nbPionWin;

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
        nbPionWin = isInt();
        if((nbPionWin <= min(lig, col)) && (nbPionWin != 1) && (nbPionWin != 2) && (nbPionWin != 0)) {
            break;
        } else {
            printf("Nb <= min(ligne, colonne) et Nb > 2 : ");
        }
    }
    printf("\n");

    board b = CreateBoard(lig, col, nbPionWin, nbj, pos);

    new_game(b);
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
