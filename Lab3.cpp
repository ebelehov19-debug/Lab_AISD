#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define MAX_EXPR 1024
#define MAX_LINE 256
typedef enum 
{
    NODE_NUMBER,
    NODE_VARIABLE,
    NODE_OPERATOR
} NodeType;
typedef struct Node 
{
    NodeType type;
    union 
    {
        int number;
        char variable;
        char opt;
    } data;
    struct Node *left;
    struct Node *right;
} Node;
Node* create_number_node(int value) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL)
    {
        return NULL;
    }
    node->type = NODE_NUMBER;
    node->data.number = value;
    node->left = node->right = NULL;
    return node;
}
Node* create_variable_node(char var) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if(node = NULL)
    {
        return NULL;
    }
    node->type = NODE_VARIABLE;
    node->data.variable = var;
    node->left = node->right = NULL;
    return node;
}
Node* create_operator_node(char op, Node* left, Node* right) 
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->type = NODE_OPERATOR;
    node->data.opt = op;
    node->left = left;
    node->right = right;
    return node;
}
void free_tree(Node* root) 
{
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
Node* copy_tree(Node* root) 
{
    if (!root) return NULL;
    if (root->type == NODE_NUMBER)
        return create_number_node(root->data.number);
    if (root->type == NODE_VARIABLE)
        return create_variable_node(root->data.variable);
    return create_operator_node(root->data.opt,
                                copy_tree(root->left),
                                copy_tree(root->right));
}
int is_operator(char c) 
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
int precedence(char op) 
{
    switch (op) 
    {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}
int right_associative(char op) 
{
    return op == '^';
}
Node* build_tree(const char* expr) 
{
    char op_stack[MAX_EXPR]; // стек для операторов
    int op_top = -1;
    Node* node_stack[MAX_EXPR];// стек узлов 
    int node_top = -1;
    int i = 0, len = strlen(expr);
    while (i < len) 
    {
        char c = expr[i];
        if (isspace(c)) 
        { 
            i++; 
            continue;
        }
        if (isdigit(c)) 
        {
            int value = 0;
            while (i < len && isdigit(expr[i])) // собираем длинное число 
            {
                value = value * 10 + (expr[i] - '0');
                i++;
            }
            node_stack[++node_top] = create_number_node(value);
            continue;
        }
        if (isalpha(c)) 
        {
            node_stack[++node_top] = create_variable_node(c);
            i++;
            continue;
        }
        if (c == '(') 
        {
            op_stack[++op_top] = c;
            i++;
            continue;
        }
        if (c == ')') 
        {
            while (op_top >= 0 && op_stack[op_top] != '(') // вытаскиваем все до открывающей 
            {
                char op = op_stack[op_top--];
                Node* right = node_stack[node_top--];
                Node* left = node_stack[node_top--];
                node_stack[++node_top] = create_operator_node(op, left, right);
            }
            if (op_top >= 0) op_top--; // удаляем закрвщую
            i++;
            continue;
        }
        if (is_operator(c)) 
        {
            
            while (op_top >= 0 && op_stack[op_top] != '(' &&
                   (precedence(op_stack[op_top]) > precedence(c) ||
                    (precedence(op_stack[op_top]) == precedence(c) && !right_associative(c)))) // вытаскиваем операторы с большим или равным приоритетом
            {
                char op = op_stack[op_top--];
                Node* right = node_stack[node_top--];
                Node* left = node_stack[node_top--];
                node_stack[++node_top] = create_operator_node(op, left, right);
            }
            op_stack[++op_top] = c;
            i++;
            continue;
        }
        fprintf(stderr, "Неизвестный символ: %c\n", c);
        return NULL;
    }
    while (op_top >= 0)// достаем оставшиеся операторы 
    {
        char op = op_stack[op_top--];
        if (op == '(' || op == ')') continue;
        Node* right = node_stack[node_top--];
        Node* left = node_stack[node_top--];
        node_stack[++node_top] = create_operator_node(op, left, right);
    }
    if (node_top != 0) // если стек не пустой ошибка 
    {
        return NULL;
    }
    return node_stack[0];
}
void flatten_multiplication(Node* node, Node** factors, int* count) 
{
    if (!node) return;
    if (node->type == NODE_OPERATOR && node->data.opt == '*') // если * то рекурсивно обходим 
    {
        flatten_multiplication(node->left, factors, count);
        flatten_multiplication(node->right, factors, count);
    } 
    else 
    {
        factors[(*count)++] = node;
    }
}
Node* simplify_multiplication(Node* node) 
{
    if (!node) 
        return NULL;
        
    if (node->type != NODE_OPERATOR || node->data.opt != '*')
    {
        node->left = simplify_multiplication(node->left);
        node->right = simplify_multiplication(node->right);
        return node;
    } 
    node->left = simplify_multiplication(node->left); // рекурсивно упращаем поддеревь я
    node->right = simplify_multiplication(node->right);
    Node* factors[MAX_EXPR];
    int factor_count = 0;
    flatten_multiplication(node, factors, &factor_count);
    int const_prod = 1;
    Node* other_factors[MAX_EXPR];
    int cnt = 0;
    
    for (int i = 0; i < factor_count; i++) 
    {
        if (factors[i]->type == NODE_NUMBER) 
        {
            const_prod *= factors[i]->data.number; // умножаем числа 
        } 
        else 
        {
            other_factors[cnt++] = factors[i];  // сохраняем остальные
        }
    }
    if (const_prod == 0) 
    {
        // Возвращаем 0 и не копируем ничего
        return create_number_node(0);
    }
    Node* new_factors[MAX_EXPR];
    int n_cnt = 0;
    if (const_prod != 1) // если не 1 до надо добавить 
    {
        new_factors[n_cnt++] = create_number_node(const_prod);
    }
    for (int i = 0; i < cnt; i++) // добавляем копии 
    {
        new_factors[n_cnt++] = copy_tree(other_factors[i]);
    }
    if (n_cnt == 0) 
    {
        return create_number_node(1);
    }
    if (n_cnt == 1) 
    {
        return new_factors[0];
    }
    Node* result = new_factors[0];
    for (int i = 1; i < n_cnt; i++) 
    {
        result = create_operator_node('*', result, new_factors[i]);
    }
    return result;
}
Node* simplify_tree(Node* root) 
{
    if (!root) return NULL;
    root->left = simplify_tree(root->left); // упрощаем поддеревья 
    root->right = simplify_tree(root->right);
    if (root->type == NODE_OPERATOR && root->data.opt == '*') // просто умножаем 
    {
        return simplify_multiplication(root);
    }
    return root;
}
void print_infix(Node* root, int parent_prec, int is_right) 
{
    if (!root) 
        return;  
    if (root->type == NODE_NUMBER) 
    {
        printf("%d", root->data.number);
    } 
    else if (root->type == NODE_VARIABLE) 
    {
        printf("%c", root->data.variable);
    } 
    else
    {
        char op = root->data.opt;
        int cur_prec = precedence(op);
        int need_paren = (parent_prec > cur_prec) ||
                         (parent_prec == cur_prec && !right_associative(op) && is_right); // смотрим нужны ли скобки 
        if (need_paren) 
            printf("(");
        print_infix(root->left, cur_prec, 0);
        printf(" %c ", op);
        print_infix(root->right, cur_prec, 1);
        if (need_paren)
            printf(")");
    }
}
void print_expression(Node* root) 
{
    print_infix(root, 0, 0);
    printf("\n");
}
void process_expression(const char* expr, int num) 
{
    printf("Выражение %d: %s\n", num, expr);
    Node* tree = build_tree(expr);
    if (tree == NULL) 
    {
        printf("Ошибка: Некорректное выражение\n");
        return;
    }
    printf("Дерево (инфиксная форма): ");
    print_expression(tree);
    Node* simplified = simplify_tree(tree);
    printf("Упрощённое выражение: ");
    print_expression(simplified);
    printf("\n");
    free_tree(tree);
    if (simplified != tree) // проверка на удаление
    {
        free_tree(simplified);
    }
}

void read_from_file(void) 
{
    FILE *file = fopen("test.txt", "r");
    if (!file) 
    {
        printf("Ошибка: Не удалось открыть файл 'test.txt'\n");
        return;
    }
    printf("Чтение из файла 'test.txt'\n");
    char line[MAX_LINE];
    int line_num = 0;
    while (fgets(line, sizeof(line), file)) 
    {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;
        process_expression(line, ++line_num);
    }
    fclose(file);
}
void read_from_console() 
{
    printf("Ввод с клавиатуры\n");
    printf("Введите выражения (для выхода введите'exit'):\n");
    char input[MAX_LINE];
    int cnt = 0;
    while (1) 
    {
        if (!fgets(input, sizeof(input), stdin)) 
        {
            break;
        }
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "exit") == 0) 
        {
            printf("Выход из режима ввода с клавиатуры\n");
            break;
        }
        if (strlen(input) == 0) 
            continue; 
        process_expression(input, ++cnt);
    }
}
int main(void) 
{
    setlocale(LC_ALL, "Russian");
    int choice;
    while (1) 
    {
        printf("ВЫБОР РЕЖИМА ВВОДА\n");
        printf("1. Чтение из файла\n");
        printf("2. Ввод с клавиатуры\n");
        printf("3. Выход\n");
        printf("Ваш выбор: ");
        
        if (scanf("%d", &choice) != 1) 
        {
            printf("Неверный ввод. Пожалуйста, введите число.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        switch (choice) 
        {
            case 1:
                read_from_file();
                break;
            case 2:
                read_from_console();
                break;
            case 3:
                printf("До свидания!\n");
                return 0;
            default:
                printf("Неверный выбор. Пожалуйста, введите 1, 2 или 3.\n");
        }
    }
    return 0;
}
