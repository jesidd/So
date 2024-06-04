#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct
{
    int **matriz;
    int tamRow;
    int tamCol;
    int inicio;
    int final;
    int numProcess;
    int *posX;
    int *posY;
    int index;
} Data;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int *vectorP;

void leerMatriz(int ***matriz, int *tamRow, int *tamCol, char *filename)
{
    FILE *file = fopen(filename, "r");

    int fila, col;

    fscanf(file, "%d", &fila);

    *tamRow = fila;

    fscanf(file, "%d", &col);

    *tamCol = col;

    (*matriz) = (int **)malloc((col) * sizeof(int *));

    for (int i = 0; i < col; i++)
    {
        (*matriz)[i] = (int *)malloc((col) * sizeof(int));
    }

    for (int i = 0; i < fila; i++)
    {
        for (int j = 0; j < col; j++)
        {
            fscanf(file, "%d", &(*matriz)[i][j]);
        }
    }

    fclose(file);
}

void mostrarMatriz(int ***matriz, int *rows, int *cols)
{
    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            printf("%d", (*matriz)[i][j]);
            printf("\t");
        }
        printf("\n");
    }

    printf("\n");
}

void *buscarNido(void *param)
{
    Data *datos = (Data *)param;

    int existe = 0;

    for (int i = datos->inicio; i < datos->final; i++)
    {

        for (int j = 0; j < datos->tamCol; j++)
        {
            if (i-1 &&)
            {
                if (datos->matriz[i][j] == 2)
                {
                    if (datos->matriz[i - 1][j] == 1)
                    {
                        existe = 1;
                    }
                    else
                    {
                        if (datos->matriz[i - 1][j + 1] == 1)
                        {
                            existe = 1;
                        }
                        else
                        {
                            if (datos->matriz[i - 1][j - 1] == 1)
                            {
                                existe = 1;
                            }
                            else
                            {
                                if (datos->matriz[i][j - 1] == 1)
                                {
                                    existe = 1;
                                }
                                else
                                {
                                    if (datos->matriz[i][j + 1] == 1)
                                    {
                                        existe = 1;
                                    }
                                    else
                                    {
                                        if (datos->matriz[i + 1][j] == 1)
                                        {
                                            existe = 1;
                                        }
                                        else
                                        {
                                            if (datos->matriz[i + 1][j - 1])
                                            {
                                                existe = 1;
                                            }
                                            else
                                            {
                                                if (datos->matriz[i + 1][j + 1])
                                                {
                                                    existe = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (existe == 1)
                    {
                        pthread_mutex_lock(&mutex);
                        vectorP[datos->index] = datos->numProcess;
                        pthread_mutex_unlock(&mutex);
                        datos->posX[datos->index] = j;
                        datos->posY[datos->index] = i;
                        datos->index++;
                        printf("existe");
                    }
                }
            }
        }
    }
    exit(1);
}

int main(int argc, char *argv[])
{
    int **matriz;
    int tamRow, tamCol;
    int nHilos = 4;

    vectorP = (int *)malloc(tamCol * sizeof(int));

    Data *datos = (Data *)malloc((tamCol) * sizeof(Data));

    pthread_t hilo[nHilos];

    leerMatriz(&matriz, &tamRow, &tamCol, argv[1]);
    // mostrarMatriz(&matriz,&tamRow,&tamCol);

    datos->matriz = matriz;
    datos->tamRow = tamRow;
    datos->tamCol = tamCol;
    datos->index = 0;

    int delta = (int)tamCol / nHilos;

    for (int i = 0; i < nHilos; i++)
    {

        if (nHilos % 2 == 0)
        {
            datos->inicio = (int)(delta)*i;
            datos->final = (int)(delta) * (i + 1);
        }
        else
        {
            datos->inicio = (int)(delta)*i;
            datos->final = (int)(delta) * (i + 1);
            if (i == nHilos - 1)
            {
                datos->final = tamCol;
            }
        }
        datos->numProcess = i;

        pthread_create(&hilo[i], NULL, buscarNido, (void *)datos);
        pthread_join(hilo[i], NULL);
    }

    for (int i = 1; i <= datos->index; i++)
    {
        printf("Proceso[%d] En la Posicion[%d][%d]\n", datos->numProcess, datos->posY[i], datos->posX[i]);
        printf("hola");
    }

    free(datos);
    free(matriz);
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}