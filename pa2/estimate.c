#include <stdio.h>
#include <stdlib.h>

void print(double **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%.0f", matrix[i][j]);
        printf("\n");
    }
}

double **transpose(double **matrix, int rows, int cols)
{
    double **t = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        t[i] = malloc(cols * sizeof(double));

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            t[i][j] = matrix[j][i];

    return t;
}

double **multiply(double **m1, int r1, int c1, double **m2, int r2, int c2)
{
    double **result = malloc(r1 * sizeof(double *));
    for (int i = 0; i < r1; i++)
        result[i] = malloc(c2 * sizeof(double));

    for (int i = 0; i < r1; ++i)
    {
        for (int j = 0; j < c2; ++j)
        {
            result[i][j] = 0;
            for (int k = 0; k < c1; ++k)
                result[i][j] += m1[i][k] * m2[k][j];
        }
    }

    return result;
}

double *divide(double *matrix, int dimension, double divisor)
{
    for (int i = 0; i < dimension; i++)
        matrix[i] /= divisor;
    return matrix;
}

double *subtract(double *sub, double multiplier, double *from, int dimension)
{
    for (int i = 0; i < dimension; i++)
        from[i] -= sub[i] * multiplier;
    return from;
}

double **inverse(double **matrix, int dimension)
{
    double **inv = malloc(dimension * sizeof(double *));
    for (int i = 0; i < dimension; i++)
        inv[i] = malloc(dimension * sizeof(double));

    for (int i = 0; i < dimension; i++)
    {
        for (int j = 0; j < dimension; j++)
        {
            if (i == j)
                inv[i][j] = 1;
            else
                inv[i][j] = 0;
        }
    }

    for (int p = 0; p < dimension; p++)
    {
        double multiplier = matrix[p][p];
        matrix[p] = divide(matrix[p], dimension, multiplier);
        inv[p] = divide(inv[p], dimension, multiplier);
        for (int i = p + 1; i < dimension; i++)
        {
            double value = matrix[i][p];
            matrix[i] = subtract(matrix[p], value, matrix[i], dimension);
            inv[i] = subtract(inv[p], value, inv[i], dimension);
        }
    }

    for (int p = dimension - 1; p >= 0; p--)
    {
        for (int i = p - 1; i >= 0; i--)
        {
            double value = matrix[i][p];
            matrix[i] = subtract(matrix[p], value, matrix[i], dimension);
            inv[i] = subtract(inv[p], value, inv[i], dimension);
        }
    }

    // printf("inside inverse method\nmatrix\n");
    // print(matrix, dimension, dimension);
    // printf("\ninverse\n");
    // print(inv, dimension, dimension);

    return inv;
}

int main(int argc, char *argv[])
{
    // printf("%s\n",argv[1]);
    // printf("%s\n", argv[2]);
    // printf("%s",argv[3]);

    //train matrix

    //open train.txt
    FILE *trainFile = fopen(argv[1], "r");

    //read "train"
    char *trainWord = malloc(6);
    fscanf(trainFile, "%s", trainWord);
    // printf("%s\n", trainWord);

    //get number of attributes
    double trainAttributes;
    fscanf(trainFile, "%lf", &trainAttributes);
    // printf("%lf\n", trainAttributes);
    int cols = (int)trainAttributes + 1;

    //get number of houses
    double trainHouses;
    fscanf(trainFile, "%lf", &trainHouses);
    // printf("%lf\n", trainHouses);
    int rows = (int)trainHouses;

    //initialize train matrix
    double **train = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        train[i] = malloc((cols) * sizeof(double));

    //initialize price matrix
    double **prices = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++)
        prices[i] = malloc(sizeof(double));

    //set column 1 of train matrix to 0
    for (int i = 0; i < rows; i++)
        train[i][0] = 1;

    //set train matrix values
    //1st column is all ones, columns 2 to attributes is attributes, prices go in prices matrix
    double trainValue;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 1; j < cols; j++)
        {
            fscanf(trainFile, "%lf", &trainValue);
            train[i][j] = trainValue;
        }
        fscanf(trainFile, "%lf", &trainValue);
        prices[i][0] = trainValue;
    }

    // for(int i = 0; i < rows; i++){
    //     for(int j = 0; j < trainAttributes+1; j++){
    //         printf("%.0f ", train[i][j]);
    //     }
    //     printf("%.0f\n", prices[i][0]);
    // }

    //data matrix

    //open data.txt
    FILE *dataFile = fopen(argv[2], "r");

    //read "data"
    char *dataWord = malloc(5);
    fscanf(dataFile, "%s", dataWord);
    // printf("\n%s\n", dataWord);

    //get number of attributes
    double dataAttributesDouble;
    fscanf(dataFile, "%lf", &dataAttributesDouble);
    // printf("%lf\n", dataAttributesDouble);
    int dataAttributes = (int)dataAttributesDouble;

    //get number of houses
    double datahousesDouble;
    fscanf(dataFile, "%lf", &datahousesDouble);
    // printf("%lf\n", datahousesDouble);
    int dataHouses = (int)datahousesDouble;

    //initialize data matrix
    double **data = malloc(dataHouses * sizeof(double *));
    for (int i = 0; i < dataHouses; i++)
        data[i] = malloc((dataAttributes + 2) * sizeof(double));

    //set column 1 of data matrix to 0
    for (int i = 0; i < dataHouses; i++)
        data[i][0] = 1;

    //set data matrix values
    double dataValue;
    for (int i = 0; i < dataHouses; i++)
    {
        for (int j = 1; j < dataAttributes + 1; j++)
        {
            fscanf(dataFile, "%lf", &dataValue);
            data[i][j] = dataValue;
        }
    }

    // print(data, dataHouses, dataAttributes+1);

    // printf("\n%d rows\n%d columns\n\n", rows, cols);

    //tranpose of train
    double **trainTranspose = transpose(train, cols, rows);

    // printf("train transpose\n");
    // print(trainTranspose, cols, rows);

    //multiply tranpose of train and train
    double **stepOne = multiply(trainTranspose, cols, rows, train, rows, cols);

    // printf("\ntrain transpose * train\n");
    // print(W, cols, cols);
    // printf("\n");

    //inverse
    double **inv = inverse(stepOne, cols);
    // print(W, cols, cols);

    //inverse * train transpose
    // printf("\ninverse * train transpose\n");
    double **stepTwo = multiply(inv, cols, cols, trainTranspose, cols, rows);
    // print(W, cols, rows);

    //W * prices
    // printf("\nW * prices\n");
    double **stepThree = multiply(stepTwo, cols, rows, prices, rows, 1);
    // print(W, cols, 1);

    //data * prices
    // printf("\nFinal matrix\n");
    dataAttributes++;
    double **Y = multiply(data, dataHouses, dataAttributes, stepThree, cols, 1);
    print(Y, dataHouses, 1);

    fclose(trainFile);
    fclose(dataFile);

    free(trainWord);
    free(dataWord);

    for (int i = 0; i < rows; i++)
    {
        free(train[i]);
        free(prices[i]);
    }
    free(train);
    free(prices);

    for (int i = 0; i < dataHouses; i++)
    {
        free(data[i]);
        free(Y[i]);
    }
    free(data);
    free(Y);

    for (int i = 0; i < cols; i++)
    {
        free(trainTranspose[i]);
        free(stepOne[i]);
        free(inv[i]);
        free(stepTwo[i]);
        free(stepThree[i]);
    }
    free(trainTranspose);
    free(stepOne);
    free(inv);
    free(stepTwo);
    free(stepThree);

    return 0;
}