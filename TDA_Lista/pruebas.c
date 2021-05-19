#include "lista.h"
#include "pa2mm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void probar_lista_crear_destruir();
void probar_lista_insertar_al_final();
void probar_lista_operaciones_basicas();
void probar_lista_buscar();
void probar_lista_insertar_en_pos();
void probar_lista_borrar();
void probar_borrar_en_pos();
void pruebas_de_integracion_de_lista();

void probar_operaciones_pila();
void probar_operaciones_cola();
void probar_operaciones_lista();
bool mostrar_elemento(void*, void*);
bool contar(void*, void*);
bool contar_hasta_el_final(void*, void*);

void probar_iterador_interno();
void probar_iterador_externo();
void probar_pila();
void probar_cola();

int main(int argc, char const* argv[])
{
    pa2m_nuevo_grupo("Pruebas de lista");
    probar_lista_crear_destruir();
    probar_lista_insertar_al_final();
    probar_lista_operaciones_basicas();
    probar_lista_buscar();
    probar_lista_insertar_en_pos();
    probar_lista_borrar();
    probar_borrar_en_pos();
    probar_iterador_interno();
    probar_iterador_externo();
    pruebas_de_integracion_de_lista();

    pa2m_nuevo_grupo("Pruebas de pila");
    probar_pila();

    pa2m_nuevo_grupo("Pruebas de cola");
    probar_cola();

    pa2m_mostrar_reporte();
    return 0;
}

void probar_lista_crear_destruir()
{
    pa2m_mensaje("Pruebas de creación y destrucción de lista");
    lista_t* lista;
    pa2m_afirmar((lista = lista_crear()) != NULL, "Lista crear devuelve un puntero no nulo");
    pa2m_afirmar((lista->nodo_inicio == NULL && lista->nodo_fin == NULL), "Se inicializan los punteros en la lista");
    pa2m_afirmar(lista->cantidad == 0, "Lista se crea con 0 elementos");
    lista_destruir(lista);
    lista_destruir(NULL);
    return;
}

void probar_lista_insertar_al_final()
{
    pa2m_mensaje("Lista insertar al final");
    lista_t* lista = lista_crear();
    int i = 0, a = 1;
    pa2m_afirmar(lista_insertar(NULL, &i) == -1, "Lista insertar con lista NULL no falla");
    pa2m_afirmar(lista_insertar(lista, &i) == 0, "Inserto al final con lista vacía");
    pa2m_afirmar(lista->cantidad == 1 && lista->nodo_inicio == lista->nodo_fin, "Lista cantidad es 1 y nodo existe");
    pa2m_afirmar((int*)lista->nodo_inicio->elemento == &i, "Elemento insertado es el correcto");
    pa2m_afirmar(lista_insertar(lista, &a) == 0, "Inserto al final con lista de un elemento");
    pa2m_afirmar((int*)lista->nodo_fin->elemento == &a, "Elemento insertado es el correcto");
    for (i = 0; i < 998; ++i) {
        lista_insertar(lista, &i);
    }
    pa2m_afirmar(lista->cantidad == 1000, "1000 elementos insertados correctamente");
    lista_destruir(lista);
}

void probar_lista_operaciones_basicas()
{
    pa2m_mensaje("Pruebas de operaciones básicas de lista");
    lista_t* lista = lista_crear();
    int a = 0, b = 1, c = 2;
    pa2m_afirmar(lista_vacia(lista) == true, "Lista vacía devuelve true");
    pa2m_afirmar(lista_vacia(NULL) == true, "Lista vacía con NULL devuelve true");
    pa2m_afirmar(lista_elementos(lista) == 0, "Lista elementos con lista vacía devuelve 0");
    pa2m_afirmar(lista_elementos(NULL) == 0, "Lista elementos con lista NULL devuelve 0");

    pa2m_afirmar(lista_ultimo(lista) == NULL, "Lista último con lista vacía devuelve NULL");
    pa2m_afirmar(lista_ultimo(NULL) == 0, "Lista último con lista NULL devuelve NULL");
    lista_insertar(lista, &a);
    lista_insertar(lista, &b);
    lista_insertar(lista, &c);
    pa2m_afirmar(lista_elementos(lista) == 3, "Lista elementos devuelve la cantidad correcta");
    pa2m_afirmar(lista_vacia(lista) == false, "Lista vacía devuelve false con lista llena");
    pa2m_afirmar((int*)lista_ultimo(lista) == &c, "Lista último devuelve el elemento correcto");
    lista_destruir(lista);
}

