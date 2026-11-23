#ifndef SORT_H
#define SORT_H
#include "node.h"

// Сортирует массив индексов order[low..high] по ключам узлов nodes[].
void quickSort(Node nodes[], int order[], int low, int high);

#endif // !SORT_H
