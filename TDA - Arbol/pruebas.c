#include "abb.h"
#include "pa2mm.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int valor;
    int destruido;
} manzana;

int comparador_numeros(void*, void*);
void destructor_numeros(void*);
int comparar_manzanas(void*, void*);
void destructor_manzanas(void*);
void probar_arbol_crear();
void probar_arbol_insertar_eliminar();
void pruebas_con_null();
void pruebas_destruccion();
void pruebas_busqueda();
void pruebas_recorrido();
void pruebas_iterador_interno();
bool sumar_numeros(void* elem, void* suma);
bool sumar_numeros_hasta_8(void*, void*);
bool chequear_inorden(void* elem, void* index);
bool chequear_preorden(void* elem, void* index);
bool chequear_postorden(void* elem, void* index);

int main(int argc, char const* argv[])
{
    pa2m_nuevo_grupo("Pruebas de creación");
    probar_arbol_crear();
    pa2m_nuevo_grupo("Pruebas con NULL");
    pruebas_con_null();
    pa2m_nuevo_grupo("Pruebas de inserción y eliminación");
    probar_arbol_insertar_eliminar();
    pa2m_nuevo_grupo("Pruebas de destrucción");
    pruebas_destruccion();
    pa2m_nuevo_grupo("Pruebas de búsqueda");
    pruebas_busqueda();
    pa2m_nuevo_grupo("Pruebas de recorrido");
    pruebas_recorrido();
    pa2m_nuevo_grupo("Pruebas de iterador interno");
    pruebas_iterador_interno();
    pa2m_mostrar_reporte();
    return 0;
}

void probar_arbol_crear()
{
    abb_t* arbol1;
    abb_t* arbol2;
    pa2m_afirmar(arbol_crear(NULL, NULL) == NULL, "No puedo crear un árbol sin comparador");
    pa2m_afirmar((arbol1 = arbol_crear(comparador_numeros, NULL)) != NULL, "Creo un árbol sin destructor");
    pa2m_afirmar(arbol_vacio(arbol1), "El árbol está vacío");
    pa2m_afirmar(arbol_raiz(arbol1) == NULL, "Árbol raíz es NULL");
    pa2m_afirmar(arbol1->comparador == comparador_numeros && arbol1->destructor == NULL, "Se inicializan los punteros");
    pa2m_afirmar((arbol2 = arbol_crear(comparador_numeros, destructor_numeros)) != NULL, "Creo un árbol con destructor");
    pa2m_afirmar(arbol_vacio(arbol2), "El árbol está vacío");
    pa2m_afirmar(arbol_raiz(arbol2) == NULL, "Árbol raíz es NULL");
    pa2m_afirmar(arbol2->comparador == comparador_numeros && arbol2->destructor == destructor_numeros, "Se inicializan los punteros");
    arbol_destruir(arbol1);
    arbol_destruir(arbol2);
}

