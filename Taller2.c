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
}Data;


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
int secuencias=0;
pthread_mutex_t my = PTHREAD_MUTEX_INITIALIZER;

void* contarSecuencia(void* param){

    Data *datos =(Data*) param;

    char secuencia[] = "gcgtga";

    int longitud_secuencia = strlen(secuencia);
    int longitud_vec = strlen(datos->vector);

    int i = datos->inicio;
    int fin = datos->final;

    if(fin == 99999){
        fin = longitud_vec;
    }

    for (i ; i < fin ; i++)
    {    
        if (strncmp(&datos->vector[i], secuencia, longitud_secuencia) == 0) {
            datos->nSecuencia++;
            pthread_mutex_lock(&my);
            secuencias++;
            pthread_mutex_unlock(&my);
        }
    }
}


int main(int argc, char* argv[]){

    char *vector;
    int tamVector;

    leerVector(&vector,&tamVector,argv[1]);
    //mostrarVector(&vector,&tamVector);
    crearArchivoFinal(&vector,&tamVector);

    Data *datos = (Data*) malloc(tamVector * sizeof(Data));

    datos->tamVector = tamVector;
    datos->vector = vector;

    int nHilos = 10;   

    pthread_t hilo[nHilos];

    pthread_create(&hilo[0],NULL,contarBase,(void*) datos);
    pthread_join(hilo[0],NULL);

    printf("Numero de bases\n [a]:%d\n [t]:%d \n [c]:%d \n [g]:%d \n ",datos->a,datos->t,datos->c,datos->g);

    datos->tamVector = tamVector;
    datos->nSecuencia = 0;
    datos->vector = vector;

    int delta = tamVector/9;

    for (int i = 0; i < 9; i++)
    {   
        datos->inicio =(int) (delta) * i;
        datos->final = (int) (delta) * (i+1);
        pthread_create(&hilo[i],NULL,contarSecuencia,(void*) datos);
        usleep(1000);
    }
    
    for (int i = 0; i < 9; i++)
    {
        pthread_join(hilo[i],NULL);
    }
    
    printf("Numero de secuencias: %d\n",datos->nSecuencia);
    printf("Numero de secuencias: %d\n",secuencias);
    printf("Tamaño del vector: %d\n",datos->tamVector);

    pthread_mutex_destroy(&my);
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}