#include "menu.h"

char menu_ppal(bool personaje, int gimnasios)
{
    printf("\x1b[1;1H\x1b[2J");
    printf("====== \x1b[1m\x1b[93m\x1b[4mAVENTURA POKEMON\x1b[0m ======\n\nGimnasios ingresados: %d %s\n\n", gimnasios, personaje ? "- Personaje cargado correctamente." : "");
    char texto[] = "A: Agregar un gimnasio al árbol de gimnasios\nI: Comenzar la partida\nS: Simular la partida\nQ: Salir\n";
    if (!personaje)
        printf("E: Ingresar el archivo del entrenador principal\n");
    printf("%s", texto);
    printf("\n :");
    char e = leer_caracter();
    return e;
}

int ingresar_gimnasio(heap_t* heap)
{
    int resultado = 2;
    char* linea;
    while (resultado == 2) {
        printf("\x1b[1;1H\x1b[2J");
        printf("===== \x1b[1mAgregar gimnasio\x1b[0m =====\n\n");
        printf("Ingrese la ruta del archivo de gimnasios\n :");
        int antes = (int)heap_elementos(heap);
        linea = leer_linea();
        FILE* archivo = fopen(linea, "r");
        if (archivo) {
            resultado = cargar_gimnasios(heap, archivo);
            fclose(archivo);
        } else
            resultado = 0;

        if (resultado == 0 && (int)heap_elementos(heap) == antes)
            resultado = menu_error_archivo() ? 0 : 2;
        free(linea);
    }
    return resultado;
}

int ingresar_personaje(personaje_t** personaje)
{
    int resultado = 2;
    char* linea;
    while (resultado == 2) {
        printf("\x1b[1;1H\x1b[2J");
        printf("===== \x1b[1mCargar personaje\x1b[0m =====\n\n");
        printf("Ingrese la ruta del archivo de personaje\n :");
        linea = leer_linea();
        FILE* archivo = fopen(linea, "r");
        if (archivo) {
            resultado = cargar_personaje(archivo, personaje);
            fclose(archivo);
        } else
            resultado = 0;

        if (resultado == 0)
            resultado = menu_error_archivo() ? 0 : 2;
        free(linea);
    }
    return resultado;
}

bool menu_error_archivo()
{
    char c = '\0';
    while (c != 'q' && c != 'r') {
        printf("\x1b[1;1H\x1b[2J");
        printf("Se produjo un error, el archivo no existe o no tiene el formato correcto.\n\nR: Volver a ingresar\nQ: Salir\n :");
        c = leer_caracter();
        if (c == 'q')
            return true;
    }
    return false;
}

int menu_gimnasio(personaje_t* personaje, gimnasio_t* gimnasio)
{
    char texto[] = "===== Gimnasio: \033[1m%s\033[0m =====\nCombates restantes hasta ganar la medalla: %zu\n\nE: Mostrar la información del entrenador principal\nG: Mostrar la información del gimnasio actual\nC: Cambiar los Pokemones de batalla\nB: Realizar la próxima batalla planificada\n\n :";
    char c = '\0';
    while (c != 'b') {
        printf("\x1b[1;1H\x1b[2J");
        printf(texto, gimnasio->nombre, lista_elementos(gimnasio->entrenadores));
        c = leer_caracter();
        printf("\x1b[1;1H\x1b[2J");
        switch (c) {
        case 'e':
            if (informacion_personaje(personaje) == ERROR)
                return ERROR;
            printf("\n---Presione Enter para volver---\n");
            leer_caracter();
            break;
        case 'g':
            if (informacion_gimnasio(gimnasio) == ERROR)
                return ERROR;
            printf("\n---Presione Enter para volver---\n");
            leer_caracter();
            break;
        case 'c':
            if (menu_cambio_pokemones(personaje) == ERROR)
                return ERROR;
            break;
        }
    }
    return EXITO;
}

