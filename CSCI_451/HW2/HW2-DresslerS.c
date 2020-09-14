//Sam Dressler
//HW 2
//CSCI 451
//9/14/2020

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include <sys/wait.h>
#include <sys/types.h>


int main(){
    /*
    create the args for execv, this array contains the executable (itself)
    that will cause the fork bomb to occur
    */
    char * args[] = {"./a.out", NULL};
    pid_t pid;
    int exitCode = 1;
    printf("Spawning child.\n");
    //Fork the parent process    
    pid = fork();
    if(pid == 0){
        execv(args[0], args);
        printf("Child : Exit Code : %d\n", exitCode);
        exit(exitCode);
    } 
    else
    {
        printf("Parent: Spawning child PID: %d\n",pid);
        system("xeyes");
        waitpid(pid, &exitCode, 0);
        printf("Parent: child exit code: %d\n", (exitCode >> 8));
      }
    
}