void probar_arbol_insertar_eliminar()
{
    abb_t* arbol = arbol_crear(comparador_numeros, destructor_numeros);
    int nums[7] = { 0, 2, 4, 6, 8, 10, 7 };
    pa2m_mensaje("Inserto elementos en el árbol");
    pa2m_afirmar(arbol_insertar(arbol, &nums[3]) == 0, "Inserto en un árbol vacío");
    pa2m_afirmar(arbol_vacio(arbol) == false, "El árbol ya no está vacío");
    pa2m_afirmar((int*)arbol_raiz(arbol) == &nums[3], "El elemento en la raíz es correcto");
    pa2m_afirmar(arbol_insertar(arbol, &nums[1]) == 0, "Inserto un elemento menor");
    pa2m_afirmar((int*)arbol->nodo_raiz->izquierda->elemento == &nums[1], "El elemento es correcto");
    pa2m_afirmar(arbol_insertar(arbol, &nums[5]) == 0, "Inserto un elemento mayor");
    pa2m_afirmar((int*)arbol->nodo_raiz->derecha->elemento == &nums[5], "El elemento es correcto");
    pa2m_afirmar(arbol_insertar(arbol, &nums[4]) == 0, "Inserto un elemento");
    pa2m_afirmar(arbol_insertar(arbol, &nums[2]) == 0, "Inserto un elemento");
    pa2m_afirmar(arbol_insertar(arbol, &nums[0]) == 0, "Inserto un elemento");
    pa2m_afirmar(arbol_insertar(arbol, &nums[3]) == 0, "Inserto un elemento repetido");
    pa2m_afirmar(arbol_insertar(arbol, &nums[2]) == 0, "Inserto un elemento repetido");
    pa2m_mensaje("Borro elementos del árbol");
    pa2m_afirmar(arbol_borrar(arbol, &nums[6]) == -1, "Intento eliminar un elemento que no está en el árbol");
    pa2m_afirmar(arbol_borrar(arbol, &nums[0]) == 0, "Elimino el elemento de un nodo hoja");
    pa2m_afirmar(arbol_borrar(arbol, &nums[1]) == 0, "Elimino el elemento de un nodo con 1 hijo");
    pa2m_afirmar(arbol_borrar(arbol, &nums[2]) == 0, "Elimino el elemento de un nodo con 2 hijos");
    pa2m_afirmar(arbol_borrar(arbol, &nums[5]) == 0, "Elimino el elemento de un nodo con 1 hijo");
    pa2m_afirmar(arbol_borrar(arbol, &nums[2]) == 0, "Elimino el elemento de un nodo con 1 hijo");
    pa2m_afirmar(arbol_borrar(arbol, &nums[3]) == 0, "Elimino el elemento del nodo raíz");
    pa2m_afirmar(arbol_borrar(arbol, &nums[3]) == 0, "Elimino el elemento del nodo raíz");
    pa2m_afirmar(arbol_borrar(arbol, &nums[4]) == 0, "Elimino el elemento del nodo raíz");
    pa2m_afirmar(arbol_vacio(arbol) == true, "El árbol queda vacío");
    pa2m_afirmar(arbol_borrar(arbol, &nums[6]) == -1, "Intento eliminar del árbol vacío");
    arbol_destruir(arbol);
}

void pruebas_destruccion()
{
    abb_t* arbol = arbol_crear(comparar_manzanas, destructor_manzanas);
    manzana manzanas[8];
    int orden[] = { 3, 1, 5, 0, 2, 4, 7, 6 };
    for (int i = 0; i < 8; ++i) {
        manzana manz = { orden[i] * 2, 0 };
        manzanas[i] = manz;
        arbol_insertar(arbol, &manzanas[i]);
    }
    int manzanas_destruidas = 0;
    bool destruidas_una_vez = true;
    arbol_destruir(arbol);
    for (int i = 0; i < 8; ++i) {
        if (manzanas[i].destruido == 1)
            manzanas_destruidas++;
        else
            destruidas_una_vez = false;
    }
    pa2m_afirmar(manzanas_destruidas == 8, "Todos los elementos del árbol fueron destruidos");
    pa2m_afirmar(destruidas_una_vez == true, "Destructor se llama solo una vez con cada elemento");
}

void pruebas_busqueda()
{
    abb_t* arbol = arbol_crear(comparador_numeros, destructor_numeros);
    int numeros[] = { 0, 2, 4, 6, 8, 10, 12, 4 };
    pa2m_afirmar(arbol_buscar(arbol, &numeros[5]) == NULL, "Busco en un árbol vacío");
    int clave_a_buscar;
    arbol_insertar(arbol, &numeros[2]);
    arbol_insertar(arbol, &numeros[0]);
    arbol_insertar(arbol, &numeros[4]);
    arbol_insertar(arbol, &numeros[1]);
    arbol_insertar(arbol, &numeros[3]);
    arbol_insertar(arbol, &numeros[5]);
    arbol_insertar(arbol, &numeros[7]);
    arbol_insertar(arbol, &numeros[6]);
    clave_a_buscar = 413;
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == NULL, "Busco un elemento que no está en el árbol");
    clave_a_buscar = 11;
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == NULL, "Busco un elemento que no está en el árbol");
    clave_a_buscar = 12;
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == &numeros[6], "Busco un elemento que está en un nodo hoja");
    clave_a_buscar = 4;
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == &numeros[2], "Busco el elemento en el nodo raíz, cuya clave está repetida");
    pa2m_afirmar(arbol_borrar(arbol, &clave_a_buscar) == 0, "Elimino el elemento en el nodo raíz");
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == &numeros[7], "Busco el elemento que queda, con la misma clave");
    clave_a_buscar = 8;
    pa2m_afirmar(arbol_buscar(arbol, &clave_a_buscar) == &numeros[4], "Busco un elemento en un nodo con 2 hijos");
    arbol_destruir(arbol);
}

