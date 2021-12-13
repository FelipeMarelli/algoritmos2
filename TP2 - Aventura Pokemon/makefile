correr: compilar
	./tp2 

valgrind: compilar
	valgrind -s --leak-check=full --track-origins=yes --show-reachable=yes ./tp2

compilar: juego.c juego.h logica.c logica.h menu.c menu.h personaje.c personaje.h batallas.c batallas.h gimnasios.c gimnasios.h tdas/tda_heap.c tdas/tda_heap.h tdas/tda_lista.c tdas/tda_lista.h tdas/tda_abb.c tdas/tda_abb.h
	gcc juego.c logica.c menu.c batallas.c gimnasios.c personaje.c tdas/*.c -o tp2 -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
