#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <ncurses.h>

int MAX_X = 25, MAX_Y = 25;

int  majSnakeMap(int ** snake, int ** map, int x, int y, int ch, int tailleSnake) {
    int saveLastX = snake[tailleSnake-1][0];
    int saveLastY = snake[tailleSnake-1][1];
    for(int i=tailleSnake-1 ; i > 0 ; i--) {
        snake[i][0] = snake[i+1][0];
        snake[i][1] = snake[i+1][1];
    }

    snake[0][0] = x;
    snake[0][1] = y;


    if(map[x][y])
    {
        snake[tailleSnake][0] = saveLastX;
        snake[tailleSnake][1] = saveLastY;
        tailleSnake++;
        map[x][y] = 0;
    }

    return tailleSnake;
}







int main() {
    int ch, saveCh;
    int x = 1, y = 1; // head
    int ** map = (int**)malloc((MAX_X-1) * sizeof(int*));
    for (int i = 0; i < (MAX_X-1); i++) {
        map[i] = (int*)calloc(MAX_Y-1, sizeof(int));
    }
    map[10][10] = 1;
     map[10][15] = 1;

    int ** snake = (int**)malloc((MAX_X-1) * (MAX_Y-1) * sizeof(int*));
    for (int i = 0; i < (MAX_X-1) * (MAX_Y-1); i++) {
        snake[i] = (int*)calloc(2, sizeof(int));
    }
    snake[0][0] = x;
    snake[0][1] = y;
    int tailleSnake = 1;




    initscr();            // Initialise l'écran
    raw();               // Désactive le buffering des entrées
    keypad(stdscr, TRUE); // Permet l'utilisation des touches spéciales
    noecho();             // Désactive l'affichage des entrées utilisateur
    curs_set(0);          // Cache le curseur


    resizeterm(MAX_X, MAX_Y);
	mvprintw(y, x, "X");
    

    saveCh = KEY_RIGHT;
    timeout(500); // wait 500ms for the input of getch
    while((ch = getch()) != 'q') { // Appuyer sur 'q' pour quitter
    
        if(ch==ERR){ // touche pas pressée

            ch = saveCh;
        } 
        else{
            saveCh = ch;
        }
        switch(ch) {
            case KEY_UP:
                y = (y > 1) ? y - 1 : MAX_Y - 1 -1;
                break;
            case KEY_DOWN:
                y = (y < MAX_Y - 1 -1) ? y + 1 : 1;
                break;
            case KEY_LEFT:
                x = (x > 1) ? x - 1 : MAX_X - 1 -1;
                break;
            case KEY_RIGHT:
                x = (x < MAX_X - 1 -1) ? x + 1 : 1;
                break;

        }

        tailleSnake = majSnakeMap(snake, map, x, y, ch, tailleSnake);


        clear();

        // Affichage de la carte:
        for (int i = 0; i < (MAX_X-1); i++) {
            for(int j = 0; j < (MAX_Y-1); j++){
                if(map[i][j] == 1)
                    mvprintw(j, i, "O");
            }
        }


        // Affihage du serpent
        for(int i=0 ; i < tailleSnake ; i++) {
            mvprintw(snake[i][1], snake[i][0], "X");
            printf("snake[i][1]: %d ; snake[i][0]: %d \r\n", snake[i][1], snake[i][0]);
        }
        
        
        // box pour les bords
        mvhline(0, 0, '-', MAX_X);
        mvhline(MAX_X-1, 0, '-', MAX_X);
        mvvline(0, MAX_X-1, '|', MAX_X);
        mvvline(0, 0, '|', MAX_X);
        

        refresh();
    }

    endwin(); // Restaure les paramètres du terminal

    /* free map */
    for (int i = 0; i < MAX_X; i++) {
        free(map[i]);
    }
    free(map);

    /* free snake */
    for (int i = 0; i < (MAX_X-1) * (MAX_Y-1); i++) {
        free(snake[i]);
    }
    free(snake);
    printf("taille: %d \r\n", tailleSnake);
    for(int i=0 ; i < tailleSnake ; i++) {
            mvprintw(snake[i][1], snake[i][0], "X");
            printf("snake[i][1]: %d ; snake[i][0]: %d \r\n", snake[i][1], snake[i][0]);
        }
    return 0;
}
