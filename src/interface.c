#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "plateau_de_jeu.h"
#include "quintuplets.h"

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

void load_game_texte(char* save)
{
    int col, lig, nbj, pos, nbpiontwin, tour, match_null;

    FILE* fichier = NULL;
    fichier = fopen(save, "r");

    fscanf(fichier, "%d %d %d %d %d %d %d", &col, &lig, &nbj, &pos, &nbpiontwin, &tour, &match_null);

    board b = CreateBoard(lig, col, nbpiontwin, nbj, tour);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture* texture_croix = NULL;
    SDL_Texture* texture_rond = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Morpion",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              (b->colonne*65)+5,
                              (b->ligne*65)+5,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectangle;
    for (int x = 0; x <= (b->colonne*65)+5; x+=65){
        rectangle.x = x;
        rectangle.y = 0;
        rectangle.w = 5;
        rectangle.h = (b->ligne*65)+5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    for (int y = 0; y <= (b->ligne*65)+5; y+=65){
        rectangle.x = 0;
        rectangle.y = y;
        rectangle.w = (b->colonne*65)+5;
        rectangle.h = 5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    SDL_RenderPresent(renderer);

    SDL_Surface* croix = NULL;
    croix = SDL_LoadBMP("images/croix.bmp");
    SDL_Surface* rond = NULL;
    rond = SDL_LoadBMP("images/rond.bmp");

    SDL_SetColorKey(croix, SDL_TRUE, SDL_MapRGB(croix->format, 0, 255, 0));
    SDL_SetColorKey(rond, SDL_TRUE, SDL_MapRGB(rond->format, 0, 255, 0));

    texture_croix = SDL_CreateTextureFromSurface(renderer, croix);
    texture_rond = SDL_CreateTextureFromSurface(renderer, rond);
    if(texture_croix == NULL) printf("%s", SDL_GetError());

    SDL_FreeSurface(croix);
    SDL_FreeSurface(rond);

    SDL_Rect rectangle_o, rectangle_x;

    SDL_QueryTexture(texture_croix, NULL, NULL, &rectangle_x.w, &rectangle_x.h);
    SDL_QueryTexture(texture_rond, NULL, NULL, &rectangle_o.w, &rectangle_o.h);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fscanf(fichier, "%d", &(b->pl[y][x]));
        }
    }

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                rectangle_o.x = (x * 65) + 5;
                rectangle_o.y = (y * 65) + 5;
                SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
                SDL_RenderPresent(renderer);
            } else if(b->pl[y][x] == (b->nbPiontWin +1)) {
                rectangle_x.x = (x * 65) + 5;
                rectangle_x.y = (y * 65) + 5;
                SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                SDL_RenderPresent(renderer);
            }
        }
    }
    fclose(fichier);
    remove(save);

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
                    rectangle_x.x = (coup[1] * 65) + 5;
                    rectangle_x.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }

                } else {
                    coup = MachineJoue(b, 1);
                    rectangle_o.x = (coup[1] * 65) + 5;
                    rectangle_o.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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
                                printf("faire la sauvegarde");
                                save_game_binaire(b, tour, match_null);
                                program_launched = SDL_FALSE;
                                break;

                            default:
                                continue;

                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:

                        while(((x+65) < event.button.x) || (x > event.button.x)){
                            x = x + 65;
                            xpos++;
                        }

                        while( (y+65 < event.button.y) || (y > event.button.y)){
                            y = y + 65;
                            ypos++;
                        }

                        if(tour == 1){
                            if (PutPiont(b, b->nbPiontWin+1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_x.x = (xpos * 65) + 5;
                                rectangle_x.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 1 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }

                        } else {
                            if (PutPiont(b, 1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_o.x = (xpos * 65) + 5;
                                rectangle_o.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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

    free(coup);
    SDL_DestroyTexture(texture_croix);
    SDL_DestroyTexture(texture_rond);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
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
    SDL_Texture* texture_croix = NULL;
    SDL_Texture* texture_rond = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Morpion",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              (b->colonne*65)+5,
                              (b->ligne*65)+5,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectangle;
    for (int x = 0; x <= (b->colonne*65)+5; x+=65){
        rectangle.x = x;
        rectangle.y = 0;
        rectangle.w = 5;
        rectangle.h = (b->ligne*65)+5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    for (int y = 0; y <= (b->ligne*65)+5; y+=65){
        rectangle.x = 0;
        rectangle.y = y;
        rectangle.w = (b->colonne*65)+5;
        rectangle.h = 5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    SDL_RenderPresent(renderer);

    SDL_Surface* croix = NULL;
    croix = SDL_LoadBMP("images/croix.bmp");
    SDL_Surface* rond = NULL;
    rond = SDL_LoadBMP("images/rond.bmp");

    SDL_SetColorKey(croix, SDL_TRUE, SDL_MapRGB(croix->format, 0, 255, 0));
    SDL_SetColorKey(rond, SDL_TRUE, SDL_MapRGB(rond->format, 0, 255, 0));

    texture_croix = SDL_CreateTextureFromSurface(renderer, croix);
    texture_rond = SDL_CreateTextureFromSurface(renderer, rond);
    if(texture_croix == NULL) printf("%s", SDL_GetError());

    SDL_FreeSurface(croix);
    SDL_FreeSurface(rond);

    SDL_Rect rectangle_o, rectangle_x;

    SDL_QueryTexture(texture_croix, NULL, NULL, &rectangle_x.w, &rectangle_x.h);
    SDL_QueryTexture(texture_rond, NULL, NULL, &rectangle_o.w, &rectangle_o.h);

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            fread(&(b->pl[y][x]), sizeof(int), 1, fichier);
        }
    }

    for(int y = 0; y < b->ligne; y++){
        for(int x = 0; x < b->colonne; x++){
            if(b->pl[y][x] == 1){
                rectangle_o.x = (x * 65) + 5;
                rectangle_o.y = (y * 65) + 5;
                SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
                SDL_RenderPresent(renderer);
            } else if(b->pl[y][x] == (b->nbPiontWin +1)) {
                rectangle_x.x = (x * 65) + 5;
                rectangle_x.y = (y * 65) + 5;
                SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                SDL_RenderPresent(renderer);
            }
        }
    }
    fclose(fichier);
    remove(save);

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
                    rectangle_x.x = (coup[1] * 65) + 5;
                    rectangle_x.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }

                } else {
                    coup = MachineJoue(b, 1);
                    rectangle_o.x = (coup[1] * 65) + 5;
                    rectangle_o.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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
                                printf("faire la sauvegarde");
                                save_game_binaire(b, tour, match_null);
                                program_launched = SDL_FALSE;
                                break;

                            default:
                                continue;

                        }
                        break;

                    case SDL_MOUSEBUTTONDOWN:

                        while(((x+65) < event.button.x) || (x > event.button.x)){
                            x = x + 65;
                            xpos++;
                        }

                        while( (y+65 < event.button.y) || (y > event.button.y)){
                            y = y + 65;
                            ypos++;
                        }

                        if(tour == 1){
                            if (PutPiont(b, b->nbPiontWin+1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_x.x = (xpos * 65) + 5;
                                rectangle_x.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 1 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }

                        } else {
                            if (PutPiont(b, 1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_o.x = (xpos * 65) + 5;
                                rectangle_o.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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

    free(coup);
    SDL_DestroyTexture(texture_croix);
    SDL_DestroyTexture(texture_rond);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///=========================================================

void new_game(board b)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture* texture_croix = NULL;
    SDL_Texture* texture_rond = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Morpion",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              (b->colonne*65)+5,
                              (b->ligne*65)+5,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rectangle;
    for (int x = 0; x <= (b->colonne*65)+5; x+=65){
        rectangle.x = x;
        rectangle.y = 0;
        rectangle.w = 5;
        rectangle.h = (b->ligne*65)+5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    for (int y = 0; y <= (b->ligne*65)+5; y+=65){
        rectangle.x = 0;
        rectangle.y = y;
        rectangle.w = (b->colonne*65)+5;
        rectangle.h = 5;

        SDL_RenderFillRect(renderer, &rectangle);
    }

    SDL_RenderPresent(renderer);

    SDL_Surface* croix = NULL;
    croix = SDL_LoadBMP("images/croix.bmp");
    SDL_Surface* rond = NULL;
    rond = SDL_LoadBMP("images/rond.bmp");

    SDL_SetColorKey(croix, SDL_TRUE, SDL_MapRGB(croix->format, 0, 255, 0));
    SDL_SetColorKey(rond, SDL_TRUE, SDL_MapRGB(rond->format, 0, 255, 0));

    texture_croix = SDL_CreateTextureFromSurface(renderer, croix);
    texture_rond = SDL_CreateTextureFromSurface(renderer, rond);
    if(texture_croix == NULL) printf("%s", SDL_GetError());

    SDL_FreeSurface(croix);
    SDL_FreeSurface(rond);

    SDL_Rect rectangle_o, rectangle_x;

    SDL_QueryTexture(texture_croix, NULL, NULL, &rectangle_x.w, &rectangle_x.h);
    SDL_QueryTexture(texture_rond, NULL, NULL, &rectangle_o.w, &rectangle_o.h);

    SDL_bool program_launched = SDL_TRUE;
    int x = 5;
    int y = 5;
    int xpos = 0;
    int ypos = 0;
    int* coup;
    int tour = 1;
    bool ajouer = false;
    int match_null = b->colonne * b->ligne;

    while (program_launched){
        SDL_Event event;

        if(b->nbJoueur == 1){
            if(tour != b->pos){

                if(tour == 1){
                    coup = MachineJoue(b, b->nbPiontWin+1);
                    rectangle_x.x = (coup[1] * 65) + 5;
                    rectangle_x.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                    SDL_RenderPresent(renderer);

                    if (HaveWin(b, coup[0], coup[1]) == true) {
                        printf("IA win\n");
                        program_launched = SDL_FALSE;
                    }

                } else {
                    coup = MachineJoue(b, 1);
                    rectangle_o.x = (coup[1] * 65) + 5;
                    rectangle_o.y = (coup[0] * 65) + 5;
                    SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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

                        while(((x+65) < event.button.x) || (x > event.button.x)){
                            x = x + 65;
                            xpos++;
                        }

                        while( (y+65 < event.button.y) || (y > event.button.y)){
                            y = y + 65;
                            ypos++;
                        }

                        if(tour == 1){
                            if (PutPiont(b, b->nbPiontWin+1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_x.x = (xpos * 65) + 5;
                                rectangle_x.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_croix, NULL, &rectangle_x);
                                SDL_RenderPresent(renderer);

                                if (HaveWin(b, ypos, xpos) == true) {
                                    printf("Player 1 win\n");
                                    program_launched = SDL_FALSE;
                                }
                            }

                        } else {
                            if (PutPiont(b, 1, ypos, xpos) == true){
                                ajouer = true;
                                rectangle_o.x = (xpos * 65) + 5;
                                rectangle_o.y = (ypos * 65) + 5;
                                SDL_RenderCopy(renderer, texture_rond, NULL, &rectangle_o);
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

    free(coup);
    SDL_DestroyTexture(texture_croix);
    SDL_DestroyTexture(texture_rond);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

///=========================================================

void new_game_sans_SDL(board b) {
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
                    boucle == false;
                }

                match_null--;
                if (match_null == 0) {
                    printf("Fin de partie : match null");
                    free(coup);
                    boucle == false;
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

        if (HaveWin(b, coup[0], coup[1])) {
            printf("Player %d win\n", tour);
            free(coup);
            boucle == false;
        }

        match_null--;
        if (match_null == 0) {
            printf("Fin de partie : match null");
            free(coup);
            boucle == false;
        }

        if (tour == 1) {
            tour++;
        } else {
            tour--;
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

void menu_newgame(){
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
        if((nbpiontwin <= min(lig, col)) && (nbpiontwin != 1) && (nbpiontwin != 2)) {
            break;
        } else {
            printf("Nb <= min(ligne, colonne) et Nb > 2 : ");
        }
    }
    printf("\n");

    board b = CreateBoard(lig, col, nbpiontwin, nbj, pos);

    new_game_sans_SDL(b);

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
                sleep(4);
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
