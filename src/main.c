#include "../included/generation_game.h"
#include "../included/moteur.h"

void initialisation_couleur() {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
}

void initialisation_jeu(Story *story, Free *liberer, bool reprendre) {
  story->score = 0;
  liberer->Jstory = Lecture_Story_Json(story);
  liberer->Jitems = Lecture_Items_JSON(story);
  liberer->Jplayer = Lecture_Personnage_JSON(story, reprendre);
  liberer->Jmonster = Lecture_Monstre_JSON(story);
}

void afficher_bandeau(WINDOW *bandeau, int chapitre, char *chapter_name,
                      char *name, int score, int lp, int atk) {
  // Effacer la fenêtre du bandeau pour éviter des artefacts visuels
  werase(bandeau);

  // Dimensions de la fenêtre
  size_t largeur = (size_t)getmaxx(bandeau);
  char texte[largeur];
  // Je fais en sorte que la position du bandeau soit la meme quelque soit la
  // taille du terminale
  int score_space = (int)(largeur * 0.35);
  int name_space = (int)(largeur * 0.30);
  // Créer la chaîne formatée pour s'adapter à la largeur
  snprintf(texte, largeur,
           "Chapitre %d : %s  %*sScore: %d  %*sNom: %s  LP: %d  ATK: %d",
           chapitre, chapter_name, score_space, "", score, name_space, "", name,
           lp, atk);

  // Afficher le texte dans le bandeau
  mvwprintw(bandeau, 0, 1, texte);
  // Rafraîchir la fenêtre du bandeau
  wrefresh(bandeau);
}

int main() {
  Story *story = malloc(sizeof(Story));
  Free *liberer = malloc(sizeof(Free));

  srand(time(NULL));
  initscr();
  setlocale(LC_ALL, "");
  curs_set(0);
  initialisation_couleur();

  // Dimensions de l'écran

  int hauteur, largeur;
  getmaxyx(stdscr, hauteur, largeur);
  story->screen.hauteur = hauteur;
  story->screen.largeur = largeur;
  story->screen.bandeau = newwin(1, largeur, 0, 0);

  story->screen.jeu = newwin(hauteur - 1, largeur, 1, 0);
  unsigned int choix = 0;
  do {
    // Affichage du menu dans la fenêtre de jeu
    werase(story->screen.jeu); // Effacer le contenu précédent de la fenêtre jeu
    mvwprintw(story->screen.jeu, 1, 5, "MENU");
    mvwprintw(story->screen.jeu, 2, 5, "1. Lancement de l'histoire");
    mvwprintw(story->screen.jeu, 3, 5, "2. Reprendre la sauvegarde");
    mvwprintw(story->screen.jeu, 4, 5, "3. Quittez");
    mvwprintw(story->screen.jeu, 5, 5, "Votre choix: ");

    wrefresh(story->screen.jeu);
    wrefresh(story->screen.bandeau); // Rafraîchir les deux fenêtres

    if (wscanw(story->screen.jeu, "%u", &choix) != 1) {
      mvwprintw(story->screen.jeu, 7, 5, "Erreur : entrer une option valide");
      wrefresh(story->screen.jeu);

      continue;
    }
    switch (choix) {
    case 1: {
      werase(story->screen.jeu);
      initialisation_jeu(story, liberer, false);
      Lancement_Partie(story, false);

      break;
    }
    case 2: {
      werase(story->screen.jeu);
      initialisation_jeu(story, liberer, true);
      Lancement_Partie(story, true);
      break;
    }
    case 3: {
      werase(story->screen.jeu);
      printw("Partie quittée");
      break;
    }
    default: {
      printw("Option invalide \n");
      break;
    }
    }
  } while (choix != 1 && choix != 2 && choix != 3);
  if (choix != 3) {
    cJSON_Delete(liberer->Jitems);
    cJSON_Delete(liberer->Jplayer);
    cJSON_Delete(liberer->Jstory);
    cJSON_Delete(liberer->Jmonster);
  }
  delwin(story->screen.bandeau);
  delwin(story->screen.jeu);
  free(liberer);
  free(story);
  endwin();

  return 0;
}
