// Aqui van la definicion de las funciones
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

typedef struct Node
{
    struct Node *prev;
    void *data;
    struct Node *next;
} Node;

typedef struct List
{
    Node *head;
    Node *current;
    int size;
} List;

typedef struct Item
{
    char nombre[50];
    int puntos;
    int peso;
    struct Item *next;
} Item;

typedef struct Habitacion
{
    int id;
    char nombre[100];
    Item *items;
    int arriba, abajo, izquierda, derecha;
    int esFinal;
} Habitacion;

List *createList()
{
    List *nuevaLista = (List *)malloc(sizeof(List));
    if (!nuevaLista)
    {
        return NULL;
    }

    nuevaLista->head = NULL;
    nuevaLista->current = NULL;
    nuevaLista->size = 0;

    return nuevaLista;
}

// Función para eliminar la tercera columna (descripción) del archivo CSV
void eliminarColumnaDescripcion(const char *archivoOriginal, const char *archivoModificado)
{
    FILE *entrada = fopen(archivoOriginal, "r");
    FILE *salida = fopen(archivoModificado, "w");

    if (!entrada || !salida)
    {
        printf("Error al abrir los archivos.\n");
        if (entrada)
            fclose(entrada);
        if (salida)
            fclose(salida);
        return;
    }

    char linea[1024];
    while (fgets(linea, sizeof(linea), entrada))
    {
        char *token;
        char *rest = linea;

        // Leer y escribir la primera columna (ID)
        token = strtok_r(rest, ",", &rest);
        if (token)
        {
            fprintf(salida, "%s,", token);
        }

        // Leer y escribir la segunda columna (Nombre)
        token = strtok_r(rest, ",", &rest);
        if (token)
        {
            fprintf(salida, "%s,", token);
        }

        // Saltar la tercera columna (Descripción)
        if (rest && rest[0] == '"') // Si la columna empieza con comillas
        {
            rest = strchr(rest + 1, '"'); // Buscar la comilla de cierre
            if (rest)
            {
                rest = strchr(rest + 1, ','); // Saltar hasta después de la coma
                if (rest)
                {
                    rest++; // Avanzar al siguiente campo
                }
            }
        }
        else
        {
            token = strtok_r(rest, ",", &rest); // Saltar la columna si no tiene comillas
        }

        // Escribir el resto de las columnas
        if (rest)
        {
            fprintf(salida, "%s", rest);
        }
    }

    fclose(entrada);
    fclose(salida);
    printf("Archivo modificado guardado en: %s\n", archivoModificado);
}

