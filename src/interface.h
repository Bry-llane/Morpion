#ifndef INTERFACE_H
#define INTERFACE_H

#include <unistd.h>
#include "plateau_de_jeu.h"

SDL_Window* Create_window(const char* title, board b);
SDL_Renderer* Create_renderer(SDL_Window* window);
void Destroy_window_and_renderer(SDL_Window* window, SDL_Renderer* renderer);
SDL_Texture* LoadTexture(SDL_Renderer* renderer, char* path);
void Draw_morpion(SDL_Renderer* renderer, board b);
int IsInt();
bool ExisteFile(char* file);
void Rename_save();
void Save_game_texte(board b, int tour, int nbcaseRestant);
void Save_game_binaire(board b, int tour, int nbcaseRestant);
void Manche(SDL_Renderer* renderer, board b, int tour, int match_null);
void Load_game_texte(char* Save);
void Load_game_binaire(char* Save);
void New_game(board b);
void Menu_sauvegarde();
void Menu_delete();
int Min(int a, int b);
void Menu_newgame();
void Menu_principale();

#endif
