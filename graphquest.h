#include "graphquest.c"
// Función para crear una lista
List *createList();

// Prototipos de funciones para el Map
Map *createMap();
MapNode *createMapNode(int key, Habitacion *hab);
MapNode *insertMapNode(MapNode *root, int key, Habitacion *hab);
void mapInsert(Map *map, int key, Habitacion *hab);

// ============================
// Funciones para parsear CSV
// ============================
// Función que separa una línea CSV en 'expected_fields' campos.
void parseCSVLine(char *line, char *fields[], int expected_fields);

// Función que parsea la cadena de items y retorna una lista enlazada de Item.
Item *parseItems(const char *items_str);

// Función que lee el archivo CSV y carga las habitaciones en el Map.
void loadHabitacionesFromCSV(const char *filename, Map *map);

void imprimirHabitacion(Map *habitaciones, int key);

void iniciarPartida(Map *habitaciones);

MapNode *buscarNodo(Map *habitaciones, int key);