#include "../included/generation_game.h"
#include <cjson/cJSON.h>

cJSON *cJSON_ParseFromFile(const char *nom_fichier) {
  FILE *fichier = fopen(nom_fichier, "r");
  if (fichier == NULL) {
    perror("Erreur lors de l'ouverture du fichier");
    return NULL;
  }

  fseek(fichier, 0, SEEK_END);
  size_t taille = (size_t)ftell(fichier);
  fseek(fichier, 0, SEEK_SET);

  char *contenu = (char *)malloc(taille + 1);
  if (contenu == NULL) {
    perror("Erreur d'allocation mémoire");
    fclose(fichier);
    return NULL;
  }

  fread(contenu, 1, taille, fichier);
  contenu[taille] = '\0';

  fclose(fichier);

  cJSON *root = cJSON_Parse(contenu);
  free(contenu);

  return root;
}

cJSON *Lecture_Story_Json(Story *story) {
  const char *fichier = "data/story.json";
  cJSON *root = cJSON_ParseFromFile(fichier);

  if (root == NULL) {
    fprintf(stderr, "Erreur lors de l'analyse du fichier JSON\n");
    exit(-1); // Pas de fichier JSON acceptable
  }

  // Extraire la liste des chapitres
  cJSON *chapters = cJSON_GetObjectItem(root, "chapters");
  if (chapters == NULL) {
    fprintf(stderr, "Aucun chapitre trouvé dans le fichier JSON\n");
    cJSON_Delete(root);
    exit(-1);
  }

  // Parcours des chapitres
  int n = cJSON_GetArraySize(chapters);
  story->nbr_chapters = n;
  for (int i = 0; i < n; i++) {
    cJSON *chapter = cJSON_GetArrayItem(chapters, i);
    story->chapters[i].id_enemie = 0;
    // Récupérer le nom du chapitre
    cJSON *name = cJSON_GetObjectItem(chapter, "name");
    if (name != NULL && name->valuestring != NULL) {

      story->chapters[i].name = name->valuestring;
    }

    // Récupérer le texte du chapitre
    cJSON *txt = cJSON_GetObjectItem(chapter, "txt");
    if (txt != NULL && txt->valuestring != NULL) {
      story->chapters[i].txt = txt->valuestring;
    }

    // Récupérer les actions du chapitre
    cJSON *actions = cJSON_GetObjectItem(chapter, "actions");
    if (actions != NULL) {
      int m = cJSON_GetArraySize(actions);
      for (int j = 0; j < m; j++) {
        cJSON *action = cJSON_GetArrayItem(actions, j);
        if (action != NULL && action->valuestring != NULL) {
          story->chapters[i].action[j].description = action->valuestring;
        }
      }
    }

    printf("\n");
  }
  return root;
}

cJSON *Lecture_Items_JSON(Story *story) {
  const char *fichier = "data/items.json"; // Le chemin vers le fichier JSON
  cJSON *root = cJSON_ParseFromFile(fichier);

  if (root == NULL) {
    fprintf(stderr, "Erreur lors de l'analyse du fichier JSON des items\n");
    exit(-1); // Sortie si l'analyse échoue
  }

  // Extraire la section "items"
  cJSON *items = cJSON_GetObjectItem(root, "items");
  if (items == NULL) {
    fprintf(stderr, "Aucune donnée d'items trouvée dans le fichier JSON\n");
    cJSON_Delete(root);
    exit(-1);
  }

  int item_count = cJSON_GetArraySize(items);
  story->story_nb_items = item_count;

  // Parcourir chaque élément du tableau "items"
  for (int i = 0; i < item_count; i++) {
    cJSON *item = cJSON_GetArrayItem(items, i);

    // Récupérer les données de l'item
    cJSON *name = cJSON_GetObjectItem(item, "name");
    cJSON *description = cJSON_GetObjectItem(item, "description");
    cJSON *value = cJSON_GetObjectItem(item, "value");
    cJSON *type = cJSON_GetObjectItem(item, "type");

    if (name != NULL && description != NULL && value != NULL && type != NULL) {
      story->items[i].name = name->valuestring;
      story->items[i].description = description->valuestring;
      story->items[i].value = value->valueint;

      // Assigner le type (HEAL ou ATK) en fonction de la valeur de "type"
      if (strcmp(type->valuestring, "HEAL") == 0) {
        story->items[i].type = HEAL;
      } else if (strcmp(type->valuestring, "ATK") == 0) {
        story->items[i].type = ATK;
      } else {
        fprintf(stderr, "Type inconnu pour l'item : %s\n", name->valuestring);
        cJSON_Delete(root);
        exit(-1);
      }
    } else {
      fprintf(stderr, "Erreur dans les données de l'item\n");
      cJSON_Delete(root);
      exit(-1);
    }
  }
  return root;
}

cJSON *Lecture_Monstre_JSON(Story *story) {
  const char *fichier = "data/monsters.json";
  cJSON *root = cJSON_ParseFromFile(fichier);

  if (root == NULL) {
    cJSON_Delete(root);
    exit(-1);
  }

  cJSON *monsters = cJSON_GetObjectItem(root, "monsters");
  if (monsters == NULL) {
    cJSON_Delete(monsters);
    exit(-1);
  }
  int monster_count = cJSON_GetArraySize(monsters);
  story->nbr_monsters = monster_count;
  if (monster_count > MAX_MONSTER) {
    exit(-1);
  }

  for (int i = 0; i < monster_count; i++) {
    cJSON *monster = cJSON_GetArrayItem(monsters, i);

    cJSON *name = cJSON_GetObjectItem(monster, "name");
    cJSON *id_monstre = cJSON_GetObjectItem(monster, "id_monstre");
    cJSON *lp = cJSON_GetObjectItem(monster, "LP");
    cJSON *atk = cJSON_GetObjectItem(monster, "atk");
    cJSON *ascii_art = cJSON_GetObjectItem(monster, "ascii_art");

    if (name != NULL && id_monstre != NULL && lp != NULL && atk != NULL) {
      story->enemies[i].name = name->valuestring;

      story->enemies[i].id = id_monstre->valueint;
      story->enemies[i].lp = lp->valueint;
      story->enemies[i].atk = atk->valueint;
      story->enemies[i].ascii_art = ascii_art->valuestring;
    }
  }

  return root;
}