void menu_combate(personaje_t* personaje, entrenador_t* entre, pokemon_t* poke_pe, pokemon_t* poke_en, int resultado)
{
    char c = '\0';
    while (c != 'n') {
        printf("\x1b[1;1H\x1b[2J");
        printf("===== Menu batalla ======\n\n");
        printf("Pokemon de %s: %s (mejora %d)\n", personaje->nombre, poke_pe->nombre, poke_pe->mejora);
        printf("- Velocidad: %d \n- Ataque: %d\n- Defensa: %d\n\n", poke_pe->velocidad, poke_pe->ataque, poke_pe->defensa);
        printf("----- VS. -----\n\n");
        printf("Pokemon de %s: %s\n", entre->nombre, poke_en->nombre);
        printf("- Velocidad: %d \n- Ataque: %d\n- Defensa: %d\n\n", poke_en->velocidad, poke_en->ataque, poke_en->defensa);
        printf("---------------\nResultado de la batalla:\n");
        if (resultado == 1)
            printf("Ganador \x1b[32;1m%s\x1b[0m\n", poke_pe->nombre);
        else
            printf("Ganador \x1b[31;1m%s\x1b[0m\n", poke_en->nombre);
        printf("\nN: Próximo combate\n :");
        c = leer_caracter();
    }
}

int menu_victoria(personaje_t* personaje, entrenador_t* entre, gimnasio_t* gym)
{
    bool poke_tomado = false;
    char texto[] = "T: Tomar un Pokemon prestado del lider\nC: Cambiar los Pokemones de batalla\nN: Próximo gimnasio";
    char texto_sin_t[] = "C: Cambiar los Pokemones de batalla\nN: Próximo gimnasio";
    char c = '\0';
    while (c != 'n') {
        printf("\x1b[1;1H\x1b[2J");
        printf("===== \x1b[93mDerrotaste al lider y ganaste la medalla de %s!\x1b[0m =====\n\n", gym->nombre);
        printf("%s\n\n :", poke_tomado ? texto_sin_t : texto);
        c = leer_caracter();
        printf("\x1b[1;1H\x1b[2J");
        switch (c) {
        case 't':
            if (!poke_tomado) {
                if (menu_tomar_pokemon(personaje, entre) == ERROR)
                    return ERROR;
                poke_tomado = true;
            }
            break;
        case 'c':
            if (menu_cambio_pokemones(personaje) == ERROR)
                return ERROR;
            break;
        }
    }
    return EXITO;
}

int menu_derrota(personaje_t* personaje)
{
    char texto[] = "===== \x1b[91mPerdiste la batalla\x1b[0m =====\n\nC: Cambiar los Pokemones de batalla\nR: Reintentar el gimnasio\nF: Finalizar la partida\n\n :";
    char c = '\0';
    while (c != 'f' && c != 'r') {
        printf("\x1b[1;1H\x1b[2J");
        printf("%s", texto);
        c = leer_caracter();
        printf("\x1b[1;1H\x1b[2J");
        switch (c) {
        case 'c':
            if (menu_cambio_pokemones(personaje) == ERROR)
                return ERROR;
            break;
        case 'r':
            return 1;
        case 'f':
            return 0;
        }
    }
    return EXITO;
}

int menu_tomar_pokemon(personaje_t* personaje, entrenador_t* entre)
{
    int num = -1;
    while (num < 0 || num >= lista_elementos(entre->pokemones)) {
        printf("\x1b[1;1H\x1b[2J");
        printf("Selecciona uno de los Pokemones de %s:\n\n", entre->nombre);
        if (listar_pokemones_combate(entre->pokemones, false) == ERROR)
            return ERROR;
        printf("\nIngrese el número del Pokemon (en amarillo) a tomar prestado\n\n :");
        num = leer_numero();
    }
    pokemon_t* nuevo_pokemon = lista_elemento_en_posicion(entre->pokemones, (size_t)num);
    if (lista_borrar_de_posicion(entre->pokemones, (size_t)num) == ERROR)
        return ERROR;
    nuevo_pokemon->id = (int)arbol_elementos(personaje->poke_obtenidos);
    if (arbol_insertar(personaje->poke_obtenidos, nuevo_pokemon) == ERROR) {
        destructor_pokemones(nuevo_pokemon);
        return ERROR;
    }
    printf("\nHas obtenido a %s!\n", nuevo_pokemon->nombre);
    printf("\n---Presione Enter para volver---\n");
    leer_caracter();
    return EXITO;
}

