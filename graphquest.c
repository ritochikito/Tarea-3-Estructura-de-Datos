// Aqui van la definicion de las funcioness
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graphquest.h"

typedef struct Node
{
    Node *prev;
    void *data;
    Node *next;
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
    Item *next;
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