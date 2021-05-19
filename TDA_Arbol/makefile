correr: compilar
	./abb

valgrind: compilar
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb

compilar: abb.c pruebas.c abb.h pa2mm.h
	gcc abb.c pruebas.c abb.h pa2mm.h -o abb -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
