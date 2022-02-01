#include <stdio.h>
#include <stdlib.h>

struct node
{
    int value;
    struct node *next;
};

struct node *head = NULL;

void sort(struct node *list)
{
    struct node *listOne;
    struct node *listTwo;
    int temp;

    listOne = list;

    while(listOne != NULL)
    {
        listTwo = listOne->next;
        while(listTwo != NULL)
        {
            if(listOne->value > listTwo->value)
            {
                temp = listOne->value;
                listOne->value = listTwo->value;
                listTwo->value = temp;
            }
            listTwo = listTwo->next;
        }
        listOne = listOne->next;
    }
}

_Bool contains(int num)
{
    struct node *temp = head;
    while(temp != NULL)
    {
        if(temp->value == num)
            return 1;
        temp = temp->next;
    }

    return 0;
}

void insert(int num)
{
    if(!contains(num))
    {
        struct node *link = (struct node*) malloc(sizeof(struct node));
        link->value = num;
        link->next = head;
        head = link;
        sort(head);
    }
}

void delete(int num)
{
    if(contains(num))
    {
        if(head->value == num)
        {
            struct node *remove = head;
            head = head->next;
            free(remove);
            return;
        }
        
        struct node *temp = head;
        while(temp != NULL)
        {
            if(temp->next->value == num)
            {
                struct node *remove = temp->next;
                temp->next = temp->next->next;
                free(remove);
                break;
            }
            temp = temp->next;
        }
    }
}

int main()
{
    char command;
    int value;
    int size = 0;

    while(1)
    {
        if(scanf("%c %d", &command, &value) == EOF)
            break;
        
        if(command == 'i')
        {
            if(!contains(value))
                size++;
            insert(value);
        }

        if(command == 'd')
        {
            if(contains(value))
                size--;
            delete(value);
        }

        printf("%d :", size);

        struct node *temp = head;
        while(temp != NULL)
        {
            printf(" %d", temp->value);
            temp = temp->next;
        }
        printf("\n");

        getchar();   
    }
    
    return 0;
}