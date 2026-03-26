#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <clocale>
#include <windows.h>

#define MIN_ELEMENTS 11
typedef struct {
    int key;
    int data;
} TableElement;
void bubbleSort(TableElement arr[], int n);
int binarySearch(TableElement arr[], int n, int key);
void printTable(TableElement arr[], int n);
void generateSortedTable(TableElement arr[], int n);
void generateReverseTable(TableElement arr[], int n);
void generateRandomTable(TableElement arr[], int n);
void bubbleSort(TableElement arr[], int n) 
{
    for (int i = 0; i < n - 1; i++) 
    {
        int fl = 0;
        for (int j = 0; j < n - i - 1; j++) 
        {
            if (arr[j].key > arr[j + 1].key) 
            {
                TableElement temp;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                fl++;
            }
        }
        if (!fl)
            break;
    }
}

int binarySearch(TableElement arr[], int n, int key) 
{
    int left = 0;
    int right = n - 1;
    while (left <= right) 
    {
        int mid = left + (right - left) / 2;
        if (arr[mid].key == key) 
        {
            return mid;
        }
        if (arr[mid].key < key) 
        {
            left = mid + 1;
        }
        else 
        {
            right = mid - 1;
        }
    }

    return -1;
}

void printTable(TableElement arr[], int n)
{
    printf("%-10s %-10s\n", "Ключ", "Данные");
    for (int i = 0; i < n; i++) 
    {
        printf("%-10d %-10d\n", arr[i].key, arr[i].data);
    }
    printf("\n");
}

void generateSortedTable(TableElement arr[], int n)
{
    for (int i = 0; i < n; i++) 
    {
        arr[i].key = i * 10 + 5;
        arr[i].data = arr[i].key * 2;
    }
}

void generateReverseTable(TableElement arr[], int n) 
{
    for (int i = 0; i < n; i++) 
    {
        arr[i].key = (n - i) * 10;
        arr[i].data = arr[i].key * 2;
    }
}

void generateRandomTable(TableElement arr[], int n) 
{
    for (int i = 0; i < n; i++) {
        arr[i].key = rand() % 100;
        arr[i].data = arr[i].key * 2;
    }
}

void testCase(int caseNum) {
    TableElement table[MIN_ELEMENTS];
    int i, key, pos;
    switch (caseNum) {
    case 1:
        printf("Элементы упорядочены по возрастанию\n");
        generateSortedTable(table, MIN_ELEMENTS);
        break;
    case 2:
        printf("Элементы в обратном порядке (по убыванию)\n");
        generateReverseTable(table, MIN_ELEMENTS);
        break;
    case 3:
        printf("Элементы не упорядочены (случайные)\n");
        generateRandomTable(table, MIN_ELEMENTS);
        break;
    }

    printf("\nНачальное состояние :\n");
    printTable(table, MIN_ELEMENTS);
    bubbleSort(table, MIN_ELEMENTS);
    printf("После сортировки:\n");
    printTable(table, MIN_ELEMENTS);
    printf("Бинарный поиск (введите -1 для выхода):\n");
    while (1)
    {
        printf("Введите ключ: ");
        scanf("%d", &key);
        if (key == -1) break;
        pos = binarySearch(table, MIN_ELEMENTS, key);
        if (pos != -1) 
        {
            printf("позиция = %d, ключ = %d, значение = %d\n", pos, table[pos].key, table[pos].data);
        }
        else 
        {
            printf("Не найдено\n");
        }
    }
}

int main() 
{
    int choice;
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));
    while (1) 
    {
        printf("1. Тест 1 (упорядоченные элементы)\n");
        printf("2. Тест 2 (обратный порядок)\n");
        printf("3. Тест 3 (случайные элементы)\n");
        printf("4. Выход\n");
        printf("Тест: ");
        scanf("%d", &choice);
        if (choice == 4) break;
        if (choice >= 1 && choice <= 3)
        {
            testCase(choice);
        }
        else
        {
            printf("Неизвестная команда\n");
        }
    }
    return 0;
}
