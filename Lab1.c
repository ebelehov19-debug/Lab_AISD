#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* Create_Node(int val)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        printf("ERROR MEMORY\n");
        exit(1);
    }
    new_node->data = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Node* Add_Node(Node* root, int val)
{
    if (root == NULL) {
        return Create_Node(val);
    }
    if (val < root->data)
    {
        root->left = Add_Node(root->left, val);
    }
    else if (val > root->data)
    {
        root->right = Add_Node(root->right, val);
    }
    else
    {
        printf("Value %d already in tree\n", val);
    }
    return root;
}

void Print_Tree(Node* root, int tab)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }

    for (int i = 0; i < tab; i++)
    {
        printf("    ");
    }
    printf("%d\n", root->data);

    if (root->left != NULL)
        Print_Tree(root->left, tab + 1);
    if (root->right != NULL)
        Print_Tree(root->right, tab + 1);
}

Node* Min_val(Node* root)
{
    if (root == NULL) return NULL;

    Node* t = root;
    while (t != NULL && t->left != NULL)
    {
        t = t->left;
    }
    return t;
}

Node* Del_Node(Node* root, int val)
{
    if (root == NULL)
    {
        printf("Value %d not found in tree\n", val);
        return NULL;
    }

    if (val < root->data)
    {
        root->left = Del_Node(root->left, val);
    }
    else if (val > root->data)
    {
        root->right = Del_Node(root->right, val);
    }
    else
    {
        if (root->left == NULL)
        {
            Node* t = root->right;
            free(root);
            return t;
        }
        else if (root->right == NULL)
        {
            Node* t = root->left;
            free(root);
            return t;
        }
        else
        {
            Node* t = Min_val(root->right);
            int del_val = root->data; 
            root->data = t->data;
            root->right = Del_Node(root->right, t->data);
        }
    }
    return root;
}

void Min_Deep(Node* root, int deep, int* min_d, int* min_val)
{
    if (root == NULL)
    {
        return;
    }
    if (root->left == NULL && root->right == NULL)
    {
        if (deep < *min_d)
        {
            *min_d = deep;
            *min_val = root->data;
        }
        return;
    }
    Min_Deep(root->left, deep + 1, min_d, min_val);
    Min_Deep(root->right, deep + 1, min_d, min_val);
}
void Free_Tree(Node* root)
{
    if (root == NULL)
    {
        return;
    }
    Free_Tree(root->right);
    Free_Tree(root->left);
    free(root);
}

int main()
{
    Node* root = NULL;

    printf("1 - Add Node\n");
    printf("2 - Delete Node\n");
    printf("3 - Print Tree\n");
    printf("4 - Value in Min deep\n");
    printf("5 - Stop\n\n");

    while (1)
    {
        int num;
        printf("Choose option: ");
        scanf("%d", &num);
        printf("\n");

        switch (num)
        {
        case 1:
        {
            int val = 0;
            printf("Value to add: ");
            scanf("%d", &val);
            root = Add_Node(root, val);
            printf("Added %d to tree\n\n", val);
            break;
        }
        case 2:
        {
            int val;
            printf("Value to delete: ");
            scanf("%d", &val);
            root = Del_Node(root, val);
            printf("Deleted %d from tree\n\n", val);
            break;
        }
        case 3:
        {
            printf("Tree:\n");
            Print_Tree(root, 0);
            printf("\n");
            break;
        }
        case 4:
        {
            if (root == NULL)
            {
                printf("Tree is empty\n\n");
                break;
            }

            int mi = 99999;
            int val = 0;
            Min_Deep(root, 1, &mi, &val);
            if (mi != 99999)
            {
                printf("Minimum deep: %d\n", mi);
                printf("Value at minimum deep: %d\n\n", val);
            }
            else
            {
                printf("Nodes no found\n\n");
            }
            break;
        }
        case 5:
        {
            printf("End\n");
            Free_Tree(root);
            return 0;
        }
        default:
        {
            printf("No found option\n\n");
            break;
        }
        }
    }

    return 0;
}