void probar_lista_buscar()
{
    pa2m_mensaje("Lista elemento en posición");
    lista_t* lista = lista_crear();
    pa2m_afirmar(lista_elemento_en_posicion(lista, 0) == NULL, "Elemento en posición con lista vacía no falla");
    pa2m_afirmar(lista_elemento_en_posicion(NULL, 0) == NULL, "Elemento en posición con lista NULL no falla");
    pa2m_afirmar(lista_elemento_en_posicion(lista, 1000) == NULL, "Elemento en posición 1000 con lista vacía no falla");
    int a = 0, b = 1, c = 2;
    lista_insertar(lista, &a);
    lista_insertar(lista, &b);
    lista_insertar(lista, &c);
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 0) == &a, "Elemento en posición 0 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 1) == &b, "Elemento en posición 1 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 2) == &c, "Elemento en última posición es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 3) == NULL, "Elemento en la posición siguiente no falla");
    pa2m_afirmar(lista_elemento_en_posicion(lista, 1000) == NULL, "Elemento en posición 1000 con lista llena no falla");

    lista_destruir(lista);
}

void probar_lista_insertar_en_pos()
{
    pa2m_mensaje("Insertar en posición");
    lista_t* lista = lista_crear();
    int a = 0, b = 1, c = 2, d = 3, e = 4;
    pa2m_afirmar(lista_insertar_en_posicion(NULL, &a, 100) == -1, "Insertar en posición con lista NULL no falla");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &a, 0) == 0, "Insertar en posición 0 con lista vacía no falla");
    pa2m_afirmar(lista_elementos(lista) == 1, "Lista cantidad es 1");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &e, 1000) == 0, "Insertar en posición 1000 no falla");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &d, 1) == 0, "Insertar en el medio de la lista");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &b, 1) == 0, "Insertar en el medio de la lista");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &c, 2) == 0, "Insertar en el medio de la lista");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 0) == &a, "Elemento en posición 0 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 1) == &b, "Elemento en posición 1 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 2) == &c, "Elemento en posición 2 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 3) == &d, "Elemento en posición 3 es correcto");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 4) == &e, "Elemento en posición 4 es correcto");
    pa2m_afirmar(lista_insertar_en_posicion(lista, &a, 0) == 0, "Insertar en posición 0 con lista llena");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 0) == &a, "Elemento en posición 0 es correcto");
    lista_destruir(lista);
}

void probar_lista_borrar()
{
    pa2m_mensaje("Borrar al final");
    lista_t* lista = lista_crear();
    int a = 0, b = 1, c = 2;
    pa2m_afirmar(lista_borrar(NULL) == -1, "Borrar al final con lista NULL no falla");
    pa2m_afirmar(lista_borrar(lista) == -1, "Borrar al final con lista vacía no falla");
    lista_insertar(lista, &a);
    lista_insertar(lista, &b);
    lista_insertar(lista, &c);
    pa2m_afirmar(lista_borrar(lista) == 0, "Borrar al final con lista de 3 elementos");
    pa2m_afirmar((int*)lista_ultimo(lista) == &b, "Último elemento es correcto");
    pa2m_afirmar(lista_borrar(lista) == 0, "Borrar al final lista de 2 elementos");
    pa2m_afirmar((int*)lista_ultimo(lista) == &a, "Último elemento es correcto");
    pa2m_afirmar(lista_elementos(lista) == 1, "Cantidad de elementos restantes es correcta");
    pa2m_afirmar(lista_borrar(lista) == 0 && lista_vacia(lista), "Borrar al final con lista de un elemento funciona correctamente");
    int i;
    for (i = 0; i < 1000; ++i) {
        lista_insertar(lista, &i);
    }
    while (lista_borrar(lista) == 0)
        ;
    pa2m_afirmar(lista_vacia(lista), "Puedo insertar 1000 elementos y borrar hasta que la lista se vacíe");
    lista_destruir(lista);
}

