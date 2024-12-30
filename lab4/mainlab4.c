#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Структура для хранения информации о файле
typedef struct FileInfo {
    char name[256]; // Имя файла
    time_t creationTime; // Время создания файла
} FileInfo;

// Структура для узла списка
typedef struct Node {
    FileInfo data; // Данные файла
    struct Node *next; // Указатель на следующий узел
} Node;

// Функция для создания нового узла (возвращает указатель на новый узел)
Node *createNode(FileInfo file) {
    Node *newNode = (Node *) malloc(sizeof(Node)); // Выделяем память для нового узла
    newNode->data = file;
    // Заполняем данные узла (Мы копируем данные из переданного аргумента file (типа FileInfo) в поле data новой структуры Node.)
    newNode->next = NULL; // Устанавливаем указатель на следующий узел в NULL, так как он еще не связан с другими
    return newNode; // Возвращаем указатель на новый узел
}

// Функция для вставки узла в начало списка
void insertNode(Node **head, FileInfo file) {
    Node *newNode = createNode(file); // Создаем новый узел
    newNode->next = *head; // Новый узел указывает на текущую голову списка
    *head = newNode; // Голова списка становится новым узлом
}

// Функция для поиска узла по имени файла
Node *findNode(Node *head, const char *name) {
    // имя файла, указатель на первый узел
    while (head != NULL) {
        // Проходим по всему списку
        if (strcmp(head->data.name, name) == 0) {
            // Сравниваем data.name с искомым именем
            return head; // Возвращаем указатель на узел
        }
        head = head->next; // Переходим к следующему узлу
    }
    return NULL; // Если узел не найден, возвращаем NULL
}

// Функция для удаления узла по имени файла
void deleteNode(Node **head, const char *name) {
    Node *temp = *head; // Временный указатель для прохождения по списку
    Node *prev = NULL; // Указатель на предыдущий узел

    // Ищем узел с нужным именем
    while (temp != NULL && strcmp(temp->data.name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // Если узел не найден
    if (temp == NULL) {
        printf("Element with name '%s' not found.\n", name);
        return;
    }

    // Если узел находится в начале списка
    if (prev == NULL) {
        *head = temp->next;
    } else {
        // Если узел находится не в начале списка
        prev->next = temp->next; //перескакивакм
    }

    free(temp); // Освобождаем память для удаляемого узла
    printf("Element '%s' removed from the list.\n", name);
}

// Функция для печати списка файлов
void printList(Node *head) {
    while (head != NULL) {
        // Проходим по всему списку
        printf("%s\n", head->data.name); // Печатаем имя файла
        head = head->next; // Переходим к следующему узлу
    }
}

// Функция для печати информации о файле
void printFileInfo(Node *node) {
    if (node == NULL) {
        // Если узел не найден
        printf("Element not found.\n");
        return;
    }
    printf("Name: %s | Creation time: %ld\n", node->data.name, node->data.creationTime);
    // Печатаем имя файла и время создания
}

// Функция для сканирования каталога и обновления списка файлов
void scanDirectory(Node **head, const char *dirPath) {
    DIR *dir = opendir(dirPath); // Открываем указанный каталог (тип данных и указатель для работы с opendir, read)
    if (dir == NULL) {
        // Если не удалось открыть каталог
        perror("Error opening directory");
        return;
    }

    struct dirent *entry; // Структура для хранения информации о файле/каталоге (Имя)
    struct stat fileStat; // Структура для получения информации о файле (Наше время)
    char filePath[512]; // Массив для хранения полного пути к файлу


    // Очистка текущего списка перед его пересозданием
    while (*head != NULL) {
        Node *temp = *head; // Создаем temp который будет указывать на текущий узел, на head (который надо удалить)
        *head = (*head)->next; // Теперь голова указывает на следующий
        free(temp);
    }

    // Чтение содержимого каталога
    while ((entry = readdir(dir)) != NULL) {
        // Читаем и информацию сохраняем в entry
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);
        // Формируем полный путь к файлу: путь к каталогу (dirPath + имя файла) "./documents/file.txt".

        // Получаем информацию о файле
        if (stat(filePath, &fileStat) == 0) {
            // 0 - успешное выполнение
            FileInfo file; // Создание локальной переменной file типа FileInfo
            strncpy(file.name, entry->d_name, sizeof(file.name) - 1);
            // Копируем имя файла (функция копирует строку entry->d_name (имя файла) в поле file.name)
            file.creationTime = fileStat.st_ctime; // Получаем время создания файла

            insertNode(head, file); // Вставляем узел в список
        }
    }

    closedir(dir); // Закрываем каталог
    printf("List updated.\n");
}

// Функция для удаления файла из файловой системы
void deleteFileFromSystem(const char *path) {
    if (remove(path) == 0) {
        printf("File '%s' deleted from the system.\n", path);
    } else {
        perror("Error deleting file");
    }
}

int main() {
    Node *fileList = NULL; // Указатель на голову списка (node - моя структура)
    char dirPath[256] = "."; // Путь к каталогу (по умолчанию текущий каталог)
    int choice; // Переменная для хранения выбора пользователя

    while (1) {
        printf("\n");
        printf("1. Scan directory and update list\n");
        printf("2. Show list of files and folders\n");
        printf("3. Show object information\n");
        printf("4. Delete a file\n");
        printf("5. Exit\n");

        printf("Your choice: ");
        scanf("%d", &choice); // Считываем выбор пользователя
        getchar(); // Чистим буфер ввода

        if (choice == 5) {
            // Если пользователь выбрал выход
            break;
        }

        switch (choice) {
            case 1: // Сканирование каталога и обновление списка
                printf("Enter directory path: ");
                fgets(dirPath, sizeof(dirPath), stdin); // Считываем путь к каталогу и сохраняем в dirPath
                dirPath[strcspn(dirPath, "\n")] = '\0';
            // В строке dirPath не будет лишнего символа новой строки в конце.
                scanDirectory(&fileList, dirPath);
            // Указатель на голову связного списка, чтобы мы смогли изменить список к файлу, наш путь
                break;

            case 2: // Показ списка файлов и папок
                printList(fileList); // Печатаем список, в main
                break;

            case 3: {
                // Показ информации об объекте
                char name[256];
                printf("Enter object name: ");
                fgets(name, sizeof(name), stdin); // Считываем имя объекта
                name[strcspn(name, "\n")] = '\0'; // Удаляем символ новой строки
                Node *node = findNode(fileList, name); // Ищем узел по имени
                printFileInfo(node); // Печатаем информацию об объекте
                break;
            }

            case 4: {
                // Удаление файла
                char name[256];
                printf("Enter file name to delete: ");
                fgets(name, sizeof(name), stdin); // Считываем имя файла
                name[strcspn(name, "\n")] = '\0'; // Удаляем символ новой строки

                Node *node = findNode(fileList, name); // Ищем узел по имени

                if (node != NULL) {
                    // Формируем полный путь к файлу
                    char filePath[512];
                    snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, name);

                    // Удаляем файл из файловой системы
                    deleteFileFromSystem(filePath);


                    // Удаляем файл из списка
                    deleteNode(&fileList, name);
                } else {
                    printf("File '%s' not found in the list.\n", name);
                }
                break;
            }

            default: // Неверный выбор
                printf("Invalid choice. Try again.\n");
        }
    }

    // Очистка списка перед выходом
    while (fileList != NULL) {
        Node *temp = fileList; // temp - мы удалим, присваиваем head
        fileList = fileList->next; // Присваиваем head - новый head
        free(temp);
    }

    return 0;
}
