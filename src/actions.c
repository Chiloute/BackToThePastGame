#include "../included/moteur.h"

void Information_Player(Story *story) {
  werase(story->screen.jeu);

  Player *player = &story->player;

  unsigned int choix = 0;
  do {
    werase(story->screen.jeu);
    mvwprintw(story->screen.jeu, 1, 10, "Nom : %s", player->name);
    mvwprintw(story->screen.jeu, 2, 10, "Point de vie : %d \n", player->lp);
    mvwprintw(story->screen.jeu, 3, 10, "Point d'attaque: %d \n", player->atk);
    mvwprintw(story->screen.jeu, 5, 12, "Objets : \n");
    mvwprintw(story->screen.jeu, 6, 10, "Vous avez : %d objects",
              player->inv.nbrobj);
    //  FONCTION pour afficher object
    afficher_object(story);
    mvwprintw(story->screen.jeu, 7 + player->inv.nbrobj, 10,
              "1. Utilisez un objet: \n");
    mvwprintw(story->screen.jeu, 8 + player->inv.nbrobj, 10,
              "2. Quittez l'inventaire \n");
    mvwprintw(story->screen.jeu, 9 + player->inv.nbrobj, 10, "3. Sauvegarder");
    mvwprintw(story->screen.jeu, 10 + player->inv.nbrobj, 10,
              "Quel action souhaitez-vous faire ? ");
    wrefresh(story->screen.jeu);
    if (wscanw(story->screen.jeu, "%u", &choix) != 1) {
      choix = 0;
      continue;
    }

    switch (choix) {
      // UTILISATION OBJETS
    case 1: {
      werase(story->screen.jeu);
      if (player->inv.nbrobj == 0) {
        wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(story->screen.jeu, 6 + player->inv.nbrobj,
                  story->screen.largeur - 100, "Vous n'avez pas d'objet \n");
        wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        wrefresh(story->screen.jeu);
      } else {
        werase(story->screen.jeu);
        int object = 0;
        afficher_object(story);
        mvwprintw(story->screen.jeu, 12 + player->inv.nbrobj, 5,
                  "Quel objet voulez-vous utiliser ?(Pressez entrer pour ne "
                  "rien faire) : ");
        wrefresh(story->screen.jeu);
        if (wscanw(story->screen.jeu, "%d", &object) != 1) {

          object = 0;
        }
        // VRAI UTILISATION LORSQUE LES OBJECT SONT BON
        if (object > 0 && object <= player->inv.nbrobj) {
          UtilisationItems(story, object - 1);
          wattron(story->screen.jeu, COLOR_PAIR(2) | A_BOLD);
          mvwprintw(story->screen.jeu, 14 + player->inv.nbrobj, 5,
                    "Vous avez utilisé %s (+%d points)\n",
                    player->inv.itemlist[object - 1].name,
                    player->inv.itemlist[object - 1].value);

          for (int i = object - 1; i < player->inv.nbrobj; i++) {
            player->inv.itemlist[i] = player->inv.itemlist[i + 1];
          }
          player->inv.nbrobj--;
          wattroff(story->screen.jeu, COLOR_PAIR(2) | A_BOLD);
        } else {
          wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
          mvwprintw(story->screen.jeu, 15 + player->inv.nbrobj, 5,
                    "Objet invalide \n");
          wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        }
      }
      afficher_bandeau(story->screen.bandeau, story->current_chapter,
                       story->chapters[story->current_chapter].name,
                       story->player.name, 0, story->player.lp,
                       story->player.atk);

      afficher_suivant(story->screen);
      werase(story->screen.jeu);
      wrefresh(story->screen.jeu);
      break;
    }
    case (3): {
      save_game_json(story);
    }
    default: {
      wattron(story->screen.jeu, COLOR_PAIR(1));
      mvwprintw(story->screen.jeu, 22 + player->inv.nbrobj, 10,
                "Erreur entrée invalide ! ");
      wattron(story->screen.jeu, COLOR_PAIR(1));
      wrefresh(story->screen.jeu);
      break;
    }
    }
  } while (choix != 2);
  werase(story->screen.jeu);
  wrefresh(story->screen.jeu);
}