void probar_borrar_en_pos()
{
    pa2m_mensaje("Lista borrar de posición");
    lista_t* lista = lista_crear();
    int a = 0, b = 1, c = 2, d = 3, e = 4, f = 5;
    pa2m_afirmar(lista_borrar_de_posicion(NULL, 10) == -1, "Borrar de posición con lista NULL no falla");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 10) == -1, "Borrar de posición con lista vacía no falla");
    lista_insertar(lista, &a);
    lista_insertar(lista, &b);
    lista_insertar(lista, &c);
    lista_insertar(lista, &d);
    lista_insertar(lista, &e);
    lista_insertar(lista, &f);
    pa2m_afirmar(lista_borrar_de_posicion(lista, 1000) == 0, "Borrar de posición 1000 no falla");
    pa2m_afirmar((int*)lista_ultimo(lista) == &e, "Último elemento es correcto");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 0) == 0, "Borrar de posición 0 con lista llena");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 0) == &b, "Elemento en posición 0 es correcto");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 1) == 0, "Borrar de posición 1 con lista llena");
    pa2m_afirmar((int*)lista_elemento_en_posicion(lista, 1) == &d, "Elemento en posición 1 es correcto");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 0) == 0, "Borrar de posición 1 con lista de 3 elementos");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 0) == 0, "Borrar de posición 1 con lista de 2 elementos");
    pa2m_afirmar(lista_borrar_de_posicion(lista, 0) == 0, "Borrar de posición 0 con lista de un elemento");
    pa2m_afirmar(lista_vacia(lista), "La lista queda vacía");
    int i;
    for (i = 0; i < 1000; ++i) {
        lista_insertar(lista, &i);
    }
    while (lista_borrar_de_posicion(lista, 0) == 0)
        ;
    pa2m_afirmar(lista_vacia(lista), "Puedo insertar 1000 elementos y borrar en la posición 0 hasta que la lista se vacíe");
    lista_destruir(lista);
}

void probar_iterador_interno()
{
    pa2m_mensaje("Pruebas de iterador interno");
    lista_t* lista = lista_crear();
    int i, contador = 0;
    for (i = 0; i < 200; ++i) {
        lista_insertar(lista, &i);
    }
    pa2m_afirmar(lista_con_cada_elemento(lista, NULL, &contador) == 0, "Lista por cada elemento con función NULL no falla");
    pa2m_afirmar(lista_con_cada_elemento(NULL, contar, &contador) == 0, "Lista por cada elemento con lista NULL no falla");
    pa2m_afirmar(lista_con_cada_elemento(NULL, NULL, &contador) == 0, "Lista por cada elemento con lista NULL y función NULL no falla");
    pa2m_afirmar(lista_con_cada_elemento(lista, contar, &contador) == 200, "Lista por cada elemento funciona correctamente y devuelve 200");
    pa2m_afirmar(contador == 200, "Variable contexto es 200 luego de iterar");
    contador = 0;
    lista_destruir(lista);
    lista = lista_crear();
    for (i = 0; i < 400; ++i) {
        lista_insertar(lista, &i);
    }
    pa2m_afirmar(lista_con_cada_elemento(lista, contar, &contador) == 300, "Lista por cada elemento corta correctamente y devuelve 300");
    pa2m_afirmar(contador == 300, "Variable contexto es 300 luego de iterar");

    lista_destruir(lista);
}

void probar_iterador_externo()
{
    pa2m_mensaje("Pruebas de iterador externo");
    lista_t* lista = lista_crear();
    int i, contador = 0;
    lista_insertar(lista, &contador);
    for (i = 0; i < 199; ++i) {
        lista_insertar(lista, &i);
    }
    lista_iterador_t* iterador = lista_iterador_crear(lista);
    pa2m_afirmar(lista_iterador_crear(NULL) == NULL, "Lista iterador crear con función NULL no falla");
    pa2m_afirmar(lista_iterador_avanzar(NULL) == false, "Lista iterador avanzar con función NULL no falla");
    pa2m_afirmar(lista_iterador_elemento_actual(NULL) == NULL, "Lista iterador elemento actual con función NULL no falla");
    pa2m_afirmar(lista_iterador_tiene_siguiente(NULL) == false, "Lista iterador tiene siguiente con función NULL no falla");
    pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &contador, "Lista iterador elemento actual en posicion 0 devuelve el elemento correcto");
    while (lista_iterador_tiene_siguiente(iterador)) {
        contar(lista_iterador_elemento_actual(iterador), &contador);
        if (contador == 100)
            pa2m_afirmar(lista_iterador_elemento_actual(iterador) == &i, "Lista iterador elemento actual devuelve el elemento correcto");
        lista_iterador_avanzar(iterador);
    }
    pa2m_afirmar(contador == 200, "Lista iterador externo recorre correctamente la lista");
    pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL, "Lista iterador elemento actual devuelve NULL después de llegar al último elemento");
    pa2m_afirmar(lista_iterador_avanzar(iterador) == false, "Lista iterador avanzar devuelve false después de llegar al último elemento");
    pa2m_afirmar(lista_iterador_elemento_actual(iterador) == NULL, "Lista iterador elemento actual devuelve NULL después de llegar al último elemento y avanzar");

    lista_iterador_destruir(iterador);
    lista_iterador_destruir(NULL);
    lista_destruir(lista);
}

