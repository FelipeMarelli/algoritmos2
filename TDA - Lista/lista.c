#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

lista_t* lista_crear()
{
    lista_t* lista_p = malloc(sizeof(lista_t));
    if (lista_p == NULL)
        return NULL;
    lista_p->cantidad = 0;
    lista_p->nodo_inicio = NULL;
    lista_p->nodo_fin = NULL;
    return lista_p;
}

int lista_insertar(lista_t* lista, void* elemento)
{
    if (lista == NULL)
        return -1;
    nodo_t* nuevo_nodo_final = malloc(sizeof(nodo_t));
    if (nuevo_nodo_final == NULL)
        return -1;
    nuevo_nodo_final->elemento = elemento;
    nuevo_nodo_final->siguiente = NULL;

    if (lista->cantidad == 0) {
        lista->nodo_inicio = nuevo_nodo_final;
        lista->nodo_fin = nuevo_nodo_final;
    } else {
        lista->nodo_fin->siguiente = nuevo_nodo_final;
        lista->nodo_fin = nuevo_nodo_final;
    }
    lista->cantidad++;
    return 0;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion)
{
    if (lista == NULL)
        return -1;
    if (posicion > lista->cantidad - 1 || lista->cantidad == 0)
        return lista_insertar(lista, elemento);

    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
    if (nuevo_nodo == NULL)
        return -1;
    nuevo_nodo->elemento = elemento;

    if (posicion == 0) {
        nuevo_nodo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo_nodo;
    } else {
        nodo_t* nodo_anterior = lista->nodo_inicio;
        while (posicion > 1) {
            nodo_anterior = nodo_anterior->siguiente;
            posicion--;
        }
        nuevo_nodo->siguiente = nodo_anterior->siguiente;
        nodo_anterior->siguiente = nuevo_nodo;
    }
    lista->cantidad++;
    return 0;
}

int lista_borrar(lista_t* lista)
{
    if (lista == NULL || lista->cantidad == 0)
        return -1;
    if (lista->cantidad == 1) {
        free(lista->nodo_inicio);
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
    } else {
        nodo_t* nodo_anteultimo = lista->nodo_inicio;
        while (nodo_anteultimo->siguiente != lista->nodo_fin) {
            nodo_anteultimo = nodo_anteultimo->siguiente;
        }
        free(lista->nodo_fin);
        nodo_anteultimo->siguiente = NULL;
        lista->nodo_fin = nodo_anteultimo;
    }
    lista->cantidad--;
    return 0;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion)
{
    if (lista == NULL || lista->cantidad == 0)
        return -1;
    if (lista->cantidad == 1 || lista->cantidad - 1 <= posicion)
        return lista_borrar(lista);
    if (posicion == 0) {
        nodo_t* nodo_inicio_auxiliar = lista->nodo_inicio;
        lista->nodo_inicio = nodo_inicio_auxiliar->siguiente;
        free(nodo_inicio_auxiliar);
    } else {
        nodo_t* nodo_anterior = lista->nodo_inicio;
        nodo_t* nodo_a_borrar = nodo_anterior->siguiente;
        while (posicion > 1) {
            nodo_anterior = nodo_a_borrar;
            nodo_a_borrar = nodo_a_borrar->siguiente;
            posicion--;
        }
        nodo_anterior->siguiente = nodo_a_borrar->siguiente;
        free(nodo_a_borrar);
    }
    lista->cantidad--;
    return 0;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion)
{
    if (lista == NULL || lista->cantidad == 0 || posicion >= lista->cantidad)
        return NULL;
    nodo_t* nodo_actual = lista->nodo_inicio;
    while (posicion > 0) {
        nodo_actual = nodo_actual->siguiente;
        posicion--;
    }
    return nodo_actual->elemento;
}

void* lista_ultimo(lista_t* lista)
{
    if (lista == NULL || lista->cantidad == 0)
        return NULL;
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista)
{
    if (lista == NULL)
        return true;
    return (lista->cantidad == 0);
}

size_t lista_elementos(lista_t* lista)
{
    if (lista == NULL)
        return (size_t)0;
    return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento)
{
    return lista_insertar_en_posicion(lista, elemento, 0);
}

int lista_desapilar(lista_t* lista)
{
    return lista_borrar_de_posicion(lista, 0);
}

void* lista_tope(lista_t* lista)
{
    return lista_elemento_en_posicion(lista, 0);
}

int lista_encolar(lista_t* lista, void* elemento)
{
    return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista)
{
    return lista_borrar_de_posicion(lista, 0);
}

void* lista_primero(lista_t* lista)
{
    return lista_elemento_en_posicion(lista, 0);
}

void lista_destruir(lista_t* lista)
{
    if (lista == NULL)
        return;
    nodo_t* nodo_actual = lista->nodo_inicio;
    nodo_t* nodo_siguiente;
    while (nodo_actual != NULL) {
        nodo_siguiente = nodo_actual->siguiente;
        free(nodo_actual);
        nodo_actual = nodo_siguiente;
    }
    free(lista);
    return;
}

lista_iterador_t* lista_iterador_crear(lista_t* lista)
{
    if (lista == NULL)
        return NULL;
    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador)
{
    if (iterador == NULL || iterador->corriente == NULL)
        return false;
    return true;
}

bool lista_iterador_avanzar(lista_iterador_t* iterador)
{
    if (iterador == NULL || iterador->corriente == NULL)
        return false;
    if ((iterador->corriente = iterador->corriente->siguiente) == NULL)
        return false;
    return true;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador)
{
    if (iterador == NULL || iterador->corriente == NULL)
        return NULL;
    return iterador->corriente->elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador)
{
    free(iterador);
    return;
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void* contexto)
{
    if (lista == NULL || funcion == NULL)
        return 0;
    size_t elementos_iterados = 0;
    nodo_t* nodo_actual = lista->nodo_inicio;
    bool continuar = true;
    while (nodo_actual != NULL && continuar) {
        continuar = funcion(nodo_actual->elemento, contexto);
        nodo_actual = nodo_actual->siguiente;
        elementos_iterados++;
    }
    return elementos_iterados;
}