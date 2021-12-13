#include "abb.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

nodo_abb_t* arbol_insertar_recursivo(nodo_abb_t*, abb_comparador, nodo_abb_t*);
void destruir_nodo_recursivo(nodo_abb_t*, abb_liberar_elemento);
void* buscar_nodo_recursivo(nodo_abb_t*, void*, abb_comparador);
nodo_abb_t* arbol_borrar_recursivo(nodo_abb_t*, void*, abb_comparador, abb_liberar_elemento, int*);
nodo_abb_t* sacar_nodo_maximo_recursivo(nodo_abb_t*, void**);

size_t recorrido_inorden_recursivo(nodo_abb_t*, void**, size_t, size_t);
size_t recorrido_preorden_recursivo(nodo_abb_t*, void**, size_t, size_t);
size_t recorrido_postorden_recursivo(nodo_abb_t*, void**, size_t, size_t);

size_t iterador_interno_inorden(nodo_abb_t*, bool (*)(void*, void*), void*, size_t, bool*);
size_t iterador_interno_preorden(nodo_abb_t*, bool (*)(void*, void*), void*, size_t, bool*);
size_t iterador_interno_postorden(nodo_abb_t*, bool (*)(void*, void*), void*, size_t, bool*);

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor)
{
    if (!comparador)
        return NULL;
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol)
        return NULL;
    arbol->comparador = comparador;
    arbol->destructor = destructor;
    arbol->nodo_raiz = NULL;
    return arbol;
}

int arbol_insertar(abb_t* arbol, void* elemento)
{
    if (!arbol)
        return -1;
    nodo_abb_t* nodo_a_insertar = malloc(sizeof(nodo_abb_t));
    if (!nodo_a_insertar)
        return -1;
    nodo_a_insertar->elemento = elemento;
    nodo_a_insertar->izquierda = NULL;
    nodo_a_insertar->derecha = NULL;
    arbol->nodo_raiz = arbol_insertar_recursivo(arbol->nodo_raiz, arbol->comparador, nodo_a_insertar);
    return 0;
}

nodo_abb_t* arbol_insertar_recursivo(nodo_abb_t* nodo_actual, abb_comparador comparador, nodo_abb_t* nodo_a_insertar)
{
    if (!nodo_actual)
        return nodo_a_insertar;
    if (comparador(nodo_a_insertar->elemento, nodo_actual->elemento) <= 0)
        nodo_actual->izquierda = arbol_insertar_recursivo(nodo_actual->izquierda, comparador, nodo_a_insertar);
    else
        nodo_actual->derecha = arbol_insertar_recursivo(nodo_actual->derecha, comparador, nodo_a_insertar);
    return nodo_actual;
}

int arbol_borrar(abb_t* arbol, void* elemento)
{
    if (!arbol || !arbol->nodo_raiz)
        return -1;
    int encontrado = 0;
    arbol->nodo_raiz = arbol_borrar_recursivo(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor, &encontrado);
    return encontrado;
}

nodo_abb_t* arbol_borrar_recursivo(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor, int* encontrado)
{
    if (!nodo_actual) {
        *encontrado = -1;
        return NULL;
    }
    int comparacion = comparador(elemento, nodo_actual->elemento);
    if (comparacion < 0)
        nodo_actual->izquierda = arbol_borrar_recursivo(nodo_actual->izquierda, elemento, comparador, destructor, encontrado);
    else if (comparacion > 0)
        nodo_actual->derecha = arbol_borrar_recursivo(nodo_actual->derecha, elemento, comparador, destructor, encontrado);
    else { //se encontró
        if (destructor)
            destructor(nodo_actual->elemento);
        nodo_abb_t* nodo_a_devolver;
        if (nodo_actual->izquierda && nodo_actual->derecha) { //caso 2 hijos
            void* elemento_maximo;
            nodo_actual->izquierda = sacar_nodo_maximo_recursivo(nodo_actual->izquierda, &elemento_maximo);
            nodo_actual->elemento = elemento_maximo;
            return nodo_actual;
        } else if (!nodo_actual->izquierda && !nodo_actual->derecha) //caso no hijos
            nodo_a_devolver = NULL;
        else //caso 1 hijo
            nodo_a_devolver = nodo_actual->izquierda ? nodo_actual->izquierda : nodo_actual->derecha;
        free(nodo_actual);
        return nodo_a_devolver;
    }
    return nodo_actual;
}

nodo_abb_t* sacar_nodo_maximo_recursivo(nodo_abb_t* nodo_actual, void** elemento_maximo)
{
    if (!nodo_actual->derecha) {
        *elemento_maximo = nodo_actual->elemento;
        nodo_abb_t* nodo_tmp = NULL;
        if (nodo_actual->izquierda)
            nodo_tmp = nodo_actual->izquierda;
        free(nodo_actual);
        return nodo_tmp;
    }
    nodo_actual->derecha = sacar_nodo_maximo_recursivo(nodo_actual->derecha, elemento_maximo);
    return nodo_actual;
}

void* arbol_buscar(abb_t* arbol, void* elemento)
{
    if (!arbol || !arbol->nodo_raiz)
        return NULL;
    return buscar_nodo_recursivo(arbol->nodo_raiz, elemento, arbol->comparador);
}