void pruebas_recorrido()
{
    abb_t* arbol = arbol_crear(comparador_numeros, destructor_numeros);
    int elementos[] = { 6, 2, 0, 4, 10, 8 };
    int elem_inorden[] = { 0, 2, 4, 6, 8, 10 };
    int elem_preorden[] = { 6, 2, 0, 4, 10, 8 };
    int elem_postorden[] = { 0, 4, 2, 8, 10, 6 };
    int clave_a_buscar = 0;
    clave_a_buscar = clave_a_buscar;
    int* resultado[6];
    bool correcto = true;
    correcto = correcto;
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)resultado, 6) == 0, "Recorrido inorden con árbol vacío");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)resultado, 6) == 0, "Recorrido preorden con árbol vacío");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)resultado, 6) == 0, "Recorrido postorden con árbol vacío");
    for (int i = 0; i < 6; ++i)
        arbol_insertar(arbol, &elementos[i]);
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)resultado, 0) == 0, "Recorrido inorden con tamaño 0 devuelve 0");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)resultado, 0) == 0, "Recorrido preorden con tamaño 0 devuelve 0");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)resultado, 0) == 0, "Recorrido postorden con tamaño 0 devuelve 0");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)resultado, 10) == 6, "Recorrido inorden visita todos los nodos");
    for (int i = 0; i < 6; ++i)
        if (*resultado[i] != elem_inorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)resultado, 10) == 6, "Recorrido preorden visita todos los nodos");
    for (int i = 0; i < 6; ++i)
        if (*resultado[i] != elem_preorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)resultado, 10) == 6, "Recorrido postorden visita todos los nodos");
    for (int i = 0; i < 6; ++i)
        if (*resultado[i] != elem_postorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");

    pa2m_afirmar(arbol_recorrido_inorden(arbol, (void**)resultado, 3) == 3, "Recorrido inorden con tamaño 3 devuelve 3");
    for (int i = 0; i < 3; ++i)
        if (*resultado[i] != elem_inorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, (void**)resultado, 3) == 3, "Recorrido preorden con tamaño 3 devuelve 3");
    for (int i = 0; i < 3; ++i)
        if (*resultado[i] != elem_preorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, (void**)resultado, 3) == 3, "Recorrido postorden con tamaño 3 devuelve 3");
    for (int i = 0; i < 3; ++i)
        if (*resultado[i] != elem_postorden[i])
            correcto = false;
    pa2m_afirmar(correcto == true, "El orden es correcto");

    arbol_destruir(arbol);
}

void pruebas_con_null()
{
    abb_t* arbol = arbol_crear(comparador_numeros, NULL);
    pa2m_afirmar(arbol_insertar(NULL, NULL) == -1, "Insertar en árbol NULL no falla");
    pa2m_afirmar(arbol_borrar(NULL, NULL) == -1, "Eliminar de árbol NULL no falla");
    pa2m_afirmar(arbol_vacio(NULL) == true, "Árbol vacío con NULL devuelve verdadero");
    pa2m_afirmar(arbol_raiz(NULL) == NULL, "Raíz de árbol NULL es NULL");
    pa2m_afirmar(arbol_buscar(NULL, NULL) == NULL, "Buscar en un árbol NULL no falla");
    pa2m_afirmar(arbol_recorrido_inorden(NULL, NULL, 10) == 0, "Árbol recorrido inorden con árbol NULL no falla");
    pa2m_afirmar(arbol_recorrido_inorden(arbol, NULL, 10) == 0, "Árbol recorrido inorden con array NULL no falla");
    pa2m_afirmar(arbol_recorrido_preorden(NULL, NULL, 10) == 0, "Árbol recorrido preorden con árbol NULL no falla");
    pa2m_afirmar(arbol_recorrido_preorden(arbol, NULL, 10) == 0, "Árbol recorrido preorden con array NULL no falla");
    pa2m_afirmar(arbol_recorrido_postorden(NULL, NULL, 10) == 0, "Árbol recorrido postorden con árbol NULL no falla");
    pa2m_afirmar(arbol_recorrido_postorden(arbol, NULL, 10) == 0, "Árbol recorrido postorden con array NULL no falla");
    pa2m_afirmar(abb_con_cada_elemento(NULL, 0, sumar_numeros, NULL) == 0, "Iterador interno con árbol NULL no falla");
    pa2m_afirmar(abb_con_cada_elemento(arbol, 0, NULL, NULL) == 0, "Iterador interno con función NULL no falla");
    arbol_destruir(NULL);
    arbol_destruir(arbol);
}

