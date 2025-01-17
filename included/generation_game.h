#ifndef INCLUDED_GENERATION_GAME_H
#define INCLUDED_GENERATION_GAME_H

#include "moteur.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadPlayers(const char *filename, Story *story);
void loadMonsters(const char *filename, Story *story);
void loadStory(const char *filename, Story *story);

typedef struct Free Free;
struct Free {
  cJSON *Jstory;
  cJSON *Jitems;
  cJSON *Jplayer;
  cJSON *Jmonster;
};
cJSON *cJSON_ParseFromFile(const char *nom_fichier);
void initialisation_jeu(Story *story, Free *liberer, bool reprendre);

cJSON *Lecture_Story_Json(Story *story);
cJSON *Lecture_Personnage_JSON(Story *story, bool reprise);
cJSON *Lecture_Items_JSON(Story *story);
cJSON *Lecture_Monstre_JSON(Story *story);
#endif // GENERATION_GAME_H