cJSON *Lecture_Personnage_JSON(Story *story, bool reprise) {
  char *fichier;
  if (!reprise) {
    fichier = "data/perso.json";
  } else {
    fichier = "data/save.json";
  }

  cJSON *root = cJSON_ParseFromFile(fichier);

  if (root == NULL) {
    fprintf(stderr, "Erreur lors de l'analyse du fichier JSON\n");
    exit(-1); // Sortie si l'analyse échoue
  }

  // Extraire la section "player"
  cJSON *player = cJSON_GetObjectItem(root, "player");
  if (player == NULL) {
    fprintf(stderr, "Aucune donnée de joueur trouvée dans le fichier JSON\n");
    cJSON_Delete(root);
    exit(-1);
  }

  // Récupérer le nom du joueur
  cJSON *name = cJSON_GetObjectItem(player, "name");
  if (name != NULL && name->valuestring != NULL) {
    story->player.name = name->valuestring; // Allouer et copier le nom
  } else {
    exit(-1);
  }
  // Récupérer les points de vie (LP) du joueur
  cJSON *lp = cJSON_GetObjectItem(player, "LP");
  if (lp != NULL) {
    story->player.lp = lp->valueint;
  } else {
    exit(-1);
  }

  // Récupérer l'attaque du joueur (atk)
  cJSON *atk = cJSON_GetObjectItem(player, "atk");
  if (atk != NULL) {
    story->player.atk = atk->valueint;
  } else {
    exit(-1);
  }
  cJSON *fuire = cJSON_GetObjectItem(player, "fuire");
  story->player.fuir = fuire->valueint;

  story->player.inv.nbrobj = 0;
  if (reprise) {
    cJSON *current_chapter = cJSON_GetObjectItem(player, "current_chapter");
    cJSON *score = cJSON_GetObjectItem(player, "score");
    story->current_chapter = current_chapter->valueint;
    story->score = score->valueint;
    cJSON *items_array = cJSON_GetObjectItem(player, "items");
    int num_items = cJSON_GetArraySize(items_array);
    story->player.inv.nbrobj = num_items;

    for (int i = 0; i < num_items; i++) {
      cJSON *item = cJSON_GetArrayItem(items_array, i);

      // Extraire les informations de chaque item
      cJSON *nameobj = cJSON_GetObjectItem(item, "name");
      cJSON *descriptionobj = cJSON_GetObjectItem(item, "description");
      cJSON *valueobj = cJSON_GetObjectItem(item, "value");
      cJSON *typeobj = cJSON_GetObjectItem(item, "type");
      story->player.inv.itemlist[i].name = nameobj->valuestring;
      story->player.inv.itemlist[i].description = descriptionobj->valuestring;
      story->player.inv.itemlist[i].type = typeobj->valueint;
      story->player.inv.itemlist[i].value = valueobj->valueint;
    }
  }
  return root;
}
void save_game_json(Story *story) {
  // Créer l'objet JSON principal
  cJSON *root = cJSON_CreateObject();

  // Ajouter les informations du joueur
  cJSON *player = cJSON_CreateObject();
  cJSON_AddStringToObject(player, "name", story->player.name);
  cJSON_AddNumberToObject(player, "LP", story->player.lp);
  cJSON_AddNumberToObject(player, "atk", story->player.atk);
  cJSON_AddNumberToObject(player, "fuire", story->player.fuir);
  cJSON_AddNumberToObject(player, "score", story->score);
  cJSON_AddNumberToObject(player, "current_chapter", story->current_chapter);

  // Ajouter les items du joueur
  cJSON *items_array = cJSON_CreateArray();
  for (int i = 0; i < story->player.inv.nbrobj; i++) {
    cJSON *item = cJSON_CreateObject();
    cJSON_AddStringToObject(item, "name", story->player.inv.itemlist[i].name);
    cJSON_AddStringToObject(item, "description",
                            story->player.inv.itemlist[i].description);
    cJSON_AddNumberToObject(item, "value", story->player.inv.itemlist[i].value);
    cJSON_AddNumberToObject(
        item, "type", story->player.inv.itemlist[i].type); // HEAL, ATK, etc.

    // Ajouter l'item à l'array des items
    cJSON_AddItemToArray(items_array, item);
  }

  // Ajouter l'array des items à la racine
  cJSON_AddItemToObject(player, "items", items_array);
  // Ajouter l'objet "player" à la racine
  cJSON_AddItemToObject(root, "player", player);

  // Convertir l'objet JSON en une chaîne
  char *json_string = cJSON_Print(root); // Formaté avec des indentations
  // Si vous voulez une chaîne compacte, utilisez `cJSON_PrintUnformatted`.

  // Écrire dans un fichier
  FILE *file = fopen("data/save.json", "w");
  if (file == NULL) {

    cJSON_Delete(root);
    free(json_string);
    return;
  }
  fprintf(file, "%s", json_string);
  fclose(file);

  cJSON_Delete(root);
  free(json_string);
  exit(1);
}
