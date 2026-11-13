#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/times.h>
#include <errno.h>

int main()
{    
    int fd1 [2];
    int fd2 [2];
    
    if(pipe(fd1) == -1) {
        perror("errore");
        exit(-1);
    }
    if(pipe(fd2) == -1) {
        perror("errore");
        exit(-1);
    }
    
    int numeri[5];
    for (int i = 0; i<5; i++) {
        printf("inserisci il numero %d\n", i+1);
        scanf("%d", &numeri[i]);
    }
    
    pid_t pid = fork();
    if(pid<0) {
        perror("errore fork");
        exit(-1);
    }
    if(pid == 0) {
        close (fd1[1]); //1 scrittura
        close (fd2[0]); //0 lettura
        
        int nuovo[5];
        int pari = 0;
        int dispari = 0;
        read(fd1[0], nuovo, sizeof(nuovo));
        for(int i = 0; i<5; i++) {
            if (nuovo[i]%2==0) {
                pari++;
            } else {
                dispari++;
            }
        }
        write(fd2[1], &pari, sizeof(pari));
        write(fd2[1], &dispari, sizeof(dispari));
        
        close (fd1[0]);
        close (fd2[1]);
        
        exit(0);
    } else {
        close (fd1[0]);
        close (fd2[1]);
        
        write(fd1[1], numeri, sizeof(numeri));
        
        int pari,dispari;
        read(fd2[0], &pari, sizeof(pari));
        read(fd2[0], &dispari, sizeof(dispari));
        
        wait(NULL);
        
        printf("numeri pari trovati = %d\n", pari);
        printf("numeri dispari trovati = %d\n", dispari);
        
        close (fd1[1]);
        close (fd2[0]);
    }
    
    return 0;
}
