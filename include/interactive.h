#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include "doubly_circular_list.h"

// Интерактивный режим: в цикле читает команды с параметрами, выполняет их
// и печатает результат. Заканчивается по команде exit.
void runInteractive(DoublyCircularList *list);

#endif // !INTERACTIVE_H
