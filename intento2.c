#include<stdlib.h>
#include<wait.h>
#include<unistd.h>
#include<stdio.h>

struct data
{
    char letra;
};


int main(){

    int i;

    pid_t hijos;
    pid_t nietos[3];
    pid_t bisnietos[2];

    //creacion del struct
    struct data post;

    //creaccion de tuberia
    int fd[8][2];

    for (int m = 0; m < 8; m++)
    {
        pipe(fd[m]);
    }
    

    //creacion de procesos

    for ( i = 0; i < 3; i++)
    {
        
        if(!fork()){//hijos
            hijos = getppid();
            switch (i)
            {
            case 0:
                
                if(!fork()){//nietos
                    nietos[0] = getppid();
                    if(!fork()){//bisneitos
                        bisnietos[0] = getppid();
                    }
                    break;
                }
                break;
            
            case 1:
                if(!fork()){
                    nietos[1] = getppid();
                }
                break;

            case 2:
                
                if(!fork()){//nietos
                    nietos[2] = getppid();
                    if(!fork()){//bisneitos
                        bisnietos[2] = getppid();
                    }
                    break;
                }
                break;
            
            default:
                break;
            }


            break;
        }
    }
    

    //Accseso a procesos

    switch (i)
    {
    case 0:
        if(hijos == getppid()){
           
        }else{
            if(nietos[0] == getppid()){
               
            }else{
                if(bisnietos[0] == getppid()){
                    
                }
            }
        }
        break;
    
    case 1:
        if(hijos == getppid()){
            printf("c");
        }
        if(nietos[1] == getppid()){
            printf("f");
        }
        break;

    case 2:
        if(hijos == getppid()){
            read(fd[0][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]/n",getpid(),getppid(),post.letra);
            post.letra = 'd';
            write(fd[1][1],&post,sizeof(struct data));
        }else{
            if(nietos[2] == getppid()){
                read(fd[1][0],&post,sizeof(struct data));
                printf("Proceso[%d]  Padre[%d]   Letra[%c]/n",getpid(),getppid(),post.letra);
                post.letra = 'g';
                write(fd[2][1],&post,sizeof(struct data));
            }else{
                if(bisnietos[2] == getppid()){
                    read(fd[2][0],&post,sizeof(struct data));
                    printf("Proceso[%d]  Padre[%d]   Letra[%c]/n",getpid(),getppid(),post.letra);
                    post.letra='i';
                    write(fd[3][1],&post,sizeof(struct data));
                }
            }
        }

        break;
    
    default:
        post.letra ='a';
        write(fd[0][1],&post,sizeof(struct data));
        break;
    }

    for (int i = 0; i < 3; i++)
    {
        wait(NULL);
    }
    
    

    return EXIT_SUCCESS;
}