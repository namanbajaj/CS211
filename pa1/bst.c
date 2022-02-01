#include <stdio.h>
#include <stdlib.h>

struct node
{
    int data;
    struct node *right;
    struct node *left;
};

struct node* insert(struct node *n, int value)
{
    if(n == NULL)
    {
        n = malloc(sizeof(struct node));
        n->data = value;
        n->left = NULL;
        n->right = NULL;
        printf("inserted\n");
    }
    else if(value > n->data)
        n->right = insert(n->right, value);
    else if(value < n->data)
        n->left = insert(n->left, value);
    return n;
}

_Bool search(struct node *n, int value)
{
    if(n == NULL)
        return 0;
    else if(n->data == value)
        return 1;
    else if(value < n->data)
        return search(n->left, value);
    else if(value > n->data)
        return search(n->right, value);
    return 0;
}

void print(struct node *n)
{
    if(n != NULL)
    {
        printf("(");
        print(n->left);
        printf("%d",n->data);
        print(n->right);
        printf(")");
    }
}

struct node* delete(struct node *n, int value)
{
    if(n == NULL)
        return NULL;
    if(value > n->data)
        n->right = delete(n->right, value);
    else if(value < n->data)
        n->left = delete(n->left, value);
    else if(value == n->data)
    {
        if(n->left == NULL && n->right == NULL){
            free(n);
            return NULL;
        }
            
        else if(n->left == NULL || n->right == NULL)
        {
            if(n->left == NULL){
                int val = n->right->data;
                free(n->right);
                n->data = val;
                n->right = NULL;

            }
            else if(n->right == NULL){
                int val = n->left->data;
                free(n->left);
                n->data = val;
                n->left = NULL;
            }
        }

        else
        {
            struct node *temp = n->left;
            while(temp->right != NULL)
                temp = temp->right;
            n->data = temp->data;
            n->left = delete(n->left, temp->data);
        }
    }
    return n;
}

void freeNode(struct node *n)
{
    if(n->right == NULL && n->left == NULL){
        free(n);
    }
    else if(n->right == NULL && n->left != NULL){
        freeNode(n->left);
        free(n);
    }
    else if(n->left == NULL && n->right != NULL){
        freeNode(n->right);
        free(n);
    }
    else if(n->left != NULL && n->right != NULL){
        freeNode(n->left);
        freeNode(n->right);
        free(n);
    }
}

int main()
{
    struct node *root = NULL;
    
    char command;
    int value;

    while(1)
    {
        if(scanf("%c", &command) == EOF)
            break;
        
        if(command != 'p')
            scanf("%d", &value);
        
        if(command == 'i'){
            if(!search(root, value))
                root = insert(root, value);
            else
                printf("not inserted\n");
        }

        if(command == 'd'){
            if(search(root, value)){
                root = delete(root, value);
                printf("deleted\n");
            }
            else
                printf("absent\n");
        }


        if(command == 's'){
            if(search(root, value))
                printf("present\n");
            else
                printf("absent\n");

        }

        if(command == 'p'){
            print(root);
            printf("\n");
        }
            
        getchar();
    }
    
    // free(root);
    freeNode(root);

    return 0;

}