#include "graphquest.c"
// Aqui van los headers de las funciones

List *createList(); // Crea una lista y la retorna

List *cargarHabitacionesDesdeCSV(const char *nombreArchivo);

void imprimirHabitaciones(List *lista);