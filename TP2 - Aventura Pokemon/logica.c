#include "logica.h"

int partida_manual(heap_t* heap, personaje_t* personaje)
{
    int resultado = 1;
    while (!heap_vacio(heap) && resultado == 1) {
        gimnasio_t* gym = heap_raiz(heap);
        while (!lista_vacia(gym->entrenadores) && resultado == 1) {
            if (menu_gimnasio(personaje, gym) == ERROR)
                return ERROR;
            entrenador_t* entre = lista_tope(gym->entrenadores);
            int res_entrenador = batalla_entrenador(gym, personaje, entre);
            if (res_entrenador == ERROR)
                return ERROR;
            else if (res_entrenador == 0)
                resultado = 0;
        }
        if (heap_eliminar(heap) == NULL)
            return ERROR;
        destructor_gimnasios(gym);
    }
    return resultado;
}

int batalla_entrenador(gimnasio_t* gym, personaje_t* personaje, entrenador_t* entre)
{
    int res_combate = combate(personaje, entre, gym->fn_batalla, true);
    if (res_combate == 0) {
        int res_menu_derrota = menu_derrota(personaje);
        if (res_menu_derrota == ERROR)
            return ERROR;
        else if (res_menu_derrota == 0)
            return 0;
    } else if (res_combate == 1) {
        if (lista_desapilar(gym->entrenadores) == -1)
            return ERROR;
        if (lista_vacia(gym->entrenadores)) {
            if (menu_victoria(personaje, entre, gym) == ERROR) {
                destructor_entrenadores(entre);
                return ERROR;
            }
        }
        destructor_entrenadores(entre);
    } else
        return ERROR;
    return EXITO;
}

int partida_simulada(heap_t* heap, personaje_t* personaje)
{
    int resultado = 1;
    while (!heap_vacio(heap) && resultado) {
        gimnasio_t* gym = heap_eliminar(heap);
        if (!gym)
            return ERROR;
        while (!lista_vacia(gym->entrenadores) && resultado) {
            entrenador_t* entre = lista_tope(gym->entrenadores);
            int res_combate = combate(personaje, entre, gym->fn_batalla, false);
            if (res_combate != EXITO)
                resultado = res_combate;

            if (lista_desapilar(gym->entrenadores) == -1)
                resultado = ERROR;
            destructor_entrenadores(entre);
        }
        destructor_gimnasios(gym);
    }
    return resultado;
}

int combate(personaje_t* personaje, entrenador_t* entrenador, int (*funcion_batalla)(void*, void*), bool manual)
{
    lista_t* lista_pe = personaje->poke_combate;
    lista_t* lista_en = entrenador->pokemones;
    lista_iterador_t* it_pe = lista_iterador_crear(lista_pe);
    if (!it_pe)
        return ERROR;
    lista_iterador_t* it_en = lista_iterador_crear(lista_en);
    if (!it_en) {
        free(it_pe);
        return ERROR;
    }
    while (lista_iterador_tiene_siguiente(it_en) && lista_iterador_tiene_siguiente(it_pe)) {
        pokemon_t* poke_pe = lista_iterador_elemento_actual(it_pe);
        pokemon_t* poke_en = lista_iterador_elemento_actual(it_en);
        int res = funcion_batalla(poke_pe, poke_en);
        if (manual)
            menu_combate(personaje, entrenador, poke_pe, poke_en, res);
        if (res == 1) {
            if (poke_pe->mejora < 63) {
                poke_pe->mejora++;
                poke_pe->ataque++;
                poke_pe->velocidad++;
                poke_pe->defensa++;
            }
            lista_iterador_avanzar(it_en);
        } else
            lista_iterador_avanzar(it_pe);
    }
    int ganador = lista_iterador_tiene_siguiente(it_pe) ? 1 : 0;
    lista_iterador_destruir(it_pe);
    lista_iterador_destruir(it_en);
    return ganador;
}
