#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    
    char buf;
    int p[2];
    int pp[2];
    int pid;
    int n;
    char byte = 'a';

    //pipe error checking
    if (pipe(p) == -1) {
        fprintf(2, "Pipe: unable to create pipe p...");
        exit(1);
    }
    if (pipe(pp) == -1) {
        fprintf(2, "Pipe: unable to create pipe pp...");
        exit(1);
    }    
    
    pid = fork();
    //fork error checking
    if (pid < 0) {
        fprintf(2, "Fork: unable to fork...");
        exit(1);
    }
    
    if ( pid == 0) {
        //in child process

        //receive from parent
        close(p[1]);
        n = read(p[0], &buf, sizeof(char));
        int childPid = getpid();
        if (n < 0) {
            fprintf(2, "Read: unable to read from pipe p...");
            exit(1);
        } else if (n > 0) {
            printf("%d: received ping\n", childPid);
        }
        
        //send to parent
        close(pp[0]);
        if (write(pp[1], &byte, sizeof(char)) != sizeof(char)) {
            //write error checking
            fprintf(2, "Write: unable to write...");
            exit(1);
        }
        close(pp[1]);
        


        exit(0);
    }
    //send to child
    close(p[0]);
    if (write(p[1], &byte, sizeof(char)) != sizeof(char)) {
        //write error checking
        fprintf(2, "Write: unable to write...");
        exit(1);
    }
    close(p[1]);

    //receive from child
    wait(0);
    int parentPid = getpid();
    n = read(pp[0], &buf, sizeof(char));
    if (n < 0) {
        fprintf(2, "Read: unable to read from pipe pp...");
        exit(1);            
    } else if (n > 0) {
        printf("%d: received pong\n", parentPid);
    }
    

    exit(0);
}