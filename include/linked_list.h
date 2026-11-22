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

typedef struct LinkedList {
  Node nodes[MAX_NODES]; // статический массив, в котором живут все узлы
  int head;              // индекс головы (или NO_NODE, если список пуст)
  int count;             // сколько узлов сейчас в списке
} LinkedList;

// Базовые операции
void createList(LinkedList *list);
bool isEmpty(const LinkedList *list);
bool isFull(const LinkedList *list);
int countList(const LinkedList *list);
ListStatus appendNode(LinkedList *list, const char *key, const char *value);
ListStatus deleteNode(LinkedList *list, const char *key);
void printInternal(const LinkedList *list);

// Дополнительные операции
void printList(const LinkedList *list);
void sortList(LinkedList *list);
bool isSorted(const LinkedList *list);
ListStatus insertSorted(LinkedList *list, const char *key, const char *value);
int findNode(const LinkedList *list, const char *key);
const char *findValue(const LinkedList *list, const char *key);

#endif // !LINKED_LIST_H
