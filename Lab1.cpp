#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
typedef struct Node 
{
    int data;
    struct Node* first;
    struct Node* other;
} Node;
Node* Create_Node(int val);
Node* Add_Node(Node* root, int pval, int cval);
void Print_Tree(Node* root, int tab);
Node* Find_Parent(Node* root, Node* child);
Node* Find_Node(Node* root, int val);
Node* Del_Node(Node* root, int val);
void Find_Min_Deep(Node* root, int depp, int* min_deep, int* min_val);
void Free_Tree(Node* root);
Node* Create_Node(int val)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        printf("ОШИБКА ВЫДЕЛЕНИЯ ПАМЯТИ\n");
        exit(1);
    }
    new_node->data = val;
    new_node->first = NULL;
    new_node->other = NULL;
    return new_node;
}
Node* Add_Node(Node* root, int pval, int cval)
{
    if (root == NULL) {
        printf("Ошибка дерево не существует\n");
        return NULL;
    }

    if (root->data == pval)
    {
        Node* new_node = Create_Node(cval);
        if (root->first == NULL)
        {
            root->first = new_node;
        }
        else
        {
            Node* temp = root->first;
            while (temp->other != NULL)
            {
                temp = temp->other;
            }
            temp->other = new_node;
        }
        return root;
    }
    Node* found = NULL;
    if (root->first != NULL)
    {
        found = Add_Node(root->first, pval, cval);
        if (found != NULL) 
            return root;
    }
    if (root->other != NULL)
    {
        found = Add_Node(root->other, pval, cval);
        if (found != NULL) 
            return root;
    }
    return NULL;
}
void Print_Tree(Node* root, int tab)
{
    if (root == NULL)
    {
        return;
    }

    for (int i = 0; i < tab; i++)
    {
        printf("    ");
    }
    printf("%d\n", root->data);

    Print_Tree(root->first, tab + 1);
    Print_Tree(root->other, tab);
}
Node* Find_Parent(Node* root, Node* child)
{
    if (root == NULL || child == NULL)
        return NULL;
    Node* temp = root->first;
    while (temp != NULL)
    {
        if (temp == child)
            return root;
        temp = temp->other;
    }
    Node* parent = Find_Parent(root->first, child);
    if (parent != NULL)
        return parent;

    return Find_Parent(root->other, child);
}
Node* Find_Node(Node* root, int val)
{
    if (root == NULL)
        return NULL;

    if (root->data == val)
        return root;

    Node* found = Find_Node(root->first, val);
    if (found != NULL)
        return found;

    return Find_Node(root->other, val);
}
void Free_Tree(Node* root)
{
    if (root == NULL)
    {
        return;
    }

    Free_Tree(root->first);
    Free_Tree(root->other);
    free(root);
}
Node* Del_Node(Node* root, int val)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (root->data == val)
    {
        Free_Tree(root);
        return NULL;
    }
    Node* del = Find_Node(root, val);
    if (del == NULL)
    {
        printf("Узел со значением %d не найден в дереве\n", val);
        return root;
    }
    Node* parent = Find_Parent(root, del);
    if (parent != NULL)
    {
        Node* prev = NULL;
        Node* curr = parent->first;

        while (curr != del)
        {
            prev = curr;
            curr = curr->other;
        }
        if (prev == NULL)
        {
            parent->first = curr->other;
        }
        else
        {
            prev->other = curr->other;
        }

        Free_Tree(curr);
    }
    return root;
}
void Find_Min_Deep(Node* root, int depp, int* min_deep, int* min_val)
{
    if (root == NULL)
    {
        return;
    }
    if (root->first == NULL)
    {
        if (depp < *min_deep)
        {
            *min_deep = depp;
            *min_val = root->data;
        }
        return;
    }
    Find_Min_Deep(root->first, depp + 1, min_deep, min_val);
    Find_Min_Deep(root->other, depp, min_deep, min_val);
}
int main()
{
    setlocale(LC_ALL, "Russian");
    int rootVal;
    printf("Введите значение корня: ");
    scanf("%d", &rootVal);
    Node* root = Create_Node(rootVal);
    printf("Корень создан со значением %d\n\n", rootVal);
    printf("1 - Добавить узел\n");
    printf("2 - Удалить узел\n");
    printf("3 - Вывести дерево\n");
    printf("4 - Найти лист с минимальной глубиной\n");
    printf("5 - Завершить работу\n\n");
    while (1)
    {
        int num;
        printf("Выберите опцию: ");
        scanf("%d", &num);
        printf("\n");
        switch (num)
        {
        case 1:
        {
            int pval, cval;
            printf("Значение родителя: ");
            scanf("%d", &pval);
            printf("Значение потомка: ");
            scanf("%d", &cval);
            Node* result = Add_Node(root, pval, cval);
            if (result != NULL)
            {
                printf("Добавлен узел %d как потомок узла %d\n\n", cval, pval);
            }
            else
            {
                printf("Родитель со значением %d не найден в дереве\n\n", pval);
            }
            break;
        }
        case 2:
        {
            int val;
            printf("Значение для удаления: ");
            scanf("%d", &val);
            Node* to_del = Find_Node(root, val);
            if (to_del == NULL)
            {
                printf("Узел со значением %d не найден в дереве\n\n", val);
                break;
            }
            
            Node* result = Del_Node(root, val);
            if (result == NULL)
            {
                printf("Корневой узел со значением %d удален. Дерево удалено. Работа завершена\n\n", val);
                Free_Tree(root);
                return 0;
            }
            else
            {
                printf("Удален узел %d\n\n", val);
            }
            break;
        }
        case 3:
        {
            printf("Дерево:\n");
            Print_Tree(root, 0);
            printf("\n");
            break;
        }
        case 4:
        {
            int mi = 99999;
            int mi_val = 0;
            Find_Min_Deep(root, 1, &mi, &mi_val);
            printf("Минимальная глубина листа: %d\n", mi);
            printf("Значение на минимальной глубине: %d\n\n", mi_val);
            break;
        }
        case 5:
        {
            printf("Завершение работы\n");
            Free_Tree(root);
            return 0;
        }
        default:
        {
            printf("Опция не найдена\n\n");
            break;
        }
        }
    }
    return 0;
}
