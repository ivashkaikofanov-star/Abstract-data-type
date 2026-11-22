#include "interactive.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define LINE_SIZE 256   // размер буфера для одной введённой строки
#define SPACES " \t\r\n" // символы, которыми разделены слова команды

// Результат чтения строки с клавиатуры.
typedef enum ReadResult {
  READ_OK,       // строка прочитана
  READ_END,      // ввод закончился (Ctrl+D в Linux, Ctrl+Z в Windows)
  READ_TOO_LONG  // строка не поместилась в буфер
} ReadResult;

// ---------- Ввод ----------

// Читает одну строку с клавиатуры в буфер line размером size.
static ReadResult readLine(char *line, int size) {
  if (fgets(line, size, stdin) == NULL) {
    return READ_END;
  }
  // Если в строке нет символа '\n' и ввод не закончился,
  // значит, строка длиннее буфера: выбрасываем её остаток.
  if (strchr(line, '\n') == NULL && !feof(stdin)) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    return READ_TOO_LONG;
  }
  return READ_OK;
}

// ---------- Вывод ----------

static void printHelp(void) {
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

// Печатает сообщение об успехе или причину неудачи.
static void reportStatus(ListStatus status, const char *successMessage) {
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

// ---------- Проверки параметров ----------

// Сверяет число введённых параметров с ожидаемым.
static bool checkArgs(const char *command, int given, int expected) {
  if (given == expected) {
    return true;
  }
  printf("Ошибка: команда %s принимает параметров: %d, введено: %d. "
         "Справка: help\n",
         command, expected, given);
  return false;
}

// Проверяет, что текст поместится в буфер размером size (вместе с '\0').
static bool checkLength(const char *text, int size, const char *name) {
  if (strlen(text) < (size_t)size) {
    return true;
  }
  printf("Ошибка: параметр \"%s\" слишком длинный (максимум %d символов).\n",
         name, size - 1);
  return false;
}

// ---------- Выполнение команд ----------

static void doAdd(LinkedList *list, const char *key, const char *value) {
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

static void doInsert(LinkedList *list, const char *key, const char *value) {
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

static void doDelete(LinkedList *list, const char *key) {
  ListStatus status = deleteNode(list, key);
  reportStatus(status, "Элемент удалён.");
  if (status == LIST_OK) {
    printList(list);
  }
}

static void doFind(const LinkedList *list, const char *key) {
  const char *value = findValue(list, key);
  if (value == NULL) {
    printf("Элемент с ключом \"%s\" не найден.\n", key);
  } else {
    printf("Значение элемента \"%s\": \"%s\"\n", key, value);
  }
}

// Разбирает строку на команду и параметры и выполняет её.
// Возвращает false, если пользователь ввёл exit, иначе true.
static bool executeCommand(LinkedList *list, char *line) {
  char *command = strtok(line, SPACES); // первое слово
  if (command == NULL) {
    return true; // пустая строка: ничего не делаем
  }
  char *arg1 = strtok(NULL, SPACES); // второе слово
  char *arg2 = strtok(NULL, "\r\n"); // всё остальное до конца строки
  if (arg2 != NULL) {
    while (*arg2 == ' ' || *arg2 == '\t') {
      arg2++; // пропускаем пробелы перед значением
    }
    size_t length = strlen(arg2);
    while (length > 0 && (arg2[length - 1] == ' ' || arg2[length - 1] == '\t')) {
      length--; // отрезаем пробелы в конце значения
    }
    arg2[length] = '\0';
    if (length == 0) {
      arg2 = NULL; // после обрезки ничего не осталось
    }
  }
  int given = 0; // сколько параметров введено
  if (arg1 != NULL) {
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
      doAdd(list, arg1, arg2);
    }
  } else if (strcmp(command, "insert") == 0) {
    if (checkArgs(command, given, 2)) {
      doInsert(list, arg1, arg2);
    }
  } else if (strcmp(command, "delete") == 0) {
    if (checkArgs(command, given, 1)) {
      doDelete(list, arg1);
    }
  } else if (strcmp(command, "find") == 0) {
    if (checkArgs(command, given, 1)) {
      doFind(list, arg1);
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

// ---------- Главный цикл ----------

void runInteractive(LinkedList *list) {
  char line[LINE_SIZE];
  bool running = true;

  printf("Интерактивный режим. Введите help, чтобы увидеть команды.\n");
  while (running) {
    printf("> ");
    fflush(stdout); // показать приглашение сразу, не дожидаясь перевода строки
    ReadResult result = readLine(line, LINE_SIZE);
    if (result == READ_END) {
      printf("\nВвод закончился. Выход из программы.\n");
      break;
    }
    if (result == READ_TOO_LONG) {
      printf("Ошибка: команда слишком длинная.\n");
      continue;
    }
    running = executeCommand(list, line);
  }
}
