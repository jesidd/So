#include <stdio.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void sighandler(int sig){
    printf("[%d]\n", getpid());
}


int main(){

    int i = 0, h0 = 0, h1 = 0, h2= 0,   hn0 = 0, hn2 = 0;
    __sighandler_t s = signal(SIGUSR1, sighandler);

    for (i; i < 3; i++)
    {
        if (!fork()){
            if(i == 0){
                hn0 = fork();
            }
            if(i == 2){
                hn2 = fork();
            }

            if (i == 0) h0 = getegid(); 
            else{
                if (i == 1) h1 = getegid();
                else if (i == 2) h2 = getegid();
            }

            break;
        }
    }    

    switch (i)
    {
    case 0:
        if(h2 != 0){s;  kill(hn2,SIGUSR1);}
        if(hn2 != 0){ s; kill(h2,SIGUSR1);}
        break;
    
    default:
        //printf("padre\n");
        usleep(100);
        kill(h2,SIGUSR1);
        break;
    }

    for ( int j = 0 ; j < 3; i++) wait(NULL);

}