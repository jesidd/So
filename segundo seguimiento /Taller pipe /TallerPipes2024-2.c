#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <stdbool.h>

typedef struct
{
    int x;
    int y;
    int hijoId;
}Data;


void leerMatriz (int ***matriz, int *row, int *col, char *filename){
    FILE *file = fopen(filename, "r");

    fscanf(file,"%d", row);
    fscanf(file,"%d", col);

    (*matriz) = (int **) malloc(*row * sizeof (int *));
    for (int i = 0 ; i < *row ; i++){
        (*matriz)[i] = (int *) malloc( *col * sizeof (int));
    }

    int i, j;
    for (i = 0; i < *row; i++) {
        for (j = 0; j < *col; j++) {
            fscanf(file, "%1d", &((*matriz)[i][j]));
        }
    }
    fclose(file);
}

void mostrarMatriz(int **matriz, int row, int col){
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

bool esNido(int **matriz,int iR,int fR,int iC,int fC,int y , int x){

    if( matriz[y][x] != 1) return false;

    for (int i = y-1; i <= y+1; i++)
    {
        for (int j = x-1; j <= x+1; j++)
        {
            if((i < iR || i > fR) || (j < iC || j > fC) || (i == y && j == x)) {
                continue;
            }

            if((matriz)[i][j] == 2) {
                return true;
            }
        }
        
    }
    
}

void buscarNido(int **matriz, int col, int sizeRowHijos[][2], int idHijo, int pipe_fd){

    int i = sizeRowHijos[idHijo][0];
    int row = sizeRowHijos[idHijo][1];

    for (int e = i; e < row; e++)
    {
        for (int j = 0; j < col; j++)
        {
            if (matriz[e][j] == 1){
                if (esNido(matriz, sizeRowHijos[idHijo][0], sizeRowHijos[idHijo][1], 0, col, e, j)){
                     // Encontramos un nido, enviamos los datos al padre
                    Data data;
                    data.x = e;
                    data.y = j;
                    data.hijoId = idHijo + 1; // Para que el hijo 0 sea el hijo 1

                    write(pipe_fd, &data, sizeof(Data)); // Escribir datos en la tubería

                }
            }
        }
        
    }
    close(pipe_fd);
}

int main(int argc, char *argv[]){

    int **matriz;
    int  row, col;


    leerMatriz(&matriz, &row, &col, argv[1]);
    mostrarMatriz(matriz, row, col);

    int nh = 2;  // numero de hijos
    int delta = row / nh;
    int sizeRowHijos[2][2];
    
    for (int  i = 0; i < nh; i++)
    {
        sizeRowHijos[i][0] = delta * (i);
        sizeRowHijos[i][1]= delta * (i+1);
    }
    
    //----------------------------- Creacion de tuberias --------------------------------
    int fd[2][2];  
    for (int i = 0; i < nh; i++) {
        pipe(fd[i]);  
    }

    //---------------------------creacion de Procesos -----------------------------------
    int i = 0;
    for (i; i < 2; i++)
    {
        if(!fork()){
            close(fd[i][0]);
            break;
        }else {
            close(fd[i][1]); // Cerrar el lado de escritura en el padre
        }
    }


    //----------------------- Logica de programacion y comunicacion ----------------------
    switch (i)
    {
    case 0:

        buscarNido(matriz, col, sizeRowHijos, 0, fd[0][1]);
        break;

    case 1:

        buscarNido(matriz, col, sizeRowHijos, 1, fd[1][1]);
        break;

    default:
        //----------------------- Lectura de resultados en el padre -------------------------
        Data receivedData;
        for (int i = 0; i < nh; i++) {
            while (read(fd[i][0], &receivedData, sizeof(Data)) > 0) {
                printf("Nido encontrado en la posición (%d,%d) por el hijo %d\n",
                    receivedData.x, receivedData.y, receivedData.hijoId);
            }
            close(fd[i][0]); // Cerrar el lado de lectura después de usarlo
        }
        break;
    }

    
    // Esperar a que los hijos terminen
    for (int i = 0; i < nh; i++) {
        wait(NULL);
    }

    // Liberar memoria de la matriz
    for (int i = 0; i < row; i++) {
        free(matriz[i]);
    }

    free(matriz);

    return EXIT_SUCCESS;
}