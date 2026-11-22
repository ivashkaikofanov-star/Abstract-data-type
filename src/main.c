#include "interactive.h"
#include "linked_list.h"

int main(void) {
  LinkedList list;
  createList(&list);
  runInteractive(&list);
  return 0;
}
