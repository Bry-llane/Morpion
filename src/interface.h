#ifndef INTERFACE_H
#define INTERFACE_H

#include <unistd.h>
#include "plateau_de_jeu.h"

void destroy_window_and_renderer(SDL_Window* window, SDL_Renderer* renderer);
void manche(SDL_Renderer* renderer, board b, int tour, int match_null);
void draw_morpion(SDL_Renderer* renderer, board b);
SDL_Window* create_window(const char* title, board b);
SDL_Renderer* create_renderer(SDL_Window* window);
int isInt();
bool existeFile(char* file);
void rename_save();
void save_game_texte(board b, int tour, int nbcaseRestant);
void save_game_binaire(board b, int tour, int nbcaseRestant);
void load_game_texte(char* save);
void load_game_binaire(char* save);
void new_game(board b);
void menu_sauvegarde();
void menu_delete();
int min(int a, int b);
void menu_newgame();
void menu_principale();

#endif
