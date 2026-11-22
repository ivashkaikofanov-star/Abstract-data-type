#ifndef NODE_H
#define NODE_H
#include <stdbool.h>

// Размеры буферов включают завершающий символ '\0'.
// Значит, ключ может быть длиной до 31 символа, значение - до 63.
#define KEY_SIZE 32
#define VALUE_SIZE 64

typedef struct Node {
  char key[KEY_SIZE];     // строковый ключ
  char value[VALUE_SIZE]; // строковое значение
  int next;               // индекс следующего узла в массиве
  int prev;               // индекс предыдущего узла в массиве
  bool used;              // true - ячейка занята, false - свободна
} Node;

#endif // !NODE_H
