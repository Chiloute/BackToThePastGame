#ifndef INCLUDED_MAIN_H
#define INCLUDED_MAIN_H

#include <locale.h> // Pour l'UTF-8;
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Graphical function
void afficher_bandeau(WINDOW *bandeau, int chapitre, char *chapter_name,
                      char *name, int score, int lp, int atk);
void initialisation_couleur();
#endif // !INCLUDED_MAIN_H
#define INCLUDED_MAIN_H
