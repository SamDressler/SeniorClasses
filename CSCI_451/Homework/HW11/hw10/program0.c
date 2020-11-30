/*
    John Neis
    j.neis@und.edu
    ID: 8003486

    Homework 10
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

sem_t *semaphore_boi;
int pip1[2];
int pip2[2];

int main()
{
    sem_unlink("/prog0_sem");
    semaphore_boi = sem_open("/prog0_sem", O_CREAT, 0600, 1);
    if(semaphore_boi == SEM_FAILED)
    {
        printf("Unable to open semaphore!\n");
        exit(1);
    }
    // sem_close(semaphore_boi);
    
    if(pipe(pip1) == -1)
    {
        printf("Pipe 1 failed!\n");
        exit(1);
    }
    
    if(pipe(pip2) == -1)
    {
        printf("Pipe 2 failed!\n");
        exit(1);
    }

    char pip1_0[10], pip1_1[10], pip2_0[10], pip2_1[10];

    sprintf(pip1_0, "%d", pip1[0]);
    sprintf(pip1_1, "%d", pip1[1]);

    sprintf(pip2_0, "%d", pip2[0]);
    sprintf(pip2_1, "%d", pip2[1]);

    char* args[3][5] = {
        {"./program1", "/prog0_sem", pip1_1, NULL, NULL},
        {"./program2", "/prog0_sem", pip1_0, pip2_1, NULL},
        {"./program3", pip2_0, NULL, NULL, NULL}
    };

    pid_t pids[3];
    int i = 0;

    if((pids[0] = fork()) < 0)
    {
        perror("fork 1");
        exit(1);
    }
    else if(pids[0] == 0)
    {
        close(pip1[0]);
        //char* args[4] = {"program1", }
        execv(args[0][0], args[0]);
        perror("Child 1 execv failed\n");
    }
    //printf("PARENT: Forked off child PID: %ld\n", (long)pids[0]);

    if((pids[1] = fork()) < 0)
    {
        perror("fork 2");
        exit(1);
    }
    else if(pids[1] == 0)
    {
        //close(pip1[1]);
        //dup2(pip1[0], 0);
        close(pip1[1]);
        close(pip2[0]);
        execv(args[1][0], args[1]);
    }
    //printf("PARENT: Forked off child PID: %ld\n", (long)pids[1]);

    close(pip1[0]);
    close(pip1[1]);

    int status;
    waitpid(pids[0], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[0], status);
    waitpid(pids[1], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[1], status);

    if((pids[2] = fork()) < 0)
    {
        perror("fork 3");
        exit(1);
    }
    if(pids[2] == 0)
    {
        execv(args[2][0], args[2]);
    }

    waitpid(pids[2], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[2], status);
    sem_unlink("/prog0_sem");
    
    return 0;
}