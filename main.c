#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphquest.h"

int main()
{
    system("chcp 850");
    system("cls");

    // Menu
    // 1. Cargar mapa CSV
    // 2. Iniciar partida
    // 3. Salir del programa

    printf("Menú principal GraphQuest\n");
    printf("1. Cargar mapa CSV\n");
    printf("2. Iniciar partida\n");
    printf("3. Salir del programa\n");

    int opcion;

    do
    {
        printf("Seleccione una opción\n");

        switch (opcion)
        {
        // 1. Cargar mapa CSV
        case 1:
            cargarMapa();
            break;

        // 2. Iniciar partida
        case 2:
            inciarPartida();
            break;

        // 3. Salir del programa
        case 3:
            break;
        }

    } while (opcion < 1 || opcion > 3);
}