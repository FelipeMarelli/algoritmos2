#ifndef __TDA_HEAP_H__
#define __TDA_HEAP_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR -1
#define EXITO 1

typedef int (*comparador_t)(void*, void*);
typedef void (*destructor_t)(void*);

typedef struct {
    void** array;
    size_t size;
    comparador_t comparador;
    destructor_t destructor;
} heap_t;

void sift_up(heap_t* heap, size_t elem);
void sift_down(heap_t* heap, size_t elem);
void swap(void** arr, size_t i, size_t j);

/*Reserva la memoria para el heap minimal, recibe necesariamente un comparador 
de elementos (si elem1 >= elem2 devuelve 1) y opcionalmente un destructor*/
heap_t* heap_crear(comparador_t, destructor_t);

/*inserta un elemento en el heap, devuelve ERROR o EXITO*/
int heap_insertar(heap_t* heap, void* elemento);

/*elimina raiz y la devuelve, NULL si hay un error*/
void* heap_eliminar(heap_t* heap);

/*devuelve el elemento en la raiz o NULL si no hay*/
void* heap_raiz(heap_t* heap);

/*libera la memoria reservada por el heap*/
void heap_destruir(heap_t* heap);

/*true si esta vacio*/
bool heap_vacio(heap_t* heap);

/*devuelve la cantidad de elementos en el heap*/
size_t heap_elementos(heap_t* heap);

#endif /*__TDA_HEAP_H__*/
