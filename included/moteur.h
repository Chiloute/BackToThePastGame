#ifndef INCLUDED_MOTEUR_H
#define INCLUDED_MOTEUR_H
#include "../included/main.h"
#include <ncurses.h>

typedef struct Story Story;
typedef struct Chapters Chapters;
typedef struct Player Player;
typedef struct Inventory Inventory;
typedef struct Items Items;
typedef struct Bestiaire Bestiaire;
typedef struct Screen Screen;
typedef struct Action Action;

#define MAX_NAME 256
#define MAX_ITEMS 10
#define MAX_LINE_LENGTH 1024
#define MAX_ACTIONS 5
#define MAX_CHAPTERS 8
#define MAX_MONSTER 20

enum Type { HEAL, ATK };
struct Action {
  char *description;
};
struct Chapters {
  char *name;
  char *txt;
  int nbraction;
  Action action[MAX_ACTIONS];
  int id_enemie; // = -1 si pas d'enemie
};

struct Items {
  char *name;
  char *description;
  int value;
  enum Type type;
};

struct Inventory {
  Items itemlist[MAX_ITEMS];
  int nbrobj;
};

struct Player {
  char *name;
  int lp;
  int atk;
  int fuir;
  Inventory inv;
};
struct Bestiaire {
  int id;
  char *name;
  int lp;
  int atk;
  char *ascii_art;
};

struct Screen {
  WINDOW *bandeau;
  WINDOW *jeu;
  int hauteur;
  int largeur;
};

// Structure qui relie tout
struct Story {
  Chapters chapters[MAX_CHAPTERS];
  int nbr_chapters;
  int nbr_monsters;
  Player player;
  Bestiaire enemies[MAX_MONSTER];
  Items items[MAX_ITEMS];
  int story_nb_items;
  Screen screen;
  int current_chapter;
  int score;
};

// ############
// # Fonction #
// ############
void save_game_json(Story *story);
void Lancement_Partie(Story *story, bool reprise);
void Information_Player(Story *story);
void afficher_object(Story *story);
void afficher_suivant(Screen screen);
void save_game(Story *story);
void GetItems(Story *story, int item_id);
void UtilisationItems(Story *story, int itemNumber);
int Combat(Story *story, int monster);
void UtilisationItems(Story *story, int itemNumber);
bool ChoixVoie(Story *story);

#endif // !INCLUDED_MOTEUR_H
