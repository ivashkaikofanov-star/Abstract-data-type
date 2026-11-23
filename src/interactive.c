#include "interactive.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_SIZE 256

void printHelp(void) {
  printf("Команды:\n");
  printf("  help                   - показать эту справку\n");
  printf("  add <ключ> <значение>  - добавить элемент в конец\n");
  printf("  insert <ключ> <знач.>  - добавить с сохранением порядка\n");
  printf("  delete <ключ>          - удалить элемент по ключу\n");
  printf("  find <ключ>            - найти значение по ключу\n");
  printf("  sort                   - отсортировать по ключу (Quick Sort)\n");
  printf("  print                  - напечатать все элементы\n");
  printf("  internal               - напечатать внутреннее представление\n");
  printf("  empty                  - проверить, пуст ли список\n");
  printf("  count                  - посчитать элементы\n");
  printf("  exit                   - выход\n");
  printf("Ключ - одно слово. Значение может состоять из нескольких слов.\n");
}

void reportStatus(ListStatus status, const char *successMessage) {
  switch (status) {
  case LIST_OK:
    printf("%s\n", successMessage);
    break;
  case LIST_FULL:
    printf("Ошибка: список заполнен (максимум %d элементов).\n", MAX_NODES);
    break;
  case LIST_EMPTY:
    printf("Ошибка: список пуст.\n");
    break;
  case LIST_DUPLICATE:
    printf("Ошибка: элемент с таким ключом уже есть.\n");
    break;
  case LIST_NOT_FOUND:
    printf("Ошибка: элемент с таким ключом не найден.\n");
    break;
  }
}

// Сверяет число введённых параметров с ожидаемым.
bool checkArgs(const char *command, int given, int expected) {
  if (given == expected) {
    return true;
  }

  printf("Ошибка: команда %s принимает параметров: %d, введено: %d. "
         "Справка: help\n",
         command, expected, given);

  return false;
}

// Проверяет, что текст поместится в буфер размером size (вместе с '\0').
bool checkLength(const char *text, int size, const char *name) {
  if (strlen(text) < (size_t)size) {
    return true;
  }

  printf("Ошибка: параметр \"%s\" слишком длинный (максимум %d символов).\n",
         name, size - 1);

  return false;
}

void doAdd(DoublyCircularList *list, char *key, char *value) {
  if (!checkLength(key, KEY_SIZE, "ключ") ||
      !checkLength(value, VALUE_SIZE, "значение")) {
    return;
  }

  ListStatus status = appendNode(list, key, value);
  reportStatus(status, "Элемент добавлен в конец.");

  if (status == LIST_OK) {
    printList(list);
  }
}

void doInsert(DoublyCircularList *list, char *key, char *value) {
  if (!isSorted(list)) {
    printf("Список не отсортирован. Сначала выполните команду sort.\n");
    return;
  }

  if (!checkLength(key, KEY_SIZE, "ключ") ||
      !checkLength(value, VALUE_SIZE, "значение")) {
    return;
  }

  ListStatus status = insertSorted(list, key, value);
  reportStatus(status, "Элемент добавлен с сохранением порядка.");

  if (status == LIST_OK) {
    printList(list);
  }
}

void doDelete(DoublyCircularList *list, char *key) {
  ListStatus status = deleteNode(list, key);
  reportStatus(status, "Элемент удалён.");

  if (status == LIST_OK) {
    printList(list);
  }
}

void doFind(const DoublyCircularList *list, char *key) {
  const char *value = findValue(list, key);

  if (value == NULL) {
    printf("Элемент с ключом \"%s\" не найден.\n", key);
  } else {
    printf("Значение элемента \"%s\": \"%s\"\n", key, value);
  }
}

// Разбирает введённую строку и выполняет команду и возвращает false, если пользователь ввёл exit.
bool executeCommand(DoublyCircularList *list, char *line) {
  char command[LINE_SIZE];
  char arg1[LINE_SIZE];

  char *arg1Ptr = NULL;
  char *arg2 = NULL;

  int position = 0;

  // Получаем первое слово — название команды.
  if (sscanf(line, "%255s", command) != 1) {
    return true;
  }

  // %n запоминает позицию, с которой начинается оставшаяся строка.
  int parsed = sscanf(line, "%*255s %255s %n", arg1, &position);

  if (parsed == 1) {
    arg1Ptr = arg1;

    // Всё, что осталось после первого аргумента, — второй аргумент.
    if (line[position] != '\0') {
      arg2 = line + position;

      // Убираем пробелы и табуляции в конце значения.
      size_t length = strlen(arg2);
      while (length > 0 &&
             (arg2[length - 1] == ' ' || arg2[length - 1] == '\t')) {
        length--;
      }
      arg2[length] = '\0';
      if (length == 0) {
        arg2 = NULL;
      }
    }
  }

  int given = 0;
  if (arg1Ptr != NULL) {
    given++;
  }
  if (arg2 != NULL) {
    given++;
  }
  if (strcmp(command, "exit") == 0) {
    if (checkArgs(command, given, 0)) {
      printf("Выход из программы.\n");
      return false;
    }
  } else if (strcmp(command, "help") == 0) {
    if (checkArgs(command, given, 0)) {
      printHelp();
    }
  } else if (strcmp(command, "add") == 0) {
    if (checkArgs(command, given, 2)) {
      doAdd(list, arg1Ptr, arg2);
    }
  } else if (strcmp(command, "insert") == 0) {
    if (checkArgs(command, given, 2)) {
      doInsert(list, arg1Ptr, arg2);
    }
  } else if (strcmp(command, "delete") == 0) {
    if (checkArgs(command, given, 1)) {
      doDelete(list, arg1Ptr);
    }
  } else if (strcmp(command, "find") == 0) {
    if (checkArgs(command, given, 1)) {
      doFind(list, arg1Ptr);
    }
  } else if (strcmp(command, "sort") == 0) {
    if (checkArgs(command, given, 0)) {
      sortList(list);
      printf("Список отсортирован по ключу (Quick Sort).\n");
      printList(list);
    }
  } else if (strcmp(command, "print") == 0) {
    if (checkArgs(command, given, 0)) {
      printList(list);
    }
  } else if (strcmp(command, "internal") == 0) {
    if (checkArgs(command, given, 0)) {
      printInternal(list);
    }
  } else if (strcmp(command, "empty") == 0) {
    if (checkArgs(command, given, 0)) {
      printf(isEmpty(list) ? "Список пуст.\n" : "Список не пуст.\n");
    }
  } else if (strcmp(command, "count") == 0) {
    if (checkArgs(command, given, 0)) {
      printf("Количество элементов: %d\n", countList(list));
    }
  } else {
    printf("Неизвестная команда: %s. Справка: help\n", command);
  }
  return true;
}

void runInteractive(DoublyCircularList *list) {
    char line[LINE_SIZE];
    bool running = true;
    printf("Интерактивный режим. Введите help для справки.\n");
    while (running) {
        printf("> ");
        if (scanf(" %255[^\n]", line) != 1) {
            printf("\nВвод закончился. Выход из программы.\n");
            break;
        }
        running = executeCommand(list, line);
    }
}

