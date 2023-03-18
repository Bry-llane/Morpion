#ifndef INTERFACE_H
#define INTERFACE_H
#include "plateau_de_jeu.h"

void save_game(board b, int tour, int nbcaseRestant);
void load_game(char* save);
void new_game(board b);
void menu();

#endif
