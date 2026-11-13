#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/times.h>
#include <errno.h>

int main()
{    
    int somma = 0;
    int num = -1;
    printf("quanti numeri vuoi inserire? \n");
    scanf("%d", &num);
    if (num<0 || num>10) {
        printf("errore");
        return -1;
    }
    
    printf("[padre - livello 0] %d\n", getpid());
    for (int i = 0; i<num; i++) {
        pid_t pid = fork();
        if(pid <0) {
            perror("errore fork");
            exit(-1);
        }
        if(pid == 0) {
            printf("[figlio - livello %d] pid %d, padre %d\n",i+1, getpid(), getppid());
            exit(i+1);
        } else {
            int status; //ritorno del figlio
            wait(&status);
            if (WIFEXITED(status) == -1) {
                printf("errore");
                exit(-1);
            }
            int numero = WEXITSTATUS(status); //converte status in numero
            somma+=numero;
            printf("[padre] figlio terminato con codice %d\n", numero);
        }
    }
    
    printf("[padre] somma dei codici %d\n", somma);
    
    return 0;
}
