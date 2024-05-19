#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


//Thoughts: write handles byte stream, not support int 
//Thoughts: its hard to arrange when to fork and when to create pipe 
//Thoughts: its hard to decide which end of the pipe to close
//Thoughts: and also hard for a recursive function with multi-process

//Remember: always check the return value of write and read and do forth, it hrlps a lot when debugging

//child process and grandchild process
void recursive(int* p) {
    close(p[1]); // inherit from parent
    char buf;
    int cnt = read(p[0], &buf, sizeof(char));
    if (cnt < 0) {
        fprintf(2, "Read error...\n");
        exit(1);
    }
    if (cnt == 0) {
        return;
    }
    
    int prime = buf - '0';
    printf("prime %d\n",prime);
    
    int pp[2]; //pipe for child and grandchild
    pipe(pp);    
    
    int pid = fork();

    if (pid > 0) {
        //in child process
        close(pp[0]);
        while (read(p[0], &buf, sizeof(char)) > 0) {
            int number = buf - '0';
            if (number % prime != 0) {
                int n = write(pp[1], &buf, sizeof(char));
                if (n < 0) {
                    fprintf(2, "Write error...\n");
                    exit(1);
                }
            }
        }
        close(pp[1]);//must close
        close(p[0]);
        wait(0);//wait for grandchild to exit
        return;

    } else if (pid == 0) {
        // in grandchild process
        recursive(pp);
        exit(0);
    } else {
        fprintf(2, "Fork: fail to fork...\n");
        exit(1);
    }

}


int main (void) {

    //pipe for current process
    int p[2];
    
    if (pipe(p) < 0) {
        fprintf(2, "Pipe: unable to create pipe p...\n");
        exit(1);
    }

    int pid = fork();

    if (pid < 0) {
        fprintf(2, "Fork: unable to fork...\n");
        exit(1);       
    } 

    if (pid == 0) {
        //in child process 
       recursive(p);  
       exit(0);  
    }

    //write number into pipe
    int i;
    for (i = 2; i < 36; i++) {
        close(p[0]);
        
        char ch = i + '0';

        if (write(p[1], &ch, sizeof(char)) < 0) {
            fprintf(2, "Write error\n");
            exit(1);
        }

    
        //if i == 35 wait for all children return
        if (i == 35) {
            close(p[1]); // must close
            wait(0); //have to wait all child process
        }

    }
    exit(0);
    

}