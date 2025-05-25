#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

// ============================
// Estructuras ya definidas
// ============================
typedef struct Node
{
    struct Node *prev;
    struct Item *items;
    int pesoTotal;
    int puntosTotal;
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
    char descripcion[1001];
    Item *items;
    int arriba, abajo, izquierda, derecha;
    int esFinal; // 1 si es final, 0 si no
} Habitacion;

List *createList()
{
    List *lista = (List *)malloc(sizeof(List));

    if (!lista)
        return NULL;

    lista->current = NULL;
    lista->head = NULL;
    lista->size = 0;

    return lista;
}

// Ya tienes una función createList(), pero para este ejemplo usaremos un "map" para
// almacenar las habitaciones usando el id como clave.

// ============================
// TDA MAP (Árbol Binario)
// ============================
typedef struct MapNode
{
    int key; // En nuestro caso, el id de la habitación
    Habitacion *habitacion;
    struct MapNode *left;
    struct MapNode *right;
} MapNode;

typedef struct Map
{
    MapNode *root;
} Map;

Map *createMap()
{
    Map *m = (Map *)malloc(sizeof(Map));
    if (m)
        m->root = NULL;
    return m;
}

MapNode *createMapNode(int key, Habitacion *hab)
{
    MapNode *node = (MapNode *)malloc(sizeof(MapNode));
    if (node)
    {
        node->key = key;
        node->habitacion = hab;
        node->left = node->right = NULL;
    }
    return node;
}

MapNode *insertMapNode(MapNode *root, int key, Habitacion *hab)
{
    if (root == NULL)
        return createMapNode(key, hab);

    if (key < root->key)
        root->left = insertMapNode(root->left, key, hab);
    else
        root->right = insertMapNode(root->right, key, hab);
    return root;
}

void mapInsert(Map *map, int key, Habitacion *hab)
{
    if (map)
        map->root = insertMapNode(map->root, key, hab);
}

MapNode *buscarNodo(Map *habitaciones, int key)
{
    MapNode *aux = habitaciones->root;

    while (aux)
    {
        if (aux->key == key)
            return aux;
        else if (key < aux->key)
            aux = aux->left;
        else
            aux = aux->right;
    }
    return NULL; // No encontrado
}

// ============================
// Funciones para parsear CSV
// ============================

// Esta función analiza una línea CSV simple (asumiendo que cada campo puede estar
// entrecomillado o no). Se espera que existan 'expected_fields' en la línea.
void parseCSVLine(char *line, char *fields[], int expected_fields)
{
    int field_index = 0;
    char *p = line;
    while (*p && field_index < expected_fields)
    {
        // Si el campo inicia con comilla, tomar todo lo que haya hasta la siguiente comilla
        if (*p == '"')
        {
            p++; // saltar la comilla de apertura
            fields[field_index++] = p;
            while (*p && *p != '"')
            {
                p++;
            }
            if (*p == '"')
            {
                *p = '\0'; // termina el campo al encontrar la comilla de cierre
                p++;       // avanzamos
                if (*p == ',')
                    p++; // saltamos la coma
            }
        }
        else
        {
            // Campo sin comillas
            fields[field_index++] = p;
            while (*p && *p != ',' && *p != '\n')
            {
                p++;
            }
            if (*p)
            {
                *p = '\0';
                p++;
            }
        }
    }
}

// Función auxiliar para "limpiar" una cadena: elimina espacios y comillas al inicio y final.
void trimString(char *str)
{
    // Eliminar espacios y comillas al inicio.
    while (*str == ' ' || *str == '\"')
        memmove(str, str + 1, strlen(str) + 1);
    // Eliminar espacios y comillas al final.
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\"'))
    {
        str[len - 1] = '\0';
        len--;
    }
}

// Función que parsea la cadena de ítems y retorna una lista enlazada de Item.
// Cada ítem tiene el formato: nombre,puntos,peso y se separan mediante ';'.
// Se utiliza strtok_s para evitar la interferencia de llamadas anidadas.
Item *parseItems(const char *items_str)
{
    if (items_str == NULL || items_str[0] == '\0')
        return NULL;

    char *items_copy = _strdup(items_str);
    if (!items_copy)
        return NULL;

    Item *head = NULL;
    Item *tail = NULL;
    char *context1 = NULL;

    char *token = strtok_s(items_copy, ";", &context1);
    while (token != NULL)
    {
        char *item_str = _strdup(token);
        if (!item_str)
            break;

        trimString(item_str);

        char *commaContext = NULL;
        char *nombre_item = strtok_s(item_str, ",", &commaContext);
        char *puntos_str = strtok_s(NULL, ",", &commaContext);
        char *peso_str = strtok_s(NULL, ",", &commaContext);

        Item *newItem = (Item *)malloc(sizeof(Item));
        if (!newItem)
        {
            free(item_str);
            break;
        }
        newItem->next = NULL;
        if (nombre_item)
        {
            trimString(nombre_item);
            strncpy(newItem->nombre, nombre_item, 49);
            newItem->nombre[49] = '\0';
        }
        else
        {
            newItem->nombre[0] = '\0';
        }
        newItem->puntos = puntos_str ? atoi(puntos_str) : 0;
        newItem->peso = peso_str ? atoi(peso_str) : 0;

        free(item_str);

        if (head == NULL)
        {
            head = newItem;
            tail = newItem;
        }
        else
        {
            tail->next = newItem;
            tail = newItem;
        }

        token = strtok_s(NULL, ";", &context1);
    }
    free(items_copy);
    return head;
}

