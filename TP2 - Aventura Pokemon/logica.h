#ifndef __LOGICA_H__
#define __LOGICA_H__

#include "batallas.h"
#include "juego.h"
#include "menu.h"

int partida_manual(heap_t* heap, personaje_t* personaje);
int batalla_entrenador(gimnasio_t* gym, personaje_t* personaje, entrenador_t* entre);
int partida_simulada(heap_t* heap, personaje_t* personaje);
int combate(personaje_t* personaje, entrenador_t* entrenador, int (*funcion_batalla)(void*, void*), bool manual);

#endif //__LOGICA_H__