// Función para cargar habitaciones desde un archivo CSV
List *cargarHabitacionesDesdeCSV(const char *nombreArchivo)
{
    // Crear un archivo temporal para almacenar el CSV modificado
    const char *archivoTemporal = "temp_modificado.csv";

    // Llamar a la función para eliminar la columna de descripción
    eliminarColumnaDescripcion(nombreArchivo, archivoTemporal);

    // Abrir el archivo temporal
    FILE *archivo = fopen(archivoTemporal, "r");
    if (!archivo)
        return NULL;

    List *listaHabitaciones = createList();
    if (!listaHabitaciones)
    {
        fclose(archivo);
        return NULL;
    }

    char linea[1024];
    // Leer y descartar encabezado
    fgets(linea, sizeof(linea), archivo);

    while (fgets(linea, sizeof(linea), archivo))
    {
        Habitacion *hab = (Habitacion *)malloc(sizeof(Habitacion));
        if (!hab)
            continue;

        // Inicializar campos
        hab->items = NULL;
        hab->esFinal = 0;

        // Variables auxiliares
        char *token;
        char *rest = linea;

        // ID
        token = strtok_r(rest, ",", &rest);
        hab->id = token ? atoi(token) : -1;

        // Nombre
        token = strtok_r(rest, ",", &rest);
        if (token)
        {
            strncpy(hab->nombre, token, sizeof(hab->nombre) - 1);
            hab->nombre[sizeof(hab->nombre) - 1] = '\0';
        }
        else
        {
            hab->nombre[0] = '\0';
        }

        // Items
        token = strtok_r(rest, ",", &rest);
        if (token && strlen(token) > 1)
        {
            char *itemStr = token;
            char *itemRest = NULL;
            char *itemToken = strtok_r(itemStr, ";", &itemRest); // Separar por punto y coma

            Item *ultimo = NULL;

            while (itemToken)
            {
                Item *nuevoItem = (Item *)malloc(sizeof(Item));
                if (nuevoItem)
                {
                    // Copiar el token actual para procesarlo
                    char *itemData = strdup(itemToken);
                    if (!itemData)
                    {
                        free(nuevoItem);
                        break;
                    }

                    // Leer nombre
                    char *nombre = strtok_r(itemData, ",", &itemData);
                    // Leer puntos
                    char *puntos = strtok_r(itemData, ",", &itemData);
                    // Leer peso
                    char *peso = strtok_r(itemData, ",", &itemData);

                    if (nombre)
                    {
                        strncpy(nuevoItem->nombre, nombre, sizeof(nuevoItem->nombre) - 1);
                        nuevoItem->nombre[sizeof(nuevoItem->nombre) - 1] = '\0';
                    }
                    else
                    {
                        nuevoItem->nombre[0] = '\0';
                    }

                    nuevoItem->puntos = puntos ? atoi(puntos) : 0;
                    nuevoItem->peso = peso ? atoi(peso) : 0;
                    nuevoItem->next = NULL;

                    if (!hab->items)
                    {
                        hab->items = nuevoItem;
                    }
                    else
                    {
                        ultimo->next = nuevoItem;
                    }
                    ultimo = nuevoItem;

                    free(itemData); // Liberar memoria temporal
                }

                itemToken = strtok_r(NULL, ";", &itemRest); // Siguiente item
            }
        }
        else
        {
            hab->items = NULL;
        }

        // Arriba
        token = strtok_r(rest, ",", &rest);
        hab->arriba = token ? atoi(token) : -1;
        // Abajo
        token = strtok_r(rest, ",", &rest);
        hab->abajo = token ? atoi(token) : -1;
        // Izquierda
        token = strtok_r(rest, ",", &rest);
        hab->izquierda = token ? atoi(token) : -1;
        // Derecha
        token = strtok_r(rest, ",", &rest);
        hab->derecha = token ? atoi(token) : -1;
        // EsFinal
        token = strtok_r(rest, ",\n", &rest);
        if (token && (strcmp(token, "Si") == 0 || strcmp(token, "si") == 0))
            hab->esFinal = 1;
        else
            hab->esFinal = 0;

        // Insertar en la lista
        Node *nuevoNodo = (Node *)malloc(sizeof(Node));
        if (nuevoNodo)
        {
            nuevoNodo->data = hab;
            nuevoNodo->prev = NULL;
            nuevoNodo->next = listaHabitaciones->head;
            if (listaHabitaciones->head)
                listaHabitaciones->head->prev = nuevoNodo;
            listaHabitaciones->head = nuevoNodo;
            listaHabitaciones->size++;
        }
        else
        {
            free(hab);
        }
    }

    fclose(archivo);

    // Eliminar el archivo temporal
    remove(archivoTemporal);

    return listaHabitaciones;
}

void imprimirHabitaciones(List *lista)
{
    if (!lista || !lista->head)
    {
        printf("No hay habitaciones cargadas.\n");
        return;
    }

    Node *nodo = lista->head;
    while (nodo)
    {
        Habitacion *hab = (Habitacion *)nodo->data;
        printf("ID: %d\n", hab->id);
        printf("Nombre: %s\n", hab->nombre);
        printf("Arriba: %d | Abajo: %d | Izquierda: %d | Derecha: %d\n", hab->arriba, hab->abajo, hab->izquierda, hab->derecha);
        printf("Es final: %s\n", hab->esFinal ? "Si" : "No");

        printf("Items:\n");
        Item *item = hab->items;
        if (!item)
        {
            printf("  (Sin items)\n");
        }
        while (item)
        {
            printf("  - %s (Puntos: %d, Peso: %d)\n", item->nombre, item->puntos, item->peso);
            item = item->next;
        }
        printf("-----------------------------\n");
        nodo = nodo->next;
        Sleep(500);
    }
}