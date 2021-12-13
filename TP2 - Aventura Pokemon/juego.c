#include "juego.h"
#include "logica.h"
#include "menu.h"

int main(int argc, char const* argv[])
{
    personaje_t* personaje = NULL;
    heap_t* heap_gimnasios = heap_crear(comparador_gimnasios, destructor_gimnasios);
    if (!heap_gimnasios)
        return ERROR;
    char carac = '\0';
    int resultado = 0;

    while (carac != 'q' && resultado == 0) {
        carac = menu_ppal(personaje ? true : false, (int)heap_elementos(heap_gimnasios));
        switch (carac) {
        case 'e':
            if (!personaje)
                if (ingresar_personaje(&personaje) == ERROR)
                    resultado = ERROR;
            break;
        case 'a':
            if (ingresar_gimnasio(heap_gimnasios) == ERROR)
                resultado = ERROR;
            break;
        case 'i':
            if (personaje && heap_elementos(heap_gimnasios) > 0) {
                printf("\x1b[1;1H\x1b[2J");
                carac = 'q';
                int res_partida = partida_manual(heap_gimnasios, personaje);
                if (res_partida == ERROR)
                    resultado = ERROR;
                else
                    menu_maestro_pokemon(res_partida, personaje);
            } else
                printf("\nFaltan cargar archivos\n");
            printf("\n---Presione Enter para continuar---\n");
            leer_caracter();
            break;
        case 's':
            if (personaje && heap_elementos(heap_gimnasios) > 0) {
                printf("\x1b[1;1H\x1b[2J");
                carac = 'q';
                int res_partida = partida_simulada(heap_gimnasios, personaje);
                if (res_partida == ERROR)
                    resultado = ERROR;
                else
                    menu_maestro_pokemon(res_partida, personaje);
            } else
                printf("\nFaltan cargar archivos\n");
            printf("\n---Presione Enter para continuar---\n");
            leer_caracter();
            break;
        }
    }
    heap_destruir(heap_gimnasios);
    destruir_personaje(personaje);
    return resultado;
}