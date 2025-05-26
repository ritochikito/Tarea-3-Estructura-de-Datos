#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <math.h>

// ============================
// Estructuras ya definidas
// ============================
typedef struct Inventario
{
    struct Item *items;
    int pesoTotal;
    int puntosTotal;
    int size;
} Inventario;

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

Inventario *createInventario()
{
    Inventario *inv = (Inventario *)malloc(sizeof(Inventario));
    if (!inv)
        return NULL;

    inv->items = NULL;
    inv->pesoTotal = 0;
    inv->puntosTotal = 0;
    inv->size = 0;

    return inv;
}

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

void imprimirInventario(Inventario *inv)
{
    if (!inv->items)
    {
        printf("  No hay ítems en el inventario\n");
    }
    else
    {
        printf("ítems: \n");

        Item *item = inv->items;
        while (item != NULL)
        {
            printf("  - %s (Puntos: %d, Peso: %d kg)\n", item->nombre, item->puntos, item->peso);
            item = item->next;
        }
    }
    printf("  Peso total: %d KG\n", inv->pesoTotal);
    printf("  Puntos totales: %d Pts\n", inv->puntosTotal);
}

void recogerItems(Habitacion *habActual, Inventario *inv)
{
    // Verificamos si la habitación tiene algún ítem
    if (!habActual->items)
    {
        printf("No hay ítems en esta habitación.\n");
        return;
    }

    int opcion;
    do
    {
        // Contador para saber cuántos ítems hay actualmente
        int contador = 0;
        Item *current = habActual->items;

        // Imprimimos la lista actualizada de ítems disponibles
        printf("\nÍtems disponibles para recoger:\n");
        while (current != NULL)
        {
            contador++;
            printf("  %d - %s (Puntos: %d, Peso: %d kg)\n", contador, current->nombre, current->puntos, current->peso);
            current = current->next;
        }
        // Opción extra para finalizar la recolección
        printf("  %d - Finalizar recolección\n", contador + 1);
        printf("Seleccione el ítem a recoger: ");
        if (scanf("%d", &opcion) != 1)
        {
            // Evitamos problemas en lectura de la opción
            printf("Entrada no válida.\n");
            break;
        }
        if (opcion < 1 || opcion > contador + 1)
        {
            printf("Opción inválida. Intenta de nuevo.\n");
            continue;
        }
        // Si el usuario elige finalizar, se rompe el ciclo
        if (opcion == contador + 1)
        {
            break;
        }

        // Buscamos el ítem que corresponde al índice seleccionado.
        Item *prev = NULL;
        current = habActual->items;
        int index = 1;
        while (current != NULL && index < opcion)
        {
            prev = current;
            current = current->next;
            index++;
        }

        if (current != NULL)
        {
            // Extraemos el ítem de la lista de la habitación
            if (prev == NULL)
            {
                // El ítem se encuentra al inicio de la lista
                habActual->items = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            // Agregamos el ítem al inicio de la lista del inventario
            current->next = inv->items;
            inv->items = current;

            // Actualizamos los totales del inventario
            inv->puntosTotal += current->puntos;
            inv->pesoTotal += current->peso;
            inv->size++;

            printf("Has recogido: %s\n", current->nombre);
        }
        else
        {
            printf("Error al recoger el ítem. Intenta de nuevo.\n");
        }
    } while (habActual->items != NULL);

    if (habActual->items == NULL)
    {
        printf("No quedan más ítems en esta habitación.\n");
    }
    printf("-----------------------------\n");
}

void descartarItems(Habitacion *habActual, Inventario *inv)
{
    // Verificamos si el inventario tiene ítems
    if (inv->items == NULL)
    {
        printf("No tienes ítems en el inventario para descartar.\n");
        return;
    }

    int opcion;

    do
    {
        // Mostrar los ítems actuales en el inventario
        int contador = 0;
        Item *actual = inv->items;
        printf("\nÍtems en tu inventario:\n");
        while (actual != NULL)
        {
            contador++;
            printf("  %d - %s (Puntos: %d, Peso: %d kg)\n", contador, actual->nombre, actual->puntos, actual->peso);
            actual = actual->next;
        }
        // Opción para cancelar el descarte
        printf("  %d - Cancelar descarte\n", contador + 1);
        printf("Seleccione el ítem a descartar: ");

        if (scanf("%d", &opcion) != 1)
        {
            printf("Entrada no válida.\n");
            break;
        }
        if (opcion < 1 || opcion > contador + 1)
        {
            printf("Opción inválida. Intenta de nuevo.\n");
            continue;
        }
        // Si se selecciona la opción de cancelar, se sale del ciclo
        if (opcion == contador + 1)
        {
            break;
        }

        // Buscamos el ítem a descartar dentro del inventario
        Item *prev = NULL;
        actual = inv->items;
        int indice = 1;
        while (actual != NULL && indice < opcion)
        {
            prev = actual;
            actual = actual->next;
            indice++;
        }

        if (actual != NULL)
        {
            // Se elimina el ítem del inventario
            if (prev == NULL)
            {
                inv->items = actual->next;
            }
            else
            {
                prev->next = actual->next;
            }

            // Actualizamos los totales del inventario
            inv->puntosTotal -= actual->puntos;
            inv->pesoTotal -= actual->peso;
            inv->size--;

            // Se agrega el ítem a la habitación actual (al inicio de la lista de ítems de la habitación)
            actual->next = habActual->items;
            habActual->items = actual;

            printf("Has descartado: %s\n", actual->nombre);
        }

    } while (inv->items != NULL);

    if (inv->items == NULL)
    {
        printf("No quedan más ítems en el inventario para descartar.\n");
    }
    printf("-----------------------------\n");
}

Habitacion *avanzar(Map *habitaciones, Habitacion *habActual)
{
    int movimientos[4];
    int contador = 0;

    printf("Movimientos disponibles: \n");

    if (habActual->arriba != -1)
    {
        printf("  %d- Arriba\n", contador + 1);
        movimientos[contador] = habActual->arriba;
        contador++;
    }
    if (habActual->abajo != -1)
    {
        printf("  %d- Abajo\n", contador + 1);
        movimientos[contador] = habActual->abajo;
        contador++;
    }
    if (habActual->izquierda != -1)
    {
        printf("  %d- Izquierda\n", contador + 1);
        movimientos[contador] = habActual->izquierda;
        contador++;
    }
    if (habActual->derecha != -1)
    {
        printf("  %d- Derecha\n", contador + 1);
        movimientos[contador] = habActual->derecha;
        contador++;
    }
    printf("  %d- Cancelar movimiento\n", contador + 1);

    int opcion;

    printf("Seleccione un movimiento: \n");
    scanf("%d", &opcion);

    while (opcion < 1 || opcion > (contador + 1))
    {
        printf("Ingrese una movimiento valido (1-%d): \n", (contador + 1));
        scanf("%d", &opcion);
    }

    int nuevoId = movimientos[opcion - 1];
    MapNode *nuevoNodo = buscarNodo(habitaciones, nuevoId);
    if (!nuevoNodo)
    {
        printf("Error: la habitacion ID %d no se encontró.\n", nuevoId);
        return habActual;
    }

    printf("Avanzando a la habitacion con %d: %s\n", nuevoId, nuevoNodo->habitacion->nombre);

    return nuevoNodo->habitacion;
}

void iniciarPartida(Map *habitaciones)
{

    system("cls");
    /*### **Estado Actual**

    - Descripción del escenario actual.
    - Lista de ítems disponibles en este escenario (con nombre, peso y valor).
    - Tiempo restante (puedes inicializarlo en 10 o el tiempo que estimes conveniente).
    - Inventario del jugador (ítems recogidos, peso total y puntaje acumulado).
    - Acciones posibles desde este escenario: direcciones disponibles (arriba, abajo, izquierda, derecha).
    */

    if (!habitaciones->root)
    {
        printf("No hay habitaciones cargadas\n");
        return;
    }

    float tiempo = 10;
    int idActual = 1;

    MapNode *nodoActual = buscarNodo(habitaciones, idActual);
    Inventario *inv = createInventario();
    Habitacion *habActual = nodoActual->habitacion;

    int opcion;

    do
    {
        // Dato habitación actual
        printf("Habitación Actual n°%d\n", habActual->id);
        printf("Descripcion: %s\n", habActual->descripcion);
        printf("Ítems disponibles: \n");

        Item *item = habActual->items;
        while (item != NULL)
        {
            printf("  - %s (Puntos: %d, Peso: %d kg)\n", item->nombre, item->puntos, item->peso);
            item = item->next;
        }

        printf("Tiempo restante: %.3f\n", tiempo);
        printf("Inventario: \n");
        imprimirInventario(inv);
        printf("Acciones: ");
        if (habActual->arriba != -1)
            printf("arriba ");
        if (habActual->abajo != -1)
            printf("abajo ");
        if (habActual->izquierda != -1)
            printf("izquierda ");
        if (habActual->derecha != -1)
            printf("derecha ");
        printf("\n-----------------------------\n");

        // Menú de opciones
        printf("Opciones del jugador\n");
        printf("1. Recoger Ítem(s)\n");
        printf("2. Descartar Ítem(s)\n");
        printf("3. Avanzar en una direccion\n");
        printf("4. Reinciar partida\n");
        printf("5. Salir del juego\n");

        printf("Seleccione una opción: \n");

        scanf("%d", &opcion);

        while (opcion < 1 || opcion > 5)
        {
            printf("Ingrese una opción válida (1-5)\n");
            scanf("%d", &opcion);
        }

        switch (opcion)
        {
            // Recoger ítem(s)
        case 1:
            system("cls");
            if (habActual->items == NULL)
            {
                printf("No hay ítems para recoger en esta habitación\n");
                Sleep(100);
                break;
            }
            recogerItems(habActual, inv);
            tiempo -= 1;
            break;

            // Descartar ítem(s)
        case 2:
            system("cls");
            if (inv->items == NULL)
            {
                printf("Tu inventario se encuentra vacio\n");
                Sleep(100);
                break;
            }
            descartarItems(habActual, inv);
            tiempo -= 1;
            break;

            // Avanzar en una dirección
        case 3:
            system("cls");
            habActual = avanzar(habitaciones, habActual);
            tiempo -= ceilf((float)((inv->pesoTotal + 1) / 10));
            system("cls");
            break;

            // Reiniciar Partida
        case 4:
            iniciarPartida(habitaciones);
            return;

            // Salida del programa
        case 5:
            printf("Volviendo al menú principal\n");
            return;

        default:
            break;
        }

    } while (opcion != 5 && tiempo >= 0 && habActual->esFinal != 1);
    free(nodoActual);

    if (habActual->esFinal == 1)
    {
        printf("Haz completado el laberinto\n");
        imprimirInventario(inv);
        return;
    }

    if (tiempo <= 0)
    {
        printf("Te has quedado sin tiempo :c\n");
        return;
    }
}
