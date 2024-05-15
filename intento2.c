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
    int fd[13][2];

    for (int m = 0; m < 14; m++)
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
        if(hijos == getppid()){//b
            read(fd[8][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra='b';
            write(fd[9][1],&post,sizeof(struct data));

            read(fd[12][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra = 'b';
            write(fd[13][1],&post,sizeof(struct data));           
        }else{
            if(nietos[0] == getppid()){//e
                read(fd[9][0],&post,sizeof(struct data));
                printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                post.letra = 'e';
                write(fd[10][1],&post,sizeof(struct data));

                read(fd[11][0],&post,sizeof(struct data));
                printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                post.letra = 'e';
                write(fd[12][1],&post,sizeof(struct data));
            }else{
                if(bisnietos[0] == getppid()){//h
                    read(fd[10][0],&post,sizeof(struct data));
                    printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                    post.letra = 'h';
                    write(fd[11][1],&post,sizeof(struct data));
                }
            }
        }
        break;
    
    case 1:
        if(hijos == getppid()){//c
            read(fd[5][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra='c';
            write(fd[6][1],&post,sizeof(struct data));

            read(fd[7][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
        
            post.letra='c';
            write(fd[8][1],&post,sizeof(struct data));
        }
        if(nietos[1] == getppid()){//f
            read(fd[6][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra='f';
            write(fd[7][1],&post,sizeof(struct data));
        }
        break;

    case 2:
        if(hijos == getppid()){//d
            read(fd[0][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra = 'd';
            write(fd[1][1],&post,sizeof(struct data));

            read(fd[4][0],&post,sizeof(struct data));
            printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
            post.letra='d';
            write(fd[5][1],&post,sizeof(struct data));

        }else{
            if(nietos[2] == getppid()){//g
                read(fd[1][0],&post,sizeof(struct data));
                printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                post.letra = 'g';
                write(fd[2][1],&post,sizeof(struct data));

                read(fd[3][0],&post,sizeof(struct data));
                printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                post.letra='g';
                write(fd[4][1],&post,sizeof(struct data));

            }else{//i
                if(bisnietos[2] == getppid()){
                    read(fd[2][0],&post,sizeof(struct data));
                    printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);
                    post.letra='i';
                    write(fd[3][1],&post,sizeof(struct data));
                }
            }
        }

        break;
    
    default:
        post.letra ='a';
        write(fd[0][1],&post,sizeof(struct data));

        read(fd[13][0],&post,sizeof(struct data));
        printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);

        post.letra ='a';
        write(fd[14][1],&post,sizeof(struct data));
        break;
    }

    read(fd[14][0],&post,sizeof(struct data));
    printf("Proceso[%d]  Padre[%d]   Letra[%c]\n",getpid(),getppid(),post.letra);



    
    
    return EXIT_SUCCESS;
}