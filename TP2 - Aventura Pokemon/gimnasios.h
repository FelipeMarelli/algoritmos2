#ifndef __GIMNASIOS_H__
#define __GIMNASIOS_H__

#include "batallas.h"
#include "juego.h"
#include <string.h>

int cargar_gimnasios(heap_t* heap, FILE* archivo);
int procesar_linea_gym(gimnasio_t* gym, char* linea, char*);
int procesar_gimnasio(gimnasio_t* gym, char* linea);
int procesar_entrenador(gimnasio_t* gym, char* linea);
int gym_procesar_pokemon(gimnasio_t* gym, char* linea);

gimnasio_t* nuevo_gym();
entrenador_t* nuevo_entrenador();

int comparador_gimnasios(void*, void*);
void destructor_gimnasios(void*);
void destructor_entrenadores(void* elem);
//void destructor pokemones(void*) en juego.h

#endif /*__GIMNASIOS_H__*/