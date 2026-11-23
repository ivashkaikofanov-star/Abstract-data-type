#include "interactive.h"
#include "doubly_circular_list.h"

int main(void) {
  DoublyCircularList list;
  createList(&list);
  runInteractive(&list);
  return 0;
}
// Создать build папку
// .\build\program.exe