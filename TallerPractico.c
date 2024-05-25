#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

int **matrizSuma;

typedef struct {
    int rows;
    int cols;
    int **matriz1;
    int **matriz2;
}Data;


void leerMatriz(int ***matriz2,int ***matriz,int *rows,int *cols,char *filename){
    FILE *file = fopen(filename,"r");

    int row,col;

    fscanf(file,"%d",&row);
    fscanf(file,"%d",&col);

    *rows = row;
    *cols = col;


    (*matriz) = (int**)malloc(row*sizeof(int*));
    (*matriz2) = (int**)malloc(row*sizeof(int*));

    for(int i=0;i<row;i++){
        (*matriz)[i] = (int*)malloc(col*sizeof(int));
        (*matriz2)[i] = (int*)malloc(col*sizeof(int));
    }

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            fscanf(file,"%d",&(*matriz)[i][j]);
        }
    }

    char basura;
    fscanf(file,"%c",&basura);

    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){ 
            fscanf(file,"%d",&(*matriz2)[i][j]);
        }
    }

}

void mostrarMatriz(int ***matriz, int *rows, int *cols){
    for(int i=0; i<*rows; i++){
        for(int j=0; j<*cols; j++){
            printf("%d",(*matriz)[i][j]);
            printf("\t");
        }
        printf("\n");
    }

    printf("\n");
}

void* crearArchivoFinal(void *param){

    Data *datos = (Data*) param;
    FILE *file = fopen("matrizResultante.txt","w");

    printf("ID Hilo: %lu",pthread_self());

    if(file != NULL){
        fprintf(file,"Filas: %d",datos->rows);
        fprintf(file,"\nColumnas: %d",datos->cols);
        fprintf(file,"\n");

        for (int i = 0; i < datos->rows; i++)
        {
            for (int j = 0; j < datos->cols; j++)
            {
                fprintf(file,"%d ",matrizSuma[i][j]);
            }
            fprintf(file,"\n");
        }
        

        fclose(file);
    }else{
        printf("error");
        exit(0);
    }

    
    exit(1);
}



void* sumarMatriz(void* param/*int ***matriz,int ***matriz2, int *rows, int *cols+*/){ 

    Data *datos =(Data*) param; 

    (matrizSuma)= (int**) malloc((datos->rows)* sizeof(int*));

    for(int i=0; i<datos->cols; i++){
        (matrizSuma)[i]= (int*) malloc((datos->cols)* sizeof(int));
    }

    if((datos->rows)==(datos->cols)){
        for(int i=0; i<(datos->rows); i++){
            for(int j=0; j<(datos->cols); j++){
                (matrizSuma)[i][j] = (datos->matriz1)[i][j] + (datos->matriz2)[i][j];
            }
        }
    }else{
        exit(0);
    }

    printf("\nID Hilo: %lu",pthread_self());
    printf("\n");
    mostrarMatriz(&matrizSuma,&datos->rows,&datos->cols);

    pthread_t hilo;

    pthread_create(&hilo,NULL,crearArchivoFinal,(void *)datos);
    pthread_join(hilo,NULL);

    exit(1);
}


int main(int argc, char* argv[]){
    int **matriz, **matriz2;
    int rows;
    int cols;

    Data *datos = (Data*) malloc(rows * sizeof(Data));

    leerMatriz(&matriz2,&matriz,&rows,&cols,argv[1]);
    //mostrarMatriz(&matriz2,&rows,&cols);
    //sumarMatriz(&matriz,&matriz2,&rows,&cols);

    datos->rows = rows;
    datos->cols = cols;
    datos->matriz1 = matriz;
    datos->matriz2 = matriz2;
    
    printf("ID Hilo Principal: %lu",pthread_self());

    pthread_t hilo;
    //hilo = malloc(rows * sizeof(pthread));

    pthread_create(&hilo,NULL,sumarMatriz,(void *) datos);

    pthread_join(hilo,NULL);

    return 0;
}