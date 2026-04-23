#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_KEY_LEN 7

typedef struct Node // структура узла 
{
    char key[MAX_KEY_LEN];
    double value;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

int height(Node* root) // поиск высоты если нулл то 0
{
    return root ? root->height : 0;
}

int bfactor(Node* root) // вычисляем сбалансированость 
{
    return height(root->right) - height(root->left);
}

void fixheight(Node* root) // пересчет высоты 
{
    int hl = height(root->left);
    int hr = height(root->right);
    root->height = (hl > hr ? hl : hr) + 1;
}

Node* rotateright(Node* root) // правый поворот 
{
    Node* q = root->left;
    root->left = q->right;
    q->right = root;
    fixheight(root);
    fixheight(q);
    return q;
}

Node* rotateleft(Node* q) // левый 
{
    Node* root = q->right;
    q->right = root->left;
    root->left = q;
    fixheight(q);
    fixheight(root);
    return root;
}

Node* balance(Node* root) // балансировка 
{
    fixheight(root);
    if (bfactor(root) == 2)
    {
        if (bfactor(root->right) < 0)// если правое выше то правый поворот 
        {
            root->right = rotateright(root->right);
        }
        return rotateleft(root); // левый поворот 
    }
    if (bfactor(root) == -2)
    {
        if (bfactor(root->left) > 0)// если левое выше то левый 
        {
            root->left = rotateleft(root->left);
        }
        return rotateright(root); // правый поворот
    }
    return root;
}

Node* createNode(const char* key, double value) // просто создание узла 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    strncpy(node->key, key, MAX_KEY_LEN);
    node->key[MAX_KEY_LEN - 1] = '\0';
    node->value = value;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* insert(Node* root, const char* key, double value, int* inserted)
{
    if (root == NULL) // если нулл то можно вставлять 
    {
        *inserted = 1;
        return createNode(key, value);
    }

    int cmp = strcmp(key, root->key);// сравнение ключей 
    if (cmp < 0) // идем влево 
    {
        root->left = insert(root->left, key, value, inserted);
    }
    else if (cmp > 0)// вправо 
    {
        root->right = insert(root->right, key, value, inserted);
    }
    else // дубликат 
    {
        *inserted = 0;
        return root;
    }
    return balance(root);
}

Node* findmin(Node* root) // поиск с минимальным ключом 
{
    return root->left ? findmin(root->left) : root;
}

Node* removemin(Node* root) // удаление узла с минимальным ключом 
{
    if (root->left == NULL)// если нет левого потомка то минимум 
    {
        return root->right;
    }
    root->left = removemin(root->left);
    return balance(root);
}

Node* removeKey(Node* root, const char* key, int* deleted)
{
    if (root == NULL)
    {
        *deleted = 0;
        return NULL;
    }

    int cmp = strcmp(key, root->key);
    if (cmp < 0)
    {
        root->left = removeKey(root->left, key, deleted);
    }
    else if (cmp > 0)
    {
        root->right = removeKey(root->right, key, deleted);
    }
    else
    {
        *deleted = 1;
        Node* q = root->left;
        Node* r = root->right;
        free(root);

        if (r == NULL)  // если нет правого поддерева то оставляем левое 
        {
            return q;
        }

        Node* min = findmin(r); // минимальный узел в правом поддереве
        min->right = removemin(r);// удаляем его 
        min->left = q; // левое min старое левое исходного 
        return balance(min);
    }
    return balance(root);
}

int search(Node* root, const char* key, double* value) // поиск в дереве 
{
    if (root == NULL)
    {
        return 0;
    }
    int cmp = strcmp(key, root->key);
    if (cmp == 0)
    {
        *value = root->value;
        return 1;
    }
    else if (cmp < 0)
    {
        return search(root->left, key, value);
    }
    else
    {
        return search(root->right, key, value);
    }
}

void printTreeRec(FILE* out, Node* root, int space) // вывод в дерева 
{
    if (root == NULL)
    {
        return;
    }
    space += 4;
    printTreeRec(out, root->right, space);
    fprintf(out, "\n");
    for (int i = 4; i < space; i++)
    {
        fprintf(out, " ");
    }
    fprintf(out, "%s:%f (h=%d)\n", root->key, root->value, root->height);
    printTreeRec(out, root->left, space);
}

void printTree(FILE* out, Node* root) // вывод дерева в файл
{
    fprintf(out, "Tree:\n");
    if (root == NULL)
    {
        fprintf(out, "(empty)\n\n");
        return;
    }
    printTreeRec(out, root, 0);
    fprintf(out, "\n");
}
void freeTree(Node* root)// удалние 
{
    if (root == NULL)
    {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
int main()
{
    FILE* in = fopen("in.txt", "r");
    if (in == NULL)
    {
        fprintf(stderr, "No open in file\n");
        return 1;
    }
    FILE* out = fopen("out.txt", "w");
    if (out == NULL)
    {
        fprintf(stderr, "No open out file\n");
        fclose(in);
        return 1;
    }
    Node* root = NULL;
    char line[256];
    while (fgets(line, sizeof(line), in) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0)
        {
            continue;
        }
        fprintf(out, "> %s\n", line);
        int op;
        char key[MAX_KEY_LEN] = {0};
        double value;
        char* token = strtok(line, " \t"); // разбиваем строку по пробелам
        if (token == NULL)
        {
            fprintf(out, "Error: empty command\n\n");
            continue;
        }
        op = atoi(token);
        switch (op)
        {
            case 1: // добавление
                token = strtok(NULL, " \t");
                if (token == NULL)
                {
                    fprintf(out, "Error: missing key\n\n");
                    break;
                }
                strncpy(key, token, MAX_KEY_LEN - 1);
                key[MAX_KEY_LEN - 1] = '\0';

                token = strtok(NULL, " \t");
                if (token == NULL)
                {
                    fprintf(out, "Error: missing value\n\n");
                    break;
                }
                value = atof(token);
                {
                    int inserted = 0;
                    root = insert(root, key, value, &inserted);
                    if (inserted)
                    {
                        fprintf(out, "'%s' inserted (value=%.3f)\n\n", key, value);
                    }
                    else
                    {
                        fprintf(out, "Error: key '%s' already exists\n\n", key);
                    }
                }
                break;
            case 2: // удаление
                token = strtok(NULL, " \t");
                if (token == NULL)
                {
                    fprintf(out, "Error: missing key\n\n");
                    break;
                }
                strncpy(key, token, MAX_KEY_LEN - 1);
                key[MAX_KEY_LEN - 1] = '\0';

                {
                    int deleted = 0;
                    root = removeKey(root, key, &deleted);
                    if (deleted)
                    {
                        fprintf(out, "Node '%s' deleted\n\n", key);
                    }
                    else
                    {
                        fprintf(out, "Error: key '%s' not found\n\n", key);
                    }
                }
                break;

            case 3:
                printTree(out, root);
                break;

            case 4:// поиск 
                token = strtok(NULL, " \t");
                if (token == NULL)
                {
                    fprintf(out, "Error: missing key\n\n");
                    break;
                }
                strncpy(key, token, MAX_KEY_LEN - 1);
                key[MAX_KEY_LEN - 1] = '\0';
                {
                    double foundValue;
                    if (search(root, key, &foundValue))
                    {
                        fprintf(out, "Found: key='%s', value=%.3f\n\n", key, foundValue);
                    }
                    else
                    {
                        fprintf(out, "Key '%s' not found\n\n", key);
                    }
                }
                break;

            default:
                fprintf(out, "Error: unknown operation %d\n\n", op);
        }
    }
    freeTree(root);
    fclose(in);
    fclose(out);
    printf("Res save out.txt\n");
    return 0;
}
