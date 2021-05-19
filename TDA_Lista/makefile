correr: compilar
	./lista_se

valgrind: compilar
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./lista_se

compilar: lista.c pruebas.c lista.h pa2mm.h
	gcc lista.c pruebas.c lista.h pa2mm.h -o lista_se -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0



