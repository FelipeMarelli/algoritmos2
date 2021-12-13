#ifndef __JUEGO_H__
#define __JUEGO_H__

#include "tdas/tda_abb.h"
#include "tdas/tda_heap.h"
#include "tdas/tda_lista.h"

#define MASCARA_POKEMON "P; %50[a-zA-Z0-9 ]; %2[0-9]; %2[0-9]; %2[0-9]\n"
#define MASCARA_ENTRENADOR "%*[LE]; %50[a-zA-Z0-9 ]\n"
#define MASCARA_PERSONAJE "E; %50[a-zA-Z0-9 ]\n"
#define MASCARA_GIMNASIO "G; %50[a-zA-Z0-9 ]; %2[0-9]; %2[0-9]\n"

#define MAX_NOMBRE 51
#define ERROR -1
#define EXITO 1

typedef int (*funcion_batalla)(void*, void*);

typedef struct personaje {
    char nombre[MAX_NOMBRE];
    abb_t* poke_obtenidos;
    lista_t* poke_combate;
} personaje_t;

typedef struct pokemon {
    char nombre[MAX_NOMBRE];
    int velocidad;
    int defensa;
    int ataque;
    int mejora;
    int id;
} pokemon_t;

typedef struct entrenador_s {
    lista_t* pokemones;
    char nombre[MAX_NOMBRE];
} entrenador_t;

typedef struct gimnasio_s {
    char nombre[MAX_NOMBRE];
    int dificultad;
    funcion_batalla fn_batalla;
    lista_t* entrenadores;
} gimnasio_t;

personaje_t* inicializar_personaje();
void destruir_personaje(personaje_t* personaje);

int interpretar_linea_pokemon(char* linea, pokemon_t** pokemon);
pokemon_t* nuevo_pokemon();
void destructor_pokemones(void* elem);

int getlinea(char** string, size_t* max, FILE* stream);

#endif /*__JUEGO_H__*/