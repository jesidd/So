#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct {
    int **matriz;
    int tamRow;
    int tamCol;
    int inicio;
    int final;
    int numProcess;

    int index;
} Data;

int *vectorP;
int **posX;
int **posY;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void leerMatriz(int ***matriz, int *tamRow, int *tamCol, char *filename) {
    FILE *file = fopen(filename, "r");
    int fila, col;
    fscanf(file, "%d", &fila);
    *tamRow = fila;
    fscanf(file, "%d", &col);
    *tamCol = col;
    (*matriz) = (int **)malloc((fila) * sizeof(int *));
    for (int i = 0; i < fila; i++) {
        (*matriz)[i] = (int *)malloc((col) * sizeof(int));
    }
    for (int i = 0; i < fila; i++) {
        for (int j = 0; j < col; j++) {
            fscanf(file, "%d", &(*matriz)[i][j]);
        }
    }
    fclose(file);
}


bool esNido(int ***matriz,int iR,int fR,int iC,int fC,int y , int x){

    for (int i = y-1; i < y+1; i++)
    {
        for (int j = x-1; j < x+1; j++)
        {
            if((i < iR || i > fR) || (j < iC || j > fC) || (i == y && j == x)) {
                continue;
            }

            if((*matriz)[i][j] == 2) {
                return true;
            }
        }
        
    }
    
}

void* buscarNido(void* param){
    Data *datos = (Data*) param;
    int cont = 0;

    for (int i = datos->inicio; i < datos->final; i++)
    {
        for (int j = 0; j < datos->tamCol; j++)
        {
            if(datos->matriz[i][j]==1){
                bool exist = esNido(&datos->matriz,datos->inicio,datos->final,0,100,i,j);
                if(exist){
                    pthread_mutex_lock(&mutex);
                    posX[datos->numProcess][cont]=j;
                    posY[datos->numProcess][cont]=i;
                    vectorP[datos->numProcess] = cont;
                    pthread_mutex_unlock(&mutex);
                    cont=cont+1;
                }
            }
        }
        
    }
    
}




int main (int argc, char *argv[]){
    int **matriz;
    int tamRow, tamCol;
    int nHilos = 4;
    pthread_t hilo[nHilos];

    vectorP = (int*) malloc(4*sizeof(int));
    posX = (int**) malloc(100*sizeof(int*));
    posY = (int**) malloc(100*sizeof(int*));


    for (int i = 0; i < 100; i++) {
        (posY)[i] = (int *)malloc((100) * sizeof(int));
        (posX)[i] = (int *)malloc((100) * sizeof(int));
    }

    leerMatriz(&matriz, &tamRow, &tamCol, argv[1]);

    Data *datos = (Data *) malloc(tamRow*sizeof(int));

    int delta = tamCol / 4;

    datos->matriz = matriz;
    datos->index = 0;
    datos->tamCol = tamCol;
    datos->tamRow = tamRow;


    for (int  i = 0; i < nHilos; i++)
    {
        datos->inicio = delta * (i);
        datos->final = delta * (i+1);

        datos->numProcess= i;

        pthread_create(&hilo[i],NULL,buscarNido,(void *)datos);

    }

    for (int i = 0; i < nHilos; i++)
    {
       pthread_join(hilo[i], NULL);
    }
    

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < vectorP[i]; j++)
        {
            
            printf("\nProceso[%d]",i);
            printf("\nNido Encontrado en [%d][%d]",posY[i][j],posX[i][j]);
        }
        
    }

    

}