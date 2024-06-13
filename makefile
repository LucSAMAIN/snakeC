all:
	gcc ./snake.c -o snake -lncurses -g
	> ./debugHelp.txt
	> ./debugSnake.txt
	> ./debugMap.txt

