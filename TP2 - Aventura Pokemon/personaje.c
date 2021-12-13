#include "personaje.h"

int cargar_personaje(FILE* archivo, personaje_t** personaje)
{
    int resultado = EXITO;
    char* line = NULL;
    size_t length = 0;
    int char_leidos;
    char ultima_letra = '\0';

    *personaje = inicializar_personaje();
    if (!*personaje)
        return ERROR;
    char_leidos = getlinea(&line, &length, archivo);
    while (char_leidos != -1 && resultado == EXITO) {
        resultado = procesar_linea_pers(*personaje, line, &ultima_letra);
        char_leidos = getlinea(&line, &length, archivo);
    }

    if (resultado == EXITO) {
        if (ultima_letra != 'P' || lista_elementos((*personaje)->poke_combate) < 6) {
            destruir_personaje(*personaje);
            resultado = 0;
            *personaje = NULL;
        }
    } else {
        destruir_personaje(*personaje);
        *personaje = NULL;
    }
    free(line);
    return resultado;
}

int procesar_linea_pers(personaje_t* personaje, char* linea, char* ult_letra)
{
    int resultado = EXITO;
    char letra_actual = linea[0];
    if (letra_actual == 'E') {
        if (*ult_letra != '\0')
            return 0;
        resultado = procesar_personaje(personaje, linea);
    } else if (letra_actual == 'P') {
        if (*ult_letra != 'E' && *ult_letra != 'P')
            return 0;
        resultado = pers_procesar_pokemon(personaje, linea);
    } else
        return 0;
    *ult_letra = letra_actual;
    return resultado;
}

int pers_procesar_pokemon(personaje_t* personaje, char* linea)
{
    pokemon_t* poke = NULL;
    int res = interpretar_linea_pokemon(linea, &poke);
    if (res != EXITO) {
        free(poke);
        return res;
    }
    poke->id = (int)arbol_elementos(personaje->poke_obtenidos);
    if (arbol_insertar(personaje->poke_obtenidos, poke) == -1) {
        free(poke);
        return ERROR;
    }
    if (lista_elementos(personaje->poke_combate) < 6) {
        if (lista_insertar(personaje->poke_combate, poke) == -1) {
            return ERROR;
        }
    }
    return EXITO;
}

int procesar_personaje(personaje_t* personaje, char* linea)
{
    char nombre[MAX_NOMBRE];
    int campos_leidos = sscanf(linea, MASCARA_PERSONAJE, nombre);
    if (campos_leidos != 1)
        return 0;
    strcpy(personaje->nombre, nombre);
    return EXITO;
}

personaje_t* inicializar_personaje()
{
    personaje_t* personaje = malloc(sizeof(personaje_t));
    if (!personaje)
        return NULL;
    personaje->poke_combate = lista_crear();
    if (!personaje->poke_combate) {
        free(personaje);
        return NULL;
    }
    personaje->poke_obtenidos = arbol_crear(abb_comparador_pokemones, destructor_pokemones);
    if (!personaje->poke_obtenidos) {
        lista_destruir(personaje->poke_combate, NULL);
        free(personaje);
    }
    return personaje;
}

int abb_comparador_pokemones(void* poke1, void* poke2)
{
    if (((pokemon_t*)poke1)->id == ((pokemon_t*)poke2)->id)
        return 0;
    else if (((pokemon_t*)poke1)->id > ((pokemon_t*)poke2)->id)
        return 1;
    return -1;
}

void destruir_personaje(personaje_t* personaje)
{
    if (!personaje)
        return;
    if (personaje->poke_obtenidos)
        arbol_destruir(personaje->poke_obtenidos);
    if (personaje->poke_combate) {
        lista_destruir(personaje->poke_combate, NULL);
    }
    free(personaje);
}
