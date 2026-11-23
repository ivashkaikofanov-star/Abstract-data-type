#ifndef NODE_H
#define NODE_H
#include <stdbool.h>

#define KEY_SIZE 32
#define VALUE_SIZE 64

typedef struct Node {
  char key[KEY_SIZE];     
  char value[VALUE_SIZE]; 
  int next;               
  int prev;               
  bool used;              // true - ячейка занята, false - свободна
} Node;

#endif 