int Combat(Story *story, int monster) {
  werase(story->screen.jeu);
  wrefresh(story->screen.jeu);
  Bestiaire enemies = story->enemies[monster];
  Player *player = &story->player;

  do {
  revenir_au_choix:
    werase(story->screen.jeu);
    afficher_bandeau(story->screen.bandeau, story->current_chapter,
                     story->chapters[story->current_chapter].name,
                     story->player.name, story->score, story->player.lp,
                     story->player.atk);
    mvwprintw(story->screen.jeu, 5, 5, "Vous êtes en combat contre : ");
    wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(story->screen.jeu, 5, 35, "[%s]", enemies.name);
    wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
    mvwprintw(story->screen.jeu, 6, 5, "Point de vie : %d \n", enemies.lp);
    mvwprintw(story->screen.jeu, 6, 60, "      /(.-\"\"-.)\\");
    mvwprintw(story->screen.jeu, 7, 60, "  |\\  \\/      \\/  /|");
    mvwprintw(story->screen.jeu, 8, 60, "  | \\ / =.  .= \\ / |");
    mvwprintw(story->screen.jeu, 9, 60, "\\( \\   o\\/o   / )/");
    mvwprintw(story->screen.jeu, 10, 60, "   \\_, '-/  \\-' ,_/");
    mvwprintw(story->screen.jeu, 11, 60, "     /   \\__/   \\");
    mvwprintw(story->screen.jeu, 12, 60, "     \\ \\__/--\\__/ /");
    mvwprintw(story->screen.jeu, 13, 60, "   ___\\    --    /___");
    mvwprintw(story->screen.jeu, 14, 60, " /`    '--'  '--'    `\\");
    mvwprintw(story->screen.jeu, 7, 5, "Point d'attaque : %d \n", enemies.atk);
    unsigned int choix = 0;
    do {

      afficher_bandeau(story->screen.bandeau, story->current_chapter,
                       story->chapters[story->current_chapter].name,
                       story->player.name, story->score, story->player.lp,
                       story->player.atk);

      mvwprintw(story->screen.jeu, 9, 10, "1. Attaquer \n");
      mvwprintw(story->screen.jeu, 10, 10, "2. Inventaire");
      mvwprintw(story->screen.jeu, 11, 10, "3. Fuir (restant:%d) \n",
                story->player.fuir);
      mvwprintw(story->screen.jeu, 12, 10, "Action: ");
      wrefresh(story->screen.jeu);
      if (wscanw(story->screen.jeu, "%d", &choix) != 1) {
        wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(story->screen.jeu, 20, 50,
                  "Erreur : entrer une option valide \n");
        wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        goto revenir_au_choix;
      }

      switch (choix) {
        // ATTAQUE
      case 1: {
        werase(story->screen.jeu);
        wrefresh(story->screen.jeu);
        enemies.lp -= player->atk;
        if (enemies.lp > 0) {
          player->lp -= enemies.atk;
        }
        mvwprintw(story->screen.jeu, 12, 10, "Vous avez attaqué %s \n",
                  enemies.name);
        mvwprintw(story->screen.jeu, 13, 10,
                  "Vous avez infligé %d points de dégats \n", player->atk);
        mvwprintw(story->screen.jeu, 14, 10,
                  "Il vous reste %d points de vie \n", player->lp);

        afficher_suivant(story->screen);
        werase(story->screen.jeu);
        wrefresh(story->screen.jeu);

        break;
      }
        // INVENTAIRE
      case 2: {
        Information_Player(story);
        break;
      }
        // FUITE
      case 3: {
        werase(story->screen.jeu);
        if (player->fuir == 0) {
          wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
          mvwprintw(story->screen.jeu, 4, 10,
                    "Fuite impossible car il vous reste %d skip  !",
                    player->fuir);
          wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);

          afficher_suivant(story->screen);
          werase(story->screen.jeu);
          wrefresh(story->screen.jeu);
          break;
        }
        player->fuir--;
        wattron(story->screen.jeu, A_BOLD);
        mvwprintw(story->screen.jeu, 4, 10,
                  "Vous avez fui le combat, vous perdez %d points \n",
                  (int)(story->score * 0.8));
        wattroff(story->screen.jeu, A_BOLD);

        int dammage = (int)(player->lp * 0.45);
        mvwprintw(story->screen.jeu, 5, 10, "Vous avez %d \n", player->lp);
        wattron(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(story->screen.jeu, 5, 24, "(-%d)", dammage);
        wattroff(story->screen.jeu, COLOR_PAIR(1) | A_BOLD);
        mvwprintw(story->screen.jeu, 5, 29, "points de vie");
        player->lp = dammage;
        afficher_suivant(story->screen);
        return 0;
        break;
      }
      }
    } while (choix != 1 && choix != 2);

  } while (enemies.lp > 0 && player->lp > 0);
  if (enemies.lp <= 0) {
    int score = rand() % (1 + 150);
    story->score += score;
    mvwprintw(story->screen.jeu, 5, 10, "Vous avez vaincu %s \n", enemies.name);
    mvwprintw(story->screen.jeu, 6, 10, "Vous avez gagnez (+%d) points", score);
    afficher_suivant(story->screen);
    return 1;
  }
  printw("Vous avez été vaincu par %s \n", enemies.name);
  return -1;
}
void afficher_object(Story *story) {
  Player *player = &story->player;
  if (player->inv.nbrobj > 0) { // Vérifie qu'il y a au moins un objet
    for (int i = 0; i < player->inv.nbrobj; i++) { // Parcourt chaque objet
      mvwprintw(story->screen.jeu, 6 + i, 10, "[%d] %s (+%d)", i + 1,
                player->inv.itemlist[i].name, player->inv.itemlist[i].value);

      // mvwprintw(story->screen.jeu, 7 + i, 10, "- %s",
      //           player->inv.itemlist[i].description);
    }
  } else {
    mvwprintw(story->screen.jeu, 6, 10, "Aucun objet dans l'inventaire.");
  }
}

