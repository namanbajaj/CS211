#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Maintain stack of open delimeteres (,{,[
 * If closing delimeter found ),},] --> pop open delimeter off stack
 * If delimeters do not match or stack is empty, print index for unexpected delimeter and what delimeter it is
 * If stack not empty when end of input reached, print open followed by list of delimeters needed to balance string
 * Non delimeter characters can be ignored
 * */

int main(int argc, char *argv[])
{
    int length = strlen(argv[1]);
    char *stack = (char*) malloc(length);
    int loc = 0;

    char *symbols = (char*) malloc(length+1);
    for(int i = 0; i < length; i++)
        *(symbols+i) = argv[1][i];
    *(symbols+length) = '\0';

    for(int i = 0; i < length; i++)
    {
        char c = *(symbols+i);
        
        if(c == '(' || c == '{' || c == '[')
        {
            *(stack+loc) = c;
            loc++;
        }

        else if(c == ')' || c == '}' || c == ']')
        {
            if(loc == 0)
            {
                printf("%d: %c\n",i,c);
                free(stack);
                free(symbols);
                return 1;
            }

            char p = *(stack+loc-1);
            loc--;

            if(c == ')')
            {
                if(p != '(')
                {
                    printf("%d: )",i);
                    free(stack);
                    free(symbols);
                    return 1;
                }
            }

            if(c == '}')
            {
                if(p != '{')
                {
                    printf("%d: }",i);
                    free(stack);
                    free(symbols);
                    return 1;
                }
            }

            if(c == ']')
            {
                if(p != '[')
                {
                    printf("%d: ]",i);
                    free(stack);
                    free(symbols);
                    return 1;
                }
            }
        }
    }

    if(loc != 0)
    {
        printf("open: ");
        for(int i = loc-1; i > -1; i--)
        {
            char c = *(stack+i);
            if(c == '{')
                printf("}");
            if(c == '(')
                printf(")");
            if(c == '[')
                printf("]");
        }
        printf("\n");

        free(stack);
        free(symbols);

        return 1;
    }

    free(stack);
    free(symbols);
    return 0;
}