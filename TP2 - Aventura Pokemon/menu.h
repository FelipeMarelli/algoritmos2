#ifndef __MENU_H__
#define __MENU_H__

#include "gimnasios.h"
#include "juego.h"
#include "personaje.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int listar_pokemones_combate(lista_t* lista, bool id);
int listar_pokemones_obtenidos(abb_t* arbol);
bool abb_mostrar_info_poke(void* pokemon, void* extra);
int nuevos_pokes_combate(personaje_t*, int[]);
int menu_cambio_pokemones(personaje_t*);
int informacion_gimnasio(gimnasio_t* gym);
int informacion_personaje(personaje_t* personaje);

void menu_combate(personaje_t*, entrenador_t*, pokemon_t*, pokemon_t*, int);
int menu_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio);
char menu_ppal(bool personaje, int gimnasios);
int menu_victoria(personaje_t* personaje, entrenador_t* entre, gimnasio_t* gym);
int menu_tomar_pokemon(personaje_t* personaje, entrenador_t* entre);
int menu_derrota(personaje_t* personaje);
int menu_maestro_pokemon(int, personaje_t* personaje);
bool menu_error_archivo();
char leer_caracter();
char* leer_linea();
int leer_numero();
int ingresar_personaje(personaje_t** personaje);
int ingresar_gimnasio(heap_t* heap);

#endif //__MENU_H__