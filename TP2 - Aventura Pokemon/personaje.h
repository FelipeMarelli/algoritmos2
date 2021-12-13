#ifndef __PERSONAJE_H__
#define __PERSONAJE_H__

#include "juego.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cargar_personaje(FILE* archivo, personaje_t** personaje);
int procesar_linea_pers(personaje_t*, char*, char*);
int procesar_personaje(personaje_t* personaje, char* linea);
int pers_procesar_pokemon(personaje_t* personaje, char* linea);
int abb_comparador_pokemones(void* poke1, void* poke2);

#endif /*__PERSONAJE_H__*/