int listar_pokemones_combate(lista_t* lista, bool id)
{
    int i = 0;
    lista_iterador_t* it = lista_iterador_crear(lista);
    if (!it)
        return ERROR;
    printf("======================== \x1b[93;1mCOMBATE\x1b[0m ========================\n");
    printf("   | %-8s | %-8s | %-8s | %-8s \n", "Mejora", "Ataque", "Defensa", "Velocidad");
    while (lista_iterador_tiene_siguiente(it)) {
        pokemon_t* poke = lista_iterador_elemento_actual(it);
        printf("\x1b[93;1m%2d\x1b[0m | %-8d | %-8d | %-8d | %-8d | %s\n", id ? poke->id : i, poke->mejora, poke->ataque, poke->defensa, poke->velocidad, poke->nombre);
        lista_iterador_avanzar(it);
        i++;
    }
    lista_iterador_destruir(it);
    return EXITO;
}

int listar_pokemones_obtenidos(abb_t* arbol)
{
    int i = 0;
    printf("\n======================= \x1b[93;1mOBTENIDOS\x1b[0m =======================\n");
    printf("   | %-8s | %-8s | %-8s | %-8s \n", "Mejora", "Ataque", "Defensa", "Velocidad");
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, abb_mostrar_info_poke, &i);
    return EXITO;
}

bool abb_mostrar_info_poke(void* pokemon, void* extra)
{
    pokemon_t* poke = pokemon;
    printf("\x1b[93;1m%2d\x1b[0m | %-8d | %-8d | %-8d | %-8d | %s\n", poke->id, poke->mejora, poke->ataque, poke->defensa, poke->velocidad, poke->nombre);
    *(int*)extra += 1;
    return false;
}

int menu_cambio_pokemones(personaje_t* personaje)
{
    int total = (int)arbol_elementos(personaje->poke_obtenidos);
    int num = -1;
    int ingresados[total];
    for (int i = 0; i < 6; ++i) {
        while (num < 0 || num >= total) {
            printf("\x1b[1;1H\x1b[2J");
            printf("===== Menu de cambio de Pokemones =====\n");
            if (listar_pokemones_obtenidos(personaje->poke_obtenidos) == ERROR)
                return ERROR;
            printf("\nSeleccionados (%d/%d): ", i, 6);
            for (int j = 0; j < i; ++j)
                printf("%d ", ingresados[j]);
            printf("\n");
            printf("Ingrese el número del Pokemon (en naranja) a seleccionar\n\n :");
            num = leer_numero();
            for (int j = 0; j < i; ++j)
                if (num == ingresados[j])
                    num = -1;
        }
        ingresados[i] = num;
        num = -1;
    }
    if (nuevos_pokes_combate(personaje, ingresados) == ERROR)
        return ERROR;
    printf("\x1b[1;1H\x1b[2J");
    printf("Pokemones de combate seleccionados correctamente.\nEl orden en que aparecen será el orden que mantengan en la batalla\n\n");
    if (listar_pokemones_combate(personaje->poke_combate, true) == ERROR)
        return ERROR;
    printf("\n---Presione Enter para volver---");
    leer_caracter();
    return EXITO;
}