void pruebas_iterador_interno()
{
    abb_t* arbol = arbol_crear(comparador_numeros, destructor_numeros);
    int numeros[] = { 6, 2, 1, 4, 10, 8, 12 };
    int suma = 0;
    int index = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, sumar_numeros, &suma) == 0, "Iterador interno inorden con árbol vacío devuelve 0");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, sumar_numeros, &suma) == 0, "Iterador interno preorden con árbol vacío devuelve 0");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, sumar_numeros, &suma) == 0, "Iterador interno postorden con árbol vacío devuelve 0");
    for (int i = 0; i < 7; ++i)
        arbol_insertar(arbol, &numeros[i]);
    pa2m_afirmar(abb_con_cada_elemento(arbol, -100, sumar_numeros, &suma) == 0, "Iterador interno con recorrido incorrecto");
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, sumar_numeros, &suma) == 7 && suma == 43, "Sumo todos los elementos con iterador interno indorden");
    suma = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, sumar_numeros, &suma) == 7 && suma == 43, "Sumo todos los elementos con iterador interno predorden");
    suma = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, sumar_numeros, &suma) == 7 && suma == 43, "Sumo todos los elementos con iterador interno postorden");
    suma = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, sumar_numeros_hasta_8, &suma) == 5 && suma == 21, "Sumo todos los elementos hasta el 8 con iterador interno indorden");
    suma = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, sumar_numeros_hasta_8, &suma) == 6 && suma == 31, "Sumo todos los elementos hasta el 8 con iterador interno predorden");
    suma = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, sumar_numeros_hasta_8, &suma) == 4 && suma == 15, "Sumo todos los elementos hasta el 8 con iterador interno postorden");

    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, chequear_inorden, &index) == 7 && index == 7, "Los elementos se recorren en orden con iterador interno inorden");
    index = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, chequear_preorden, &index) == 7 && index == 7, "Los elementos se recorren en orden con iterador interno preorden");
    index = 0;
    pa2m_afirmar(abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, chequear_postorden, &index) == 7 && index == 7, "Los elementos se recorren en orden con iterador interno postorden");

    arbol_destruir(arbol);
}

bool chequear_inorden(void* elem, void* index)
{
    int numeros_inorden[] = { 1, 2, 4, 6, 8, 10, 12 };
    if (numeros_inorden[*(int*)index] != *(int*)elem)
        return true;
    *(int*)index += 1;
    return false;
}

bool chequear_preorden(void* elem, void* index)
{
    int numeros_preorden[] = { 6, 2, 1, 4, 10, 8, 12 };
    if (numeros_preorden[*(int*)index] != *(int*)elem)
        return true;
    *(int*)index += 1;
    return false;
}

bool chequear_postorden(void* elem, void* index)
{
    int numeros_postorden[] = { 1, 4, 2, 8, 12, 10, 6 };
    if (numeros_postorden[*(int*)index] != *(int*)elem)
        return true;
    *(int*)index += 1;
    return false;
}

bool sumar_numeros(void* elem, void* suma)
{
    *(int*)suma += *(int*)elem;
    return false;
}

bool sumar_numeros_hasta_8(void* elem, void* suma)
{
    *(int*)suma += *(int*)elem;
    if (*(int*)elem == 8)
        return true;
    return false;
}

int comparar_manzanas(void* manzana1, void* manzana2)
{
    int resultado = 0;
    if (((manzana*)manzana1)->valor > ((manzana*)manzana2)->valor)
        resultado = 1;
    else if (((manzana*)manzana1)->valor < ((manzana*)manzana2)->valor)
        resultado = -1;
    return resultado;
}

void destructor_manzanas(void* manz)
{
    ((manzana*)manz)->destruido++;
    return;
}

int comparador_numeros(void* elemento1, void* elemento2)
{
    int resultado = 0;
    if (*((int*)elemento1) > *((int*)elemento2))
        resultado = 1;
    else if (*((int*)elemento1) < *((int*)elemento2))
        resultado = -1;
    return resultado;
}

void destructor_numeros(void* elemento1)
{
    return;
}