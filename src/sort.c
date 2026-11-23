#include "sort.h"
#include <string.h>

// Меняет местами два целых числа.
void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

// Разбиение по схеме Ломуто. Опорный элемент - средний.
// Возвращает индекс, на котором опорный элемент встал окончательно.
int partition(Node nodes[], int order[], int low, int high) {
  int middle = low + (high - low) / 2;
  swap(&order[middle], &order[high]); // прячем опорный в конец
  char *pivotKey = nodes[order[high]].key;

  int i = low - 1; // граница области "меньше опорного"
  for (int j = low; j < high; j++) {
    if (strcmp(nodes[order[j]].key, pivotKey) < 0) {
      i++;
      swap(&order[i], &order[j]);
    }
  }
  swap(&order[i + 1], &order[high]); // опорный - на своё место
  return i + 1;
}

void quickSort(Node nodes[], int order[], int low, int high) {
  if (low < high) {
    int p = partition(nodes, order, low, high);
    quickSort(nodes, order, low, p - 1);
    quickSort(nodes, order, p + 1, high);
  }
}