int nuevos_pokes_combate(personaje_t* personaje, int nuevos[])
{
    lista_t* lista_nueva = lista_crear();
    if (!lista_nueva)
        return ERROR;
    for (int i = 0; i < 6; ++i) {
        pokemon_t pokemon_a_buscar;
        pokemon_a_buscar.id = nuevos[i];
        pokemon_t* poke_a_insertar = arbol_buscar(personaje->poke_obtenidos, &pokemon_a_buscar);
        if (lista_insertar(lista_nueva, poke_a_insertar) == ERROR) {
            lista_destruir(lista_nueva, NULL);
            return ERROR;
        }
    }
    lista_destruir(personaje->poke_combate, NULL);
    personaje->poke_combate = lista_nueva;
    return EXITO;
}

int menu_maestro_pokemon(int res, personaje_t* personaje)
{
    if (res == 1) {
        printf("=================================================================\n");
        printf("=== \x1b[93m\x1b[1mFelicitaciones %s! Te convertiste en Maestro Pokemon!\x1b[0m ===\n", personaje->nombre);
        printf("=================================================================\n");
    } else
        printf("===== \x1b[91m\x1b[1mPerdiste la partida\x1b[0m =====\n");

    printf("\n\nPokemones finales:\n");
    if (listar_pokemones_combate(personaje->poke_combate, true) == ERROR)
        return ERROR;
    return EXITO;
}

int informacion_gimnasio(gimnasio_t* gym)
{
    printf("===== Gimnasio: \033[1m%s\033[0m =====\n\n", gym->nombre);
    printf("Dificultad: %d\n", gym->dificultad);
    printf("Combates restantes hasta ganar la medalla: %zu\n\n", lista_elementos(gym->entrenadores));
    entrenador_t* entre = lista_tope(gym->entrenadores);
    printf("Siguiente batalla contra: \033[1m%s\033[0m\n", entre->nombre);
    if (listar_pokemones_combate(entre->pokemones, false) == ERROR)
        return ERROR;
    return EXITO;
}

int informacion_personaje(personaje_t* personaje)
{
    printf("\x1b[1;1H\x1b[2J");
    printf("Entrenador principal: \033[1m%s\033[0m\n\n", personaje->nombre);
    if (listar_pokemones_combate(personaje->poke_combate, true) == ERROR)
        return ERROR;
    if (listar_pokemones_obtenidos(personaje->poke_obtenidos) == ERROR)
        return ERROR;

    return EXITO;
}

char leer_caracter()
{
    char* line = NULL;
    size_t length = 0;
    int leidos = 0;
    char caracter = '\0';
    if ((leidos = getlinea(&line, &length, stdin)) > 0) {
        caracter = line[0];
        if (caracter > 'A' && caracter < 'Z')
            caracter = (char)((int)caracter + (int)'a' - (int)'A');
    }
    free(line);
    return caracter;
}

char* leer_linea()
{
    char* line = NULL;
    size_t length = 0;
    int leidos;
    if ((leidos = getlinea(&line, &length, stdin)) > 0) {
        return line;
    }
    free(line);
    return NULL;
}

int leer_numero()
{
    char* line = NULL;
    size_t length = 0;
    int leidos = 0;
    int num = -1;
    if ((leidos = getlinea(&line, &length, stdin)) > 0) {
        if (line[0] >= '0' && line[0] <= '9')
            num = atoi(line);
    }
    free(line);
    return num;
}

int getlinea(char** string, size_t* max, FILE* stream)
{
    int leidos = 0;
    if (*string == NULL || *max <= 1) {
        *max = 50;
        *string = malloc(sizeof(char) * (*max));
        if (*string == NULL) {
            *max = 0;
            return -1;
        }
    }
    int caracter = fgetc(stream);
    while (caracter != '\n' && caracter != EOF) {
        (*string)[leidos] = (char)caracter;
        leidos++;
        if (leidos >= *max - 2) {
            *max *= 2;
            char* nuevo_string = realloc(*string, sizeof(char) * (*max));
            if (nuevo_string == NULL) {
                *max = 0;
                return -1;
            }
            *string = nuevo_string;
        }
        caracter = fgetc(stream);
    }
    (*string)[leidos] = '\0';
    return leidos > 0 ? leidos : -1;
}