#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <ncurses.h>


// var glob
const int MAX_X = 25; 
const int MAX_Y = 25;
int POSX_BONBON = MAX_X/2;
int POSY_BONBON = MAX_Y/2;
char ** MAP; // "b" = bonbon, "x" = cell/snake, "*" nothing 

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
void majScoreSnake(snake *mySnake, int newPosHeadX, int newPosHeadY);
void majSnakeMap(snake *mySnake, int newPosHeadX, int newPosHeadY);
void mapInit(void);
void randomBonbon(void);

// debug
void debugPrintSnake(snake *mySnake);
void debugPrint(const char *toPrint);



// définitions:
void debugPrint(const char *toPrint)
{
    // Ouvrir un fichier pour écrire les résultats
    FILE *file = fopen("debugHelp.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return;
    } 

    fprintf(file, toPrint);
    
    fprintf(file, "\r\n");

    // Fermer le fichier
    fclose(file);
}



void debugPrintSnake(snake *mySnake)
{
    // Ouvrir un fichier pour écrire les résultats
    FILE *file = fopen("debugHelp.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Erreur d'ouverture du fichier.\n");
        return;
    } 

    cell *curCell = mySnake->head;
    fprintf(file, "{cell:%p, x:%d, y:%d} --> ", curCell, curCell->x, curCell->y);
    while(curCell->next){
        fprintf(file, "{cell:%p, x:%d, y:%d} --> ", curCell, curCell->x, curCell->y);
        curCell = curCell->next;
    }
    fprintf(file, "\r\n");

    // Fermer le fichier
    fclose(file);

}


void randomBonbon(void) {
    if(POSY_BONBON && POSX_BONBON)
        return;
    POSX_BONBON = (rand() % MAX_X-2) + 1; // Générer un entier entre 1 et MAX_X-2
    POSY_BONBON = (rand() % MAX_Y-2) + 1; // Générer un entier entre 1 et MAX_Y-2

}


void mapInit(void)
{
    // init map
    MAP = (char**)malloc((MAX_X) * sizeof(char*)); // -2 pour les walls
    for (int i = 0; i < (MAX_X); i++) {
        MAP[i] = (char*)malloc((MAX_Y)*sizeof(char));
        for(int j = 0; j<MAX_Y; j++)
            MAP[i][j] = '*'; // rien
    }
}



void majScoreSnake(snake *mySnake, int newPosHeadX, int newPosHeadY){
    if(newPosHeadX == POSX_BONBON && newPosHeadY == POSY_BONBON){
        cell *newCell = (cell*)malloc(sizeof(newCell));
        newCell->x = 0;
        newCell->y = 0;
        newCell->next = NULL;

        cell *curCell = mySnake->head;
        while(curCell->next){
            curCell = curCell->next;
        }
        curCell->next = newCell;

        mySnake->size++;

        POSX_BONBON = 0; // NULL
        POSY_BONBON = 0; // NULL

        return;
    }

    if(MAP[newPosHeadX][newPosHeadY] = 'x') // collision
        exit(1);
}

void majSnakeMap(snake *mySnake, int newPosHeadX, int newPosHeadY){

    int newX = newPosHeadX;
    int newY = newPosHeadY;
    int lastX = mySnake->head->x;
    int lastY = mySnake->head->y;
    
    cell *currentCell = mySnake->head;
    while(currentCell)
    {
        MAP[newX][newY] = 'x';
        

        lastX = mySnake->head->x;
        lastY = mySnake->head->y;

        MAP[lastX][lastY] = '*'; // l'ancienne position est libre

        currentCell->x = newX;
        currentCell->y = newY;

        newX = lastX;
        newY = lastY;

        currentCell = currentCell->next;
    }

}


int main() {

    // map init
    mapInit();


    // snake init
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
    timeout(100); // wait 500ms for the input of getch
    resizeterm(MAX_X, MAX_Y);
	
    int ch, saveCh;

    saveCh = 261; // dir de base = droite
    ch = 410; // err de base

    while((ch = getch()) != 'q') { // Appuyer sur 'q' pour quitter
        int x = (mySnake.head)->x;
        int y = (mySnake.head)->y;
        
        if(ch==ERR || ch==410){ // touche pas pressée

            ch = saveCh;
            // printf("ERR(savech:%d ch:%d) ", saveCh, ch);
        } 
        else{
            // printf("OK(savech:%d ch:%d) ", saveCh, ch);
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
        majScoreSnake(&mySnake, x, y); // if bobon eaten = score ++
        randomBonbon(); // update bonbon pos
        majSnakeMap(&mySnake, x, y);





        // reste aff
        clear();

        // Affichage de la carte:
        for (int i = 1; i < MAX_X-1; i++) {
            for(int j = 1; j < MAX_Y-1; j++){
                if(MAP[i][j] == 'x')
                    mvprintw(j, i, "x");
            }
        }

        // bonbon:
        mvprintw(POSY_BONBON, POSX_BONBON, "b");
        
        // box pour les bords
        mvhline(0, 0, '-', MAX_X);
        mvhline(MAX_X-1, 0, '-', MAX_X);
        mvvline(0, MAX_X-1, '|', MAX_X);
        mvvline(0, 0, '|', MAX_X);


        // debug:
        debugPrintSnake(&mySnake);
        

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
