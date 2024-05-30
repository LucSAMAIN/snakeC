#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <ncurses.h>


// var glob
int MAX_X = 25, MAX_Y = 25;
char ** MAP;

typedef struct cell{
    struct cell *next;
    int x;
    int y;
} cell;

typedef struct snake{
    cell *head;
    int size;
} snake;


// déclaration

void majScore(snake *mySnake, int newPosHeadX, int newPosHeadY);
void majSnakeMap(snake *mySnake, int newPosHeadX, int newPosHeadY);






void majSnakeMap(snake *mySnake, int newPosHeadX, int newPosHeadY){

    majScore(mySnake, newPosHeadX, newPosHeadY);

    int newX = newPosHeadX;
    int newY = newPosHeadY;
    int lastX = mySnake->head->x;
    int lastY = mySnake->head->y;
    
    cell *currentCell = mySnake->head;
    while(currentCell)
    {
        MAP[currentCell->x-1][currentCell->y-1] = 'x';  // la map commence en 0.0 pour le point 1.1

        lastX = mySnake->head->x;
        lastY = mySnake->head->y;

        currentCell->x = newX;
        currentCell->y = newY;

        newX = lastX;
        newY = lastY;

        currentCell = currentCell->next;
    }

}

void majScore(snake *mySnake, int newPosHeadX, int newPosHeadY){
    if(MAP[newPosHeadX][newPosHeadY] == 1){
        cell *newCell = (cell*)malloc(sizeof(newCell));

        cell *curCell = mySnake->head;
        while(curCell->next){
            curCell = curCell->next;
        }
        curCell->next = newCell;

        mySnake->size++;
    }
}




int main() {
    // variables:
    int ch, saveCh;

    MAP = (char**)malloc((MAX_X-2) * sizeof(char*)); // -2 pour les walls
    for (int i = 0; i < (MAX_X-2); i++) {
        MAP[i] = (char*)malloc((MAX_Y-2)*sizeof(char));
        for(int j = 0; j<MAX_Y-2; j++)
            MAP[i][j] = '*';
    }

    snake mySnake;
    cell *firstCell = (cell*)malloc(sizeof(cell));
    firstCell->next = NULL;
    firstCell->x = 1;
    firstCell->y = 1;

    mySnake.head = firstCell;
    mySnake.size = 1;



    // ncurses:
    initscr();            // Initialise l'écran
    raw();               // Désactive le buffering des entrées
    keypad(stdscr, TRUE); // Permet l'utilisation des touches spéciales
    noecho();             // Désactive l'affichage des entrées utilisateur
    curs_set(0);          // Cache le curseur
    timeout(500); // wait 500ms for the input of getch
    resizeterm(MAX_X, MAX_Y);
	
    

    saveCh = KEY_RIGHT; // dir de base
    while((ch = getch()) != 'q') { // Appuyer sur 'q' pour quitter
        int x = (mySnake.head)->x;
        int y = (mySnake.head)->y;
        if(ch==ERR){ // touche pas pressée

            ch = saveCh;
        } 
        else{
            saveCh = ch;
        }
        switch(ch) {
            case KEY_UP:
                y = (y > 1) ? y-1 : MAX_Y-2;
                break;
            case KEY_DOWN:
                y = (y < MAX_Y-2) ? y+1 : 1;
                break;
            case KEY_LEFT:
                x = (x > 1) ? x-1 : MAX_X-2;
                break;
            case KEY_RIGHT:
                x = (x < MAX_X-2) ? x+1 : 1;
                break;

        }


        // maj variable
        majSnakeMap(&mySnake, x, y);





        // rester aff
        clear();

        // Affichage de la carte:
        for (int i = 0; i < (MAX_X-2); i++) {
            for(int j = 0; j < (MAX_Y-2); j++){
                if(MAP[i][j] == 'x')
                    mvprintw(j+1, i+1, "x");
            }
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
        free(MAP[i]);
    }
    free(MAP);

    /* free snake */
    firstCell = mySnake.head;
    cell *nextCell = mySnake.head->next;
    while(nextCell){
        free(firstCell);
        firstCell = nextCell;
        nextCell = nextCell->next;
    }
    free(firstCell);

    return 0;
}