void pruebas_de_integracion_de_lista()
{
    pa2m_mensaje("Pruebas de integración de lista");
    lista_t* lista = lista_crear();
    int contador = 0, i;
    for (i = 0; i < 1000; ++i) {
        int pos = rand() % 1001;
        lista_insertar_en_posicion(lista, &contador, (size_t)pos);
    }
    pa2m_afirmar(lista_elementos(lista) == 1000, "Se insertan 1000 elementos en posiciones aleatorias");
    for (i = 0; i < 500; ++i) {
        lista_insertar(lista, &contador);
    }
    pa2m_afirmar(lista_elementos(lista) == 1500, "Se insertan 500 elementos al final de la lista");
    pa2m_afirmar(lista_con_cada_elemento(lista, contar, &contador) == 300 && contador == 300, "Se recorren 300 elementos con iterador interno");
    contador = 0;
    pa2m_afirmar(lista_con_cada_elemento(lista, contar_hasta_el_final, &contador) == 1500 && contador == 1500, "Se recorre hasta el final con iterador interno");
    contador = 0;
    lista_iterador_t* iterador = lista_iterador_crear(lista);
    while (lista_iterador_tiene_siguiente(iterador) && lista_iterador_elemento_actual(iterador) == &contador) {
        contador++;
        lista_iterador_avanzar(iterador);
    }
    pa2m_afirmar(contador == 1500, "Se recorre hasta el final con iterador externo");
    lista_iterador_destruir(iterador);
    for (i = 0; i < 300; ++i) {
        lista_borrar(lista);
    }
    pa2m_afirmar(lista_elementos(lista) == 1200, "Se eliminan 300 elementos al final de la lista");
    for (i = 0; !lista_vacia(lista); ++i) {
        int pos = rand() % 1501;
        lista_borrar_de_posicion(lista, (size_t)pos);
    }
    pa2m_afirmar(i == 1200, "Se eliminan elementos en posiciones aleatorias hasta que lista_vacia sea true");
    pa2m_afirmar(lista_elementos(lista) == 0, "La lista queda con 0 elementos");
    lista_destruir(lista);
}

bool contar_hasta_el_final(void* elemento, void* contexto)
{
    (*(int*)contexto)++;
    return true;
}

bool contar(void* elemento, void* contexto)
{
    (*(int*)contexto)++;
    if (*(int*)contexto >= 300) {
        return false;
    }
    return true;
}

void probar_pila()
{
    lista_t* pila = lista_crear();
    pa2m_afirmar(lista_tope(pila) == NULL, "Lista tope con lista vacía devuelve NULL");
    pa2m_afirmar(lista_tope(NULL) == NULL, "Lista tope con NULL devuelve NULL");
    char orden_correcto[] = "zarathustra";
    char orden_inverso[] = "artsuhtaraz";
    char palabra_formada[12];
    for (int i = 0; orden_inverso[i] != 0; i++)
        lista_apilar(pila, &orden_inverso[i]);
    pa2m_afirmar(*((char*)lista_tope(pila)) == 'z', "Lista tope devuelve el elemento correcto");
    for (int i = 0; pila->cantidad; i++) {
        palabra_formada[i] = *((char*)lista_tope(pila));
        lista_desapilar(pila);
    }
    palabra_formada[11] = '\0';
    pa2m_afirmar(strcmp(palabra_formada, orden_correcto) == 0, "Apilo y desapilo en el orden correcto");
    lista_destruir(pila);
}

void probar_cola()
{
    lista_t* cola = lista_crear();
    pa2m_afirmar(lista_primero(cola) == NULL, "Lista primero con lista vacía devuelve NULL");
    pa2m_afirmar(lista_primero(NULL) == NULL, "Lista primero con NULL devuelve NULL");
    char orden_correcto[] = "zarathustra";
    char palabra_formada[12];
    for (int i = 0; orden_correcto[i] != 0; i++)
        lista_encolar(cola, &orden_correcto[i]);
    pa2m_afirmar(*((char*)lista_primero(cola)) == 'z', "Lista primero devuelve el elemento correcto");
    for (int i = 0; !lista_vacia(cola); i++) {
        palabra_formada[i] = *((char*)lista_primero(cola));
        lista_desencolar(cola);
    }
    palabra_formada[11] = '\0';
    pa2m_afirmar(strcmp(palabra_formada, orden_correcto) == 0, "Encolo y desencolo en el orden correcto");
    lista_destruir(cola);
}
