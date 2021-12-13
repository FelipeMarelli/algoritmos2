#include "evento_pesca.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool seleccion1(pokemon_t*);
bool seleccion2(pokemon_t*);
bool seleccion3(pokemon_t*);
bool seleccion4(pokemon_t*);
bool seleccion5(pokemon_t*);
void mostrar_pokemon(pokemon_t*);

int main()
{
    arrecife_t* arrecife;
    acuario_t* acuario;
    if ((arrecife = crear_arrecife("arrecife.txt")) == NULL) {
        printf("error\n");
        return 0;
    }
    acuario = crear_acuario();
    censar_arrecife(arrecife, mostrar_pokemon);
    printf("\n----- %d -----\n", arrecife->cantidad_pokemon);
    trasladar_pokemon(arrecife, acuario, seleccion1, 5);
    trasladar_pokemon(arrecife, acuario, seleccion4, 5);
    trasladar_pokemon(arrecife, acuario, seleccion5, 5);
    censar_arrecife(arrecife, mostrar_pokemon);

    guardar_datos_acuario(acuario, "out.txt");
    liberar_acuario(acuario);
    liberar_arrecife(arrecife);
    return 0;
}

void mostrar_pokemon(pokemon_t* pokemon_p)
{
    printf("%s %d %d %s\n", pokemon_p->especie, pokemon_p->velocidad, pokemon_p->peso, pokemon_p->color);
    return;
}

bool seleccion1(pokemon_t* pokemon)
{
    int i = 0;
    while (pokemon->especie[i] != '\0') {
        if (pokemon->especie[i] == 'a' || pokemon->especie[i] == 'A') {
            return false;
        }
        i++;
    }
    return true;
}

bool seleccion2(pokemon_t* pokemon)
{
    if (rand() < (int)(RAND_MAX / 2)) {
        return true;
    }
    return false;
}

bool seleccion3(pokemon_t* pokemon)
{
    if (pokemon->velocidad % 2) {
        return true;
    }
    return false;
}

bool seleccion4(pokemon_t* pokemon)
{
    if (pokemon->peso < 10) {
        return true;
    }
    return false;
}

bool seleccion5(pokemon_t* pokemon)
{
    if (strcmp(pokemon->color, "rojo") && strcmp(pokemon->color, "verde") && strcmp(pokemon->color, "azul")) {
        return true;
    }
    return false;
}