// ============================
// Función para leer el archivo CSV y cargar las habitaciones en el Map
// ============================
// Modificación en la carga de habitaciones para incluir la descripción.
void loadHabitacionesFromCSV(const char *filename, Map *map)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("No se pudo abrir el archivo %s\n", filename);
        return;
    }

    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    { // Saltar la cabecera
        printf("El archivo está vacío.\n");
        fclose(file);
        return;
    }

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (strlen(buffer) <= 1)
            continue;

        char *fields[9];
        parseCSVLine(buffer, fields, 9);

        Habitacion *hab = (Habitacion *)malloc(sizeof(Habitacion));
        if (!hab)
            continue;

        hab->id = atoi(fields[0]);

        strncpy(hab->nombre, fields[1], 99);
        hab->nombre[99] = '\0';
        trimString(hab->nombre);

        strncpy(hab->descripcion, fields[2], 999);
        hab->descripcion[999] = '\0';
        trimString(hab->descripcion);

        hab->items = parseItems(fields[3]);
        hab->arriba = atoi(fields[4]);
        hab->abajo = atoi(fields[5]);
        hab->izquierda = atoi(fields[6]);
        hab->derecha = atoi(fields[7]);
        hab->esFinal = (_stricmp(fields[8], "Si") == 0) ? 1 : 0;

        mapInsert(map, hab->id, hab);
    }
    fclose(file);

    printf("Habitaciones cargadas exitosamente\n");
}

// Función que imprime una única habitación con el formato requerido.
void imprimirHabitacion(Map *habitaciones, int key)
{
    Habitacion *hab = buscarNodo(habitaciones, key)->habitacion;

    printf("ID: %d\n", hab->id);
    printf("Nombre: %s\n", hab->nombre);
    printf("Descripción: %s\n", hab->descripcion);
    printf("Arriba: %d | Abajo: %d | Izquierda: %d | Derecha: %d\n",
           hab->arriba, hab->abajo, hab->izquierda, hab->derecha);
    printf("Es final: %s\n", (hab->esFinal ? "Si" : "No"));
    printf("Items:\n");

    // Se verifica si la habitación tiene o no items.
    if (hab->items == NULL)
    {
        printf("  - Ninguno\n");
    }
    else
    {
        Item *item = hab->items;
        // Recorremos la lista enlazada de items.
        while (item != NULL)
        {
            printf("  - %s (Puntos: %d, Peso: %d kg)\n", item->nombre, item->puntos, item->peso);
            item = item->next;
        }
    }
    printf("-----------------------------\n");
}

void iniciarPartida(Map *habitaciones)
{
    /*### **Estado Actual**

    - Descripción del escenario actual.
    - Lista de ítems disponibles en este escenario (con nombre, peso y valor).
    - Tiempo restante (puedes inicializarlo en 10 o el tiempo que estimes conveniente).
    - Inventario del jugador (ítems recogidos, peso total y puntaje acumulado).
    - Acciones posibles desde este escenario: direcciones disponibles (arriba, abajo, izquierda, derecha).
    */

    if (habitaciones == NULL)
    {
        printf("No hay habitaciones cargadas");
        return;
    }

    float tiempo = 10;
    int idActual = 1;

    MapNode *nodoActual = buscarNodo(habitaciones, idActual);

    while (nodoActual != NULL)
    {
        Habitacion *habActual = nodoActual->habitacion;

        printf("Habitación Actual n°%d\n", habActual->id);
        printf("Descripcion: %s\n", habActual->descripcion);
        printf("Ítems disponibles: \n");
        printf("Tiempo restante: %0.f\n", tiempo);
        printf("Inventario: \n");
        printf("Acciones: arriba, abajo, izquierda, derecha\n");
        printf("-----------------------------\n");

        free(habActual);
        nodoActual = nodoActual->right;
    }
    free(nodoActual);
}
