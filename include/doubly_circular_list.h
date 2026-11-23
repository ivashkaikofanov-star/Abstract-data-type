#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "node.h"
#include <stdbool.h>

#define MAX_NODES 10 // вместимость списка (размер статического массива)
#define NO_NODE (-1) // "пустой индекс": аналог NULL для индексов

// Результат операций, которые могут не удаться.
typedef enum ListStatus {
  LIST_OK,
  LIST_FULL,
  LIST_EMPTY,
  LIST_DUPLICATE,
  LIST_NOT_FOUND
} ListStatus;

typedef struct DoublyCircularList {
  Node nodes[MAX_NODES]; // статический массив, в котором живут все узлы
  int head;              // индекс головы (или NO_NODE, если список пуст)
  int count;             // сколько узлов сейчас в списке
} DoublyCircularList;

// Базовые операции
void createList(DoublyCircularList *list);
bool isEmpty(DoublyCircularList *list);
bool isFull(DoublyCircularList *list);
int countList(DoublyCircularList *list);
ListStatus appendNode(DoublyCircularList *list, char *key, char *value);
ListStatus deleteNode(DoublyCircularList *list, char *key);
void printInternal(DoublyCircularList *list);

// Дополнительные операции
void printList(DoublyCircularList *list);
void sortList(DoublyCircularList *list);
bool isSorted(DoublyCircularList *list);
ListStatus insertSorted(DoublyCircularList *list, char *key, char *value);
int findNode(DoublyCircularList *list, char *key);
char *findValue(DoublyCircularList *list, char *key);

#endif // !LINKED_LIST_H
