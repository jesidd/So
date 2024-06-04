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

typedef struct {
    int tamVector;
    char *vector;
    int a,t,c,g;
    int nSecuencia;
    int inicio;
    int final;
    int index;
    int *posx;
    int numProcess;
}Data;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int *matrizp;

void crearArchivoFinal(char **vector, int* col){

    FILE *file = fopen("vectorResultante.dat","w");

    int tamVector = *col;

    if(file != NULL){
        fprintf(file,"Columnas: %d", tamVector);

        
        for (int j = 0; j < tamVector; j++)
        {
            fprintf(file,"%c",(*vector)[j]);
        }
    
        
        fclose(file);
    }else{
        printf("error");
        exit(0);
    }

}


void leerVector(char **vector,int *tamVector,char *filename){
    FILE *file = fopen(filename,"r");

    int col;

    fscanf(file,"%d",&col);

    *tamVector = col;


    (*vector) = (char*)malloc((col) *sizeof(char));


    
    for(int j=0; j<col; j++){
        fscanf(file,"%c",&(*vector)[j]); 
    }

    fclose(file);
}

void mostrarVector(char **vector, int *tamVector){
   
    for(int j=0; j<*tamVector; j++){
        printf("%c",(*vector)[j]);
    }


}

void* contarBase(void* param){

    Data *datos =(Data*) param;
    
    datos->a = 0;
    datos->t = 0;
    datos->c = 0;
    datos->g = 0;

    for (int i = 0; i < datos->tamVector; i++)
    {
        switch (datos->vector[i])
        {
        case 'a':
            datos->a+=1;
        break;

        case 't':
            datos->t+=1;
        break;

        case 'c':
            datos->c+=1;
        break;

        case 'g':
            datos->g+=1;
        break;

        default:
            break;
        }
    }
    
}

void* contarSecuencia(void* param){

    Data *datos =(Data*) param;

    char secuencia[] = "gcgtga";

    int longitud_secuencia = strlen(secuencia);
    int longitud_vec = strlen(datos->vector);

    int i = datos->inicio;
    int fin = datos->final;

    for (i ; i < fin ; i++)
    {    
        if (strncmp(&datos->vector[i], secuencia, longitud_secuencia) == 0) {
            pthread_mutex_lock(&mutex);
            matrizp[datos->index] = datos->numProcess; 
            pthread_mutex_unlock(&mutex);
        
            datos->nSecuencia++;
            datos->posx[datos->index] = i;
            datos->index++;
        }
    }
}


int main(int argc, char* argv[]){

    char *vector;
    int tamVector;
    int *posx;

    leerVector(&vector,&tamVector,argv[2]);
    //mostrarVector(&vector,&tamVector);
    crearArchivoFinal(&vector,&tamVector);

    Data *datos = (Data*) malloc(tamVector * sizeof(Data));
    matrizp = (int*) malloc(tamVector * sizeof(int));
    

    datos->tamVector = tamVector;
    datos->vector = vector;
    posx =(int*) malloc((tamVector) *sizeof(int));
    datos->posx = posx;
    datos->index = 0;

    int nHilos = atoi(argv[1]);   

    pthread_t hilo[nHilos];

    pthread_create(&hilo[0],NULL,contarBase,(void*) datos);
    pthread_join(hilo[0],NULL);

    printf("Numero de bases\n [a]:%d\n [t]:%d \n [c]:%d \n [g]:%d \n ",datos->a,datos->t,datos->c,datos->g);

    datos->tamVector = tamVector;
    datos->nSecuencia = 0;
    datos->vector = vector;

    int delta = tamVector/nHilos;

    for (int i = 0; i < nHilos; i++)
    {   
        if(nHilos % 2 == 0){
            datos->inicio =(int) (delta) * i;
            datos->final = (int) (delta) * (i+1);
        }else{
            datos->inicio =(int) (delta) * i;
            datos->final = (int) (delta) * (i+1);
        }

        if(i == nHilos-1){
            datos->final = tamVector;
        }

        datos->numProcess = i;
       
        pthread_create(&hilo[i],NULL,contarSecuencia,(void*) datos);
        pthread_join(hilo[i],NULL);
    }
    
    for(int i = 0; i < datos->index; i++){
        printf("Proceso [%d] Secuencia en la posicion [%d]\n",matrizp[i],datos->posx[i]);
    }
    
    printf("Numero de secuencias: %d\n",datos->nSecuencia);
    printf("Tamaño del vector: %d\n",datos->tamVector);

    free(datos);
    free(vector);
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}
