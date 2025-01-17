#include "../included/moteur.h"
#include <ncurses.h>

void afficher_suivant(Screen screen) {
  wattron(screen.jeu, A_BOLD);
  mvwprintw(screen.jeu, 30, screen.largeur - 50, "Continuez ... ");
  wattroff(screen.jeu, A_BOLD);
  wrefresh(screen.jeu);
  getchar();
}

void Lancement_Partie(Story *story, bool reprise) {
  werase(story->screen.jeu);

  int i = 0;
  int result = 0;
  // Chapitre 0 : Présentation du personnage
  if (!reprise) {
    afficher_bandeau(story->screen.bandeau, 0, story->chapters[0].name,
                     story->player.name, 0, story->player.lp,
                     story->player.atk);
    mvwprintw(story->screen.jeu, 10, 0, "%s", story->chapters[i].txt);
    wrefresh(story->screen.jeu);
    afficher_suivant(story->screen);

    GetItems(story, 2);
    story->chapters[i].id_enemie = -1;
    story->current_chapter = 1;
  } else {
    afficher_bandeau(story->screen.bandeau, story->current_chapter,
                     story->chapters[0].name, story->player.name, 0,
                     story->player.lp, story->player.atk);
  }
  Information_Player(story);
  wrefresh(story->screen.jeu);

  bool actions;
  // CHARGEMENT CHAPITRE PAR CHAPITRE
  for (i = story->current_chapter; i < story->nbr_chapters; i++) {
    story->current_chapter = i;
    afficher_bandeau(story->screen.bandeau, story->current_chapter,
                     story->chapters[i].name, story->player.name, story->score,
                     story->player.lp, story->player.atk);
    actions = ChoixVoie(story);
    if (actions) {
      GetItems(story, rand() % (3));
    } else if (!actions) {
      GetItems(story, 3 + rand() % (3));
    }

    if (story->chapters[i].id_enemie >= 0) {

      result = Combat(story, rand() % (story->nbr_monsters));
      if (result == 0 || result == 1) {
        GetItems(story, rand() % (1 + story->story_nb_items));

      } else {
        werase(story->screen.jeu);
        werase(story->screen.bandeau);
        wrefresh(story->screen.bandeau);
        wrefresh(story->screen.jeu);
        const char *endgame =
            "\n\n\t\t\t\t\t\t\t\t\t\t GGGGG   AAAAA  MMMMM  EEEEE    OOO   V "
            "  V  EEEEE  RRRRR\n\t\t\t\t\t\t\t\t\t\tG      "
            "  A   A  M   M  E       O   O  V   V  E      R  "
            " R\n\t\t\t\t\t\t\t\t\t\tG  GG    "
            "AAAAA  M   M  EEEE    O   O  V   V  EEEE   "
            "RRRRR\n\t\t\t\t\t\t\t\t\t\tG   G    A   A  "
            "M   M  E       O   O   V V   E      R  "
            "R\n\t\t\t\t\t\t\t\t\t\t GGGG    A   A  M   M  "
            "EEEEE    OOO     V    EEEEE  R   R";
        wattron(story->screen.jeu, COLOR_PAIR(1));
        wprintw(story->screen.jeu, "%s", endgame);
        wattroff(story->screen.jeu, COLOR_PAIR(1));
        mvwprintw(story->screen.jeu, 10, 30, "Vous avez parcouru %d chapitres",
                  story->current_chapter);
        mvwprintw(story->screen.jeu, 11, 30, "Vous avez obtenu un score de %d",
                  story->score);

        afficher_suivant(story->screen);
        return;
      }
    }
  }
  werase(story->screen.jeu);
  wrefresh(story->screen.jeu);
  const char *victory = "\n\n"
                        "\t\t\t\t\t\t\tV       V   IIIIII   CCCCC   TTTTTTT   "
                        "OOOOO   RRRRR    Y     Y\n"
                        "\t\t\t\t\t\t\t V     V      II    C         T      O  "
                        "   O  R    R    Y   Y \n"
                        "\t\t\t\t\t\t\t  V   V       II    C         T      O  "
                        "   O  RRRRR      YYY  \n"
                        "\t\t\t\t\t\t\t   V V        II    C         T      O  "
                        "   O  R   R       Y   \n"
                        "\t\t\t\t\t\t\t    V      IIIIII   CCCCC     T       "
                        "OOOOO   R    R      Y   ";
  wattron(story->screen.jeu,
          COLOR_PAIR(2)); // Utiliser une couleur différente si souhaité
  wprintw(story->screen.jeu, "%s", victory);
  wattroff(story->screen.jeu, COLOR_PAIR(2));

  mvwprintw(story->screen.jeu, 10, 30, "Vous avez parcouru %d chapitres",
            story->current_chapter);
  mvwprintw(story->screen.jeu, 11, 30, "Vous avez obtenu un score de %d",
            story->score);
  afficher_suivant(story->screen);
}
