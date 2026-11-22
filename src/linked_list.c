#include "linked_list.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>

// ---------- Вспомогательные (приватные) функции ----------

// Приводит ячейку к состоянию "свободна".
static void clearNode(Node *node) {
  node->key[0] = '\0';
  node->value[0] = '\0';
  node->next = NO_NODE;
  node->prev = NO_NODE;
  node->used = false;
}

// Находит свободную ячейку массива, записывает в неё ключ и значение
// и возвращает её индекс. В список узел НЕ включается.
static int createNode(LinkedList *list, const char *key, const char *value) {
  for (int i = 0; i < MAX_NODES; i++) {
    if (!list->nodes[i].used) {
      Node *node = &list->nodes[i];
      snprintf(node->key, KEY_SIZE, "%s", key);
      snprintf(node->value, VALUE_SIZE, "%s", value);
      node->next = NO_NODE;
      node->prev = NO_NODE;
      node->used = true;
      return i;
    }
  }
  return NO_NODE;
}

// Проверяет, можно ли добавлять ключ: есть ли место и нет ли такого ключа.
static ListStatus checkInsert(const LinkedList *list, const char *key) {
  if (isFull(list)) {
    return LIST_FULL;
  }
  if (findNode(list, key) != NO_NODE) {
    return LIST_DUPLICATE;
  }
  return LIST_OK;
}

// Вставляет узел newIndex в кольцо прямо перед узлом position.
static void linkBefore(LinkedList *list, int newIndex, int position) {
  int before = list->nodes[position].prev;
  list->nodes[newIndex].next = position;
  list->nodes[newIndex].prev = before;
  list->nodes[before].next = newIndex;
  list->nodes[position].prev = newIndex;
}

// Делает узел newIndex единственным узлом кольца (он ссылается сам на себя).
static void linkFirst(LinkedList *list, int newIndex) {
  list->nodes[newIndex].next = newIndex;
  list->nodes[newIndex].prev = newIndex;
  list->head = newIndex;
}

// ---------- Базовые операции ----------

void createList(LinkedList *list) {
  list->head = NO_NODE;
  list->count = 0;
  for (int i = 0; i < MAX_NODES; i++) {
    clearNode(&list->nodes[i]);
  }
}

bool isEmpty(const LinkedList *list) {
  return list->count == 0 || list->head == NO_NODE;
}

bool isFull(const LinkedList *list) { return list->count >= MAX_NODES; }

int countList(const LinkedList *list) { return list->count; }

ListStatus appendNode(LinkedList *list, const char *key, const char *value) {
  ListStatus status = checkInsert(list, key);
  if (status != LIST_OK) {
    return status;
  }
  int index = createNode(list, key, value);
  if (isEmpty(list)) {
    linkFirst(list, index);
  } else {
    // "Перед головой" в кольце - это и есть конец списка.
    linkBefore(list, index, list->head);
  }
  list->count++;
  return LIST_OK;
}

ListStatus deleteNode(LinkedList *list, const char *key) {
  if (isEmpty(list)) {
    return LIST_EMPTY;
  }
  int index = findNode(list, key);
  if (index == NO_NODE) {
    return LIST_NOT_FOUND;
  }
  if (list->count == 1) {
    list->head = NO_NODE;
  } else {
    int before = list->nodes[index].prev;
    int after = list->nodes[index].next;
    list->nodes[before].next = after;
    list->nodes[after].prev = before;
    if (list->head == index) {
      list->head = after;
    }
  }
  clearNode(&list->nodes[index]);
  list->count--;
  return LIST_OK;
}

void printInternal(const LinkedList *list) {
  printf("head = %d, count = %d, вместимость = %d\n", list->head, list->count,
         MAX_NODES);
  printf("idx  занят  prev  next  ключ / значение\n");
  for (int i = 0; i < MAX_NODES; i++) {
    const Node *node = &list->nodes[i];
    if (node->used) {
      printf("%3d  да    %4d  %4d  \"%s\" / \"%s\"\n", i, node->prev,
             node->next, node->key, node->value);
    } else {
      printf("%3d  нет      -     -  (свободна)\n", i);
    }
  }
}

// ---------- Дополнительные операции ----------

void printList(const LinkedList *list) {
  if (isEmpty(list)) {
    printf("Список пуст.\n");
    return;
  }
  int current = list->head;
  do {
    printf("[ %s : %s ] <-> ", list->nodes[current].key,
           list->nodes[current].value);
    current = list->nodes[current].next;
  } while (current != list->head);
  printf("(снова голова)\n");
}

int findNode(const LinkedList *list, const char *key) {
  if (isEmpty(list)) {
    return NO_NODE;
  }
  int current = list->head;
  do {
    if (strcmp(list->nodes[current].key, key) == 0) {
      return current;
    }
    current = list->nodes[current].next;
  } while (current != list->head);
  return NO_NODE;
}

const char *findValue(const LinkedList *list, const char *key) {
  int index = findNode(list, key);
  if (index == NO_NODE) {
    return NULL;
  }
  return list->nodes[index].value;
}

bool isSorted(const LinkedList *list) {
  if (list->count < 2) {
    return true;
  }
  int current = list->head;
  while (list->nodes[current].next != list->head) {
    int next = list->nodes[current].next;
    if (strcmp(list->nodes[current].key, list->nodes[next].key) > 0) {
      return false;
    }
    current = next;
  }
  return true;
}

void sortList(LinkedList *list) {
  if (list->count < 2) {
    return;
  }
  // 1. Выписываем индексы узлов в порядке обхода кольца.
  int order[MAX_NODES];
  int n = 0;
  int current = list->head;
  do {
    order[n++] = current;
    current = list->nodes[current].next;
  } while (current != list->head);

  // 2. Сортируем массив индексов по ключам.
  quickSort(list->nodes, order, 0, n - 1);

  // 3. Перевязываем next/prev в новом порядке (данные не копируются).
  for (int k = 0; k < n; k++) {
    int node = order[k];
    list->nodes[node].next = order[(k + 1) % n];
    list->nodes[node].prev = order[(k + n - 1) % n];
  }
  list->head = order[0];
}

// Список должен быть уже отсортирован (это проверяет вызывающий код).
ListStatus insertSorted(LinkedList *list, const char *key, const char *value) {
  ListStatus status = checkInsert(list, key);
  if (status != LIST_OK) {
    return status;
  }
  int index = createNode(list, key, value);
  if (isEmpty(list)) {
    linkFirst(list, index);
  } else {
    // Ищем первый узел, ключ которого БОЛЬШЕ нового.
    int position = list->head;
    bool found = false;
    do {
      if (strcmp(list->nodes[position].key, key) > 0) {
        found = true;
        break;
      }
      position = list->nodes[position].next;
    } while (position != list->head);

    // Если такого нет, position вернулся к голове: вставка "перед головой"
    // означает добавление в конец. Если он есть, вставляем перед ним.
    linkBefore(list, index, position);
    if (found && position == list->head) {
      list->head = index; // новый ключ меньше всех: он становится головой
    }
  }
  list->count++;
  return LIST_OK;
}
