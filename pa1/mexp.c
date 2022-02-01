#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //get filename
    int length = strlen(argv[1]);
    char *filename = (char*) malloc(length+1);
    for(int i = 0; i < length; i++)
        *(filename+i) = argv[1][i];
    *(filename+length) = '\0';

    //open file
    FILE *mexp = fopen(filename, "r");

    //get size
    double size;
    fscanf(mexp, "%lf", &size);
    int msize = (int) size;

    //initialize matrix
    int **matrix = malloc(msize * sizeof(int*));
    for(int i = 0; i < msize; i++)
        matrix[i] = malloc(msize * sizeof(int));

    //set matrix values
    double val;
    for(int i = 0; i < msize; i++)
    {
        for(int j = 0; j < msize; j++)
        {
            fscanf(mexp, "%lf", &val);
            int value = (int) val;
            matrix[i][j] = value;
        }
    }

    //get exponent
    double exp;
    int exponent;
    fscanf(mexp, "%lf", &exp);
    exponent = (int) exp;

    //check if exponent 0
    if(exponent == 0)
    {
        for(int i = 0; i < msize; i++)
        {
            for(int j = 0; j < msize; j++)
            {
                if(i == j)
                    matrix[i][j] = 1;
                else
                    matrix[i][j] = 0;
            }
        }

        for(int i = 0; i < msize; i++){
            for(int j = 0; j < msize; j++){
                printf("%d",matrix[i][j]);
                if(j != msize - 1)
                    printf(" ");
            }
            printf("\n");
        }
        
        return 0;
    }

    //initialize matrices for calculation
    int **matrixExp = malloc(msize * sizeof(int*));
    int **matrixTemp = malloc(msize * sizeof(int*));
    for(int i = 0; i < msize; i++)
    {
        matrixTemp[i] = malloc(msize * sizeof(int));
        matrixExp[i] = malloc(msize * sizeof(int));
    }
    for(int i = 0; i < msize; i++)
    {
        for(int j = 0; j < msize; j++)
        {
            matrixTemp[i][j] = matrix[i][j];
            matrixExp[i][j] = 0;
        }
    }

    //exponent
    _Bool usingTempforCalc = 1;
    for(; exponent > 1; exponent--)
    {
        if(usingTempforCalc)
        {
            for(int i = 0; i < msize; i++)
            {
                for(int j = 0; j < msize; j++)
                {
                    matrixExp[i][j] = 0;
                    for(int k = 0; k < msize; k++)
                    {
                        matrixExp[i][j] += matrixTemp[i][k] * matrix[k][j];   
                    }
                }
            }
            usingTempforCalc = 0;
        }

        else
        {
            for(int i = 0; i < msize; i++)
            {
                for(int j = 0; j < msize; j++)
                {
                    matrixTemp[i][j] = 0;
                    for(int k = 0; k < msize; k++)
                    {
                        matrixTemp[i][j] += matrixExp[i][k] * matrix[k][j];   
                    }
                }
            }
            usingTempforCalc = 1;
        }

    }

    //prints matrix based on which one did last calculation
    if(usingTempforCalc)
    {
        for(int i = 0; i < msize; i++){
            for(int j = 0; j < msize; j++){
                printf("%d",matrixTemp[i][j]);
                if(j != msize - 1)
                    printf(" ");
            }
            printf("\n");
        }
    }
    else
    {
        for(int i = 0; i < msize; i++){
            for(int j = 0; j < msize; j++){
                printf("%d",matrixExp[i][j]);
                if(j != msize - 1)
                    printf(" ");
            }
            printf("\n");
        }
    }

    //frees all memory
    for(int i = 0; i < msize; i++)
    {
        free(matrix[i]);
        free(matrixExp[i]);
        free(matrixTemp[i]);
    }
    fclose(mexp);
    free(filename);
    free(matrix);
    free(matrixExp);
    free(matrixTemp);

    return 0;
}