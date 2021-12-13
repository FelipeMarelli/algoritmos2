correr: compilar
	./evento_pesca

compilar: main.c evento_pesca.c
	gcc *.c -Wall -Werror -Wconversion -std=c99 -o evento_pesca

valgrind: compilar
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./evento_pesca
