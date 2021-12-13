#include "tda_heap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

heap_t* heap_crear(comparador_t comparador, destructor_t destructor)
{
    if (!comparador)
        return NULL;
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap)
        return NULL;
    heap->comparador = comparador;
    heap->destructor = destructor;
    heap->array = NULL;
    heap->size = 0;
    return heap;
}

int heap_insertar(heap_t* heap, void* elemento)
{
    if (!heap)
        return ERROR;
    void* nuevo_array = realloc(heap->array, sizeof(void*) * (heap->size + 1));
    if (!nuevo_array)
        return ERROR;
    heap->array = nuevo_array;
    heap->array[heap->size] = elemento;
    heap->size++;
    sift_up(heap, heap->size - 1);
    return EXITO;
}

void sift_up(heap_t* heap, size_t elem)
{
    if (elem == 0)
        return;
    size_t padre = (elem - 1) / 2;
    if (heap->comparador(heap->array[elem], heap->array[padre]) == -1) {
        swap(heap->array, elem, padre);
        sift_up(heap, padre);
    }
}

void* heap_eliminar(heap_t* heap)
{
    if (!heap || heap->size == 0) {
        return NULL;
    }
    void* raiz = heap->array[0];
    if (heap->size == 1) {
        free(heap->array);
        heap->array = NULL;
        heap->size = 0;
        return raiz;
    }
    swap(heap->array, 0, heap->size - 1);
    void* nuevo_array = realloc(heap->array, sizeof(void*) * (heap->size - 1));
    if (!nuevo_array) {
        return NULL;
    }
    heap->array = nuevo_array;
    heap->size--;
    sift_down(heap, 0);
    return raiz;
}

void sift_down(heap_t* heap, size_t elem)
{
    size_t pos_izq = 2 * elem + 1;
    size_t pos_menor = pos_izq;
    size_t pos_der = 2 * elem + 2;

    if (pos_izq >= heap->size)
        return;
    if (pos_der < heap->size)
        if (heap->comparador(heap->array[pos_der], heap->array[pos_izq]) == -1)
            pos_menor = pos_der;
    if (heap->comparador(heap->array[elem], heap->array[pos_menor]) == 1) {
        swap(heap->array, elem, pos_menor);
        sift_down(heap, pos_menor);
    }
}

void* heap_raiz(heap_t* heap)
{
    if (!heap || heap->size == 0)
        return NULL;
    return heap->array[0];
}

size_t heap_elementos(heap_t* heap)
{
    if (!heap)
        return 0;
    return heap->size;
}

bool heap_vacio(heap_t* heap)
{
    if (!heap || heap->size == 0)
        return true;
    return false;
}

void heap_destruir(heap_t* heap)
{
    if (!heap)
        return;
    if (heap->size > 0) {
        if (heap->destructor)
            for (int i = 0; i < heap->size; ++i)
                heap->destructor(heap->array[i]);
        free(heap->array);
    }
    free(heap);
}

void swap(void** arr, size_t i, size_t j)
{
    if (!arr)
        return;
    void* aux = arr[i];
    arr[i] = arr[j];
    arr[j] = aux;
}
