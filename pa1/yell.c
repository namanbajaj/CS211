#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argv[1] != NULL && strlen(argv[1]) != 0)
    {
        int length = strlen(argv[1]);
        char *words = (char*) malloc(length);

        for(int i = 0; i < length; i++)
            *(words+i) = argv[1][i];

        for(int i = 0; i < length; i++)
        {
            if(islower(*(words+i)))
                printf("%c", *(words+i) - 32);
            else
                printf("%c", *(words+i));
        }
        printf("!!");

        free(words);
    }
    
    return 0;
}