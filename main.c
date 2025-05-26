#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "graphquest.h"

int main()
{
    system("chcp 65001");
    system("cls");

    // Menu
    // 1. Cargar mapa CSV
    // 2. Iniciar partida
    // 3. Salir del programa

    int opcion;
    Map *habitacionesMap = createMap();

    do
    {
        printf("Menú principal GraphQuest\n");
        printf("1. Cargar mapa CSV\n");
        printf("2. Iniciar partida\n");
        printf("3. Salir del programa\n");

        printf("Seleccione una opción\n");

        scanf("%d", &opcion);

        while (opcion < 1 || opcion > 3)
        {
            printf("Ingrese una opción válida (1-3)\n");
            scanf("%d", &opcion);
        }

        switch (opcion)
        {
        // 1. Cargar mapa CSV
        case 1:
            loadHabitacionesFromCSV("graphquest.csv", habitacionesMap);
            Sleep(300);
            system("cls");
            break;

        // 2. Iniciar partida
        case 2:
            iniciarPartida(habitacionesMap);
            break;

        // 3. Salir del programa
        case 3:
            printf("Saliendo del programa");
            break;

        default:
            break;
        }

    } while (opcion != 3);
}