void GetItems(Story *story, int item_id) {
  werase(story->screen.jeu);

  if (story->player.inv.nbrobj >= MAX_ITEMS) {
    wattron(story->screen.jeu, COLOR_PAIR(1));
    mvwprintw(story->screen.jeu, 5, 10,
              "Vous avez atteint le nombre d'object maximale");
    wattroff(story->screen.jeu, COLOR_PAIR(1));
    wrefresh(story->screen.jeu);
    afficher_suivant(story->screen);
    return;
  } else {

    Player *player = &story->player;
    player->inv.itemlist[player->inv.nbrobj] = story->items[item_id];

    mvwprintw(
        story->screen.jeu, 5, 10,
        "Vous avez obtenus [%s] \n Description : %s \n Point d'effet: %d\n",
        story->items[item_id].name, story->items[item_id].description,
        story->items[item_id].value);
    story->player.inv.nbrobj++;
  }
  afficher_suivant(story->screen);
}

void UtilisationItems(Story *story, int itemNumber) {
  Player *player = &story->player;
  Items item = player->inv.itemlist[itemNumber];
  if (item.type == ATK) {
    player->atk += item.value;
  } else if (item.type == HEAL) {
    player->lp += item.value;
  }
}

bool ChoixVoie(Story *story) {
  int choix = -1;

  do {
    werase(story->screen.jeu);
    wattroff(story->screen.jeu, COLOR_PAIR(1));
    mvwprintw(story->screen.jeu, 5, 10, "[0] %s",
              story->chapters[story->current_chapter].action[0].description);
    mvwprintw(story->screen.jeu, 6, 10, "[1] %s",
              story->chapters[story->current_chapter].action[1].description);

    mvwprintw(story->screen.jeu, 8, 10, "Choisissez votre voie : ");
    wrefresh(story->screen.jeu);
    if (wscanw(story->screen.jeu, "%d", &choix) != 1) {
      werase(story->screen.jeu);
      choix = -1;
    } else if (choix != 0 && choix != 1) {
      choix = -1;
    }
  } while (choix == -1);
  if (choix) {
    return true;
  }
  return false;
}
