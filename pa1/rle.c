#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int length = strlen(argv[1]);
    char *letters = (char*) malloc(length+1);

    for(int i = 0; i < length; i++)
        *(letters+i) = argv[1][i];

    *(letters+length) = '\0';

    char *compressed = (char*) malloc(length);
    int *count = (int*) malloc(sizeof(int) * length);
    int placeInCompressed = 0;

    for(int i = 0; i < length; i++)
    {
        if(isdigit(*(letters+i)))
        {
            printf("error");
            free(letters);
            free(compressed);
            free(count);
            return 0;
        }

        if(2*(placeInCompressed+1) > length)
        {
            printf("%s\n", letters);
            free(letters);
            free(compressed);
            free(count);
            return 0;
        }

        int counter = 1;
        char original = *(letters+i);
        while(*(letters+i) == *(letters+i+1))
        {
            counter++;
            i++;
        }
        
        *(compressed+placeInCompressed) = original;
        *(count+placeInCompressed) = counter;
        placeInCompressed++;
    }

    for(int i = 0; i < placeInCompressed; i++)
    {
        printf("%c", *(compressed+i));
        printf("%d", *(count+i));
    }

    free(letters);
    free(compressed);
    free(count);

    return 0;
}