void* buscar_nodo_recursivo(nodo_abb_t* nodo_actual, void* elemento, abb_comparador comparador)
{
    if (!nodo_actual)
        return NULL;
    int comparacion = comparador(elemento, nodo_actual->elemento);
    if (comparacion == 0)
        return nodo_actual->elemento;
    return buscar_nodo_recursivo(comparacion < 0 ? nodo_actual->izquierda : nodo_actual->derecha, elemento, comparador);
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array)
{
    if (!arbol || !arbol->nodo_raiz || !array || !tamanio_array)
        return 0;
    return recorrido_inorden_recursivo(arbol->nodo_raiz, array, 0, tamanio_array);
}

size_t recorrido_inorden_recursivo(nodo_abb_t* raiz, void** array, size_t index, size_t tamanio_array)
{
    if (index < tamanio_array) {
        if (raiz->izquierda)
            index = recorrido_inorden_recursivo(raiz->izquierda, array, index, tamanio_array);
        if (index < tamanio_array) {
            array[index] = raiz->elemento;
            index++;
        }
        if (raiz->derecha)
            index = recorrido_inorden_recursivo(raiz->derecha, array, index, tamanio_array);
    }
    return index;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array)
{
    if (!arbol || !arbol->nodo_raiz || !array || !tamanio_array)
        return 0;
    return recorrido_preorden_recursivo(arbol->nodo_raiz, array, 0, tamanio_array);
}

size_t recorrido_preorden_recursivo(nodo_abb_t* raiz, void** array, size_t index, size_t tamanio_array)
{
    if (index < tamanio_array) {
        array[index] = raiz->elemento;
        index++;
        if (raiz->izquierda)
            index = recorrido_preorden_recursivo(raiz->izquierda, array, index, tamanio_array);
        if (raiz->derecha)
            index = recorrido_preorden_recursivo(raiz->derecha, array, index, tamanio_array);
    }
    return index;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array)
{
    if (!arbol || !arbol->nodo_raiz || !array || !tamanio_array)
        return 0;
    return recorrido_postorden_recursivo(arbol->nodo_raiz, array, 0, tamanio_array);
}

size_t recorrido_postorden_recursivo(nodo_abb_t* raiz, void** array, size_t index, size_t tamanio_array)
{
    if (index < tamanio_array) {
        if (raiz->izquierda)
            index = recorrido_postorden_recursivo(raiz->izquierda, array, index, tamanio_array);
        if (raiz->derecha)
            index = recorrido_postorden_recursivo(raiz->derecha, array, index, tamanio_array);
        if (index < tamanio_array) {
            array[index] = raiz->elemento;
            index++;
        }
    }
    return index;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra)
{
    if (!arbol || !arbol->nodo_raiz || !funcion)
        return 0;
    bool frenar = false;
    size_t visitados = 0;
    switch (recorrido) {
    case ABB_RECORRER_INORDEN:
        visitados = iterador_interno_inorden(arbol->nodo_raiz, funcion, extra, 0, &frenar);
        break;
    case ABB_RECORRER_PREORDEN:
        visitados = iterador_interno_preorden(arbol->nodo_raiz, funcion, extra, 0, &frenar);
        break;
    case ABB_RECORRER_POSTORDEN:
        visitados = iterador_interno_postorden(arbol->nodo_raiz, funcion, extra, 0, &frenar);
        break;
    }
    return visitados;
}

size_t iterador_interno_inorden(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* extra, size_t visitados, bool* frenar)
{
    if (!(*frenar)) {
        if (raiz->izquierda)
            visitados = iterador_interno_inorden(raiz->izquierda, funcion, extra, visitados, frenar);
        if (!(*frenar)) {
            *frenar = funcion(raiz->elemento, extra);
            visitados++;
        }
        if (raiz->derecha)
            visitados = iterador_interno_inorden(raiz->derecha, funcion, extra, visitados, frenar);
    }
    return visitados;
}

size_t iterador_interno_preorden(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* extra, size_t visitados, bool* frenar)
{
    if (!(*frenar)) {
        *frenar = funcion(raiz->elemento, extra);
        visitados++;
        if (raiz->izquierda)
            visitados = iterador_interno_preorden(raiz->izquierda, funcion, extra, visitados, frenar);
        if (raiz->derecha)
            visitados = iterador_interno_preorden(raiz->derecha, funcion, extra, visitados, frenar);
    }
    return visitados;
}

size_t iterador_interno_postorden(nodo_abb_t* raiz, bool (*funcion)(void*, void*), void* extra, size_t visitados, bool* frenar)
{
    if (!(*frenar)) {
        if (raiz->izquierda)
            visitados = iterador_interno_postorden(raiz->izquierda, funcion, extra, visitados, frenar);
        if (raiz->derecha)
            visitados = iterador_interno_postorden(raiz->derecha, funcion, extra, visitados, frenar);

        if (!(*frenar)) {
            *frenar = funcion(raiz->elemento, extra);
            visitados++;
        }
    }
    return visitados;
}

void* arbol_raiz(abb_t* arbol)
{
    if (!arbol || !arbol->nodo_raiz)
        return NULL;
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol)
{
    if (!arbol || !arbol->nodo_raiz)
        return true;
    return false;
}

void arbol_destruir(abb_t* arbol)
{
    if (!arbol)
        return;
    if (arbol->nodo_raiz)
        destruir_nodo_recursivo(arbol->nodo_raiz, arbol->destructor);
    free(arbol);
}

void destruir_nodo_recursivo(nodo_abb_t* nodo, abb_liberar_elemento destructor)
{
    if (nodo->izquierda)
        destruir_nodo_recursivo(nodo->izquierda, destructor);
    if (nodo->derecha)
        destruir_nodo_recursivo(nodo->derecha, destructor);
    if (destructor)
        destructor(nodo->elemento);
    free(nodo);
}