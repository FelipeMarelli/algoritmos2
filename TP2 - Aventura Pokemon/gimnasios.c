#include "gimnasios.h"

int cargar_gimnasios(heap_t* heap, FILE* archivo)
{
    if (!heap || !archivo)
        return 0;
    int resultado = EXITO;
    char* line = NULL;
    size_t length = 0;
    int char_leidos;
    char ultima_letra = '\0';

    gimnasio_t* gym = nuevo_gym();
    if (!gym)
        return ERROR;
    char_leidos = getlinea(&line, &length, archivo);
    while (char_leidos != -1 && resultado == EXITO) {
        if (line[0] == 'G' && ultima_letra == 'P') {
            if (heap_insertar(heap, gym) != EXITO) {
                destructor_gimnasios(gym);
                resultado = ERROR;
            } else if ((gym = nuevo_gym()) == NULL)
                resultado = ERROR;
            ultima_letra = '\0';
        }
        if (resultado == EXITO)
            resultado = procesar_linea_gym(gym, line, &ultima_letra);
        char_leidos = getlinea(&line, &length, archivo);
    }
    if (resultado == EXITO) {
        if (ultima_letra != 'P') {
            destructor_gimnasios(gym);
            resultado = 0;
        } else if (heap_insertar(heap, gym) != EXITO) {
            destructor_gimnasios(gym);
            resultado = ERROR;
        }
    } else
        destructor_gimnasios(gym);
    free(line);
    return resultado;
}

int procesar_linea_gym(gimnasio_t* gym, char* linea, char* ult_letra)
{
    int resultado = EXITO;
    char letra_actual = linea[0];
    switch (letra_actual) {
    case 'G':
        if (*ult_letra != '\0')
            return 0;
        resultado = procesar_gimnasio(gym, linea);
        break;
    case 'L':
        if (*ult_letra != 'G')
            return 0;
        resultado = procesar_entrenador(gym, linea);
        break;
    case 'E':
        if (*ult_letra != 'P')
            return 0;
        resultado = procesar_entrenador(gym, linea);
        break;
    case 'P':
        if (*ult_letra == 'G' || *ult_letra == '\0')
            return 0;
        resultado = gym_procesar_pokemon(gym, linea);
        break;
    default:
        return 0;
    }
    *ult_letra = letra_actual;
    return resultado;
}

int procesar_gimnasio(gimnasio_t* gym, char* linea)
{
    funcion_batalla fns_batalla[5] = { funcion_batalla_1, funcion_batalla_2, funcion_batalla_3,
        funcion_batalla_4, funcion_batalla_5 };
    char dificultad[3];
    char id[3];
    int campos_leidos = sscanf(linea, MASCARA_GIMNASIO, gym->nombre, dificultad, id);
    if (campos_leidos != 3) {
        return 0;
    }
    gym->dificultad = atoi(dificultad);
    int id_funcion = atoi(id);
    if (id_funcion < 1 || id_funcion > 5)
        return 0;
    gym->fn_batalla = fns_batalla[id_funcion - 1];
    return EXITO;
}

int procesar_entrenador(gimnasio_t* gym, char* linea)
{
    char nombre[MAX_NOMBRE];
    int campos_leidos = sscanf(linea, MASCARA_ENTRENADOR, nombre);
    if (campos_leidos != 1)
        return 0;
    entrenador_t* entren;
    if ((entren = nuevo_entrenador()) == NULL)
        return ERROR;
    strcpy(entren->nombre, nombre);
    if (lista_apilar(gym->entrenadores, entren) == -1) {
        destructor_entrenadores(entren);
        return ERROR;
    }
    return EXITO;
}

int gym_procesar_pokemon(gimnasio_t* gym, char* linea)
{
    pokemon_t* poke;
    int res = interpretar_linea_pokemon(linea, &poke);
    if (res != EXITO)
        return res;
    entrenador_t* entren = lista_tope(gym->entrenadores);
    if (!entren || lista_elementos(entren->pokemones) > 5) {
        free(poke);
        return 0;
    }
    if (lista_insertar(entren->pokemones, poke) == -1) {
        free(poke);
        return ERROR;
    }
    return EXITO;
}

int interpretar_linea_pokemon(char* linea, pokemon_t** pokemon)
{
    char velocidad[3];
    char ataque[3];
    char defensa[3];
    char nombre[MAX_NOMBRE];
    int campos_leidos = sscanf(linea, MASCARA_POKEMON, nombre, velocidad, ataque, defensa);
    if (campos_leidos != 4)
        return 0;
    pokemon_t* poke;
    if ((poke = nuevo_pokemon()) == NULL)
        return ERROR;
    strcpy(poke->nombre, nombre);
    poke->velocidad = atoi(velocidad);
    poke->ataque = atoi(ataque);
    poke->defensa = atoi(defensa);
    *pokemon = poke;
    return EXITO;
}

gimnasio_t* nuevo_gym()
{
    gimnasio_t* gym = malloc(sizeof(gimnasio_t));
    if (!gym)
        return NULL;
    gym->nombre[0] = '\0';
    gym->dificultad = -1;
    gym->fn_batalla = NULL;
    if ((gym->entrenadores = lista_crear()) == NULL) {
        free(gym);
        return NULL;
    }
    return gym;
}

entrenador_t* nuevo_entrenador()
{
    entrenador_t* entrenador = malloc(sizeof(entrenador_t));
    if (!entrenador)
        return NULL;
    entrenador->nombre[0] = '\0';
    if ((entrenador->pokemones = lista_crear()) == NULL) {
        free(entrenador);
        return NULL;
    }
    return entrenador;
}

pokemon_t* nuevo_pokemon()
{
    pokemon_t* poke = malloc(sizeof(pokemon_t));
    if (!poke)
        return NULL;
    poke->nombre[0] = '\0';
    poke->velocidad = -1;
    poke->defensa = -1;
    poke->ataque = -1;
    poke->mejora = 0;
    poke->id = 0;
    return poke;
}

//---------- DESTRUCTORES Y COMPARADOR -------------

int comparador_gimnasios(void* elem1, void* elem2)
{
    if (((gimnasio_t*)elem1)->dificultad >= ((gimnasio_t*)elem2)->dificultad)
        return 1;
    return -1;
}

void destructor_gimnasios(void* elem)
{
    if (elem) {
        gimnasio_t* gym = elem;
        if (gym->entrenadores)
            lista_destruir(gym->entrenadores, destructor_entrenadores);
        free(elem);
    }
}

void destructor_entrenadores(void* elem)
{
    if (elem) {
        entrenador_t* entren = elem;
        if (entren->pokemones)
            lista_destruir(entren->pokemones, destructor_pokemones);
        free(elem);
    }
}

void destructor_pokemones(void* elem)
{
    if (elem)
        free(elem);
}
