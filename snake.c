#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <ncurses.h>


int main() {
    int ch;
    int x = 0, y = 0;

    initscr();            // Initialise l'écran
    raw();               // Désactive le buffering des entrées
    keypad(stdscr, TRUE); // Permet l'utilisation des touches spéciales
    noecho();             // Désactive l'affichage des entrées utilisateur
    curs_set(0);          // Cache le curseur

	mvprintw(y, x, "X");

    while((ch = getch()) != 'q') { // Appuyer sur 'q' pour quitter
        switch(ch) {
            case KEY_UP:
                y = (y > 0) ? y - 1 : LINES - 1;
                break;
            case KEY_DOWN:
                y = (y < LINES - 1) ? y + 1 : 0;
                break;
            case KEY_LEFT:
                x = (x > 0) ? x - 1 : COLS - 1;
                break;
            case KEY_RIGHT:
                x = (x < COLS - 1) ? x + 1 : 0;
                break;

        }
        clear();
        mvprintw(y, x, "X");
        refresh();
    }

    endwin(); // Restaure les paramètres du terminal
    return 0;
}
