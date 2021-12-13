#include "evento_pesca.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

bool procesar_linea(FILE*, pokemon_t*);
bool verificar_trasladables(arrecife_t*, bool (*)(pokemon_t*), int, int*);

arrecife_t* crear_arrecife(const char* ruta_archivo)
{
    FILE* Archivo = fopen(ruta_archivo, "r");
    if (Archivo == NULL)
        return NULL;

    arrecife_t* arrecife_p = malloc(sizeof(arrecife_t));
    if (arrecife_p == NULL) {
        fclose(Archivo);
        return NULL;
    }
    arrecife_p->cantidad_pokemon = 0;
    arrecife_p->pokemon = NULL;
    pokemon_t pokemon_aux;
    while (procesar_linea(Archivo, &pokemon_aux)) {
        int indexo = arrecife_p->cantidad_pokemon;
        void* nuevo_p = realloc(arrecife_p->pokemon, sizeof(pokemon_t) * (size_t)(indexo + 1));
        if (nuevo_p == NULL) {
            free(arrecife_p->pokemon);
            free(arrecife_p);
            fclose(Archivo);
            return NULL;
        }
        arrecife_p->pokemon = nuevo_p;
        arrecife_p->pokemon[indexo] = pokemon_aux;
        arrecife_p->cantidad_pokemon++;
    }
    if (!arrecife_p->pokemon) {
        free(arrecife_p);
        fclose(Archivo);
        return NULL;
    }
    fclose(Archivo);
    return arrecife_p;
}

bool procesar_linea(FILE* Arch, pokemon_t* pokemon_p)
{
    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, Arch);
    if (feof(Arch) || buffer[strlen(buffer) - 1] != '\n')
        return false;
    char velocidad[6];
    char peso[6];
    int campos_leidos = sscanf(buffer, "%100[a-zA-Z ];%5[0-9];%5[0-9];%50[a-zA-Z ]\n", pokemon_p->especie,
        velocidad, peso, pokemon_p->color);
    if (campos_leidos != 4) {
        return false;
    }
    pokemon_p->velocidad = atoi(velocidad);
    pokemon_p->peso = atoi(peso);
    return true;
}

acuario_t* crear_acuario()
{
    acuario_t* acuario = malloc(sizeof(acuario_t));
    if (acuario == NULL) {
        return NULL;
    }
    acuario->cantidad_pokemon = 0;
    acuario->pokemon = NULL;
    return acuario;
}

int trasladar_pokemon(arrecife_t* arrecife, acuario_t* acuario, bool (*seleccionar_pokemon)(pokemon_t*), int cant_seleccion)
{
    if (cant_seleccion <= 0)
        return 0;

    int indexo_trasladables[cant_seleccion];
    if (!verificar_trasladables(arrecife, seleccionar_pokemon, cant_seleccion, indexo_trasladables))
        return -1;
    //traslado de arrecife a acuario
    pokemon_t* nuevo_arrecife_p = malloc(sizeof(pokemon_t) * (size_t)(arrecife->cantidad_pokemon - cant_seleccion));
    if (nuevo_arrecife_p == NULL)
        return -1;
    pokemon_t* nuevo_acuario_p = realloc(acuario->pokemon, sizeof(pokemon_t) * (size_t)(acuario->cantidad_pokemon + cant_seleccion));
    if (nuevo_acuario_p == NULL) {
        free(nuevo_arrecife_p);
        return -1;
    }
    int cont_trasladables = 0;
    int cont_nuevo_arrecife = 0;
    for (int j = 0; j < arrecife->cantidad_pokemon; ++j) {
        if (j == indexo_trasladables[cont_trasladables]) {
            nuevo_acuario_p[acuario->cantidad_pokemon] = arrecife->pokemon[j];
            acuario->cantidad_pokemon++;
            if (cont_trasladables < cant_seleccion - 1)
                cont_trasladables++;
        } else {
            nuevo_arrecife_p[cont_nuevo_arrecife] = arrecife->pokemon[j];
            cont_nuevo_arrecife++;
        }
    }
    arrecife->cantidad_pokemon = arrecife->cantidad_pokemon - cant_seleccion;
    free(arrecife->pokemon);
    arrecife->pokemon = nuevo_arrecife_p;
    acuario->pokemon = nuevo_acuario_p;
    return 0;
}

bool verificar_trasladables(arrecife_t* arrecife, bool (*seleccionar)(pokemon_t*), int cantidad, int* indexo_trasladables)
{
    if (arrecife->cantidad_pokemon < cantidad)
        return false;
    int trasladables = 0;
    int i = 0;
    while (trasladables < cantidad && i < arrecife->cantidad_pokemon) {
        if (seleccionar(&arrecife->pokemon[i])) {
            indexo_trasladables[trasladables] = i;
            trasladables++;
        }
        i++;
    }
    if (trasladables < cantidad)
        return false;
    return true;
}

void censar_arrecife(arrecife_t* arrecife, void (*mostrar_pokemon)(pokemon_t*))
{
    for (int i = 0; i < arrecife->cantidad_pokemon; ++i) {
        mostrar_pokemon(&arrecife->pokemon[i]);
    }
    return;
}

int guardar_datos_acuario(acuario_t* acuario, const char* nombre_archivo)
{
    FILE* Archivo = fopen(nombre_archivo, "w");
    if (Archivo == NULL) {
        return -1;
    }
    pokemon_t poke;
    for (int i = 0; i < acuario->cantidad_pokemon; ++i) {
        poke = acuario->pokemon[i];
        fprintf(Archivo, "%s;%d;%d;%s\n", poke.especie, poke.velocidad, poke.peso, poke.color);
    }
    fclose(Archivo);
    return 0;
}

void liberar_acuario(acuario_t* acuario)
{
    free(acuario->pokemon);
    free(acuario);
    return;
}

void liberar_arrecife(arrecife_t* arrecife)
{
    free(arrecife->pokemon);
    free(arrecife);
    return;
}