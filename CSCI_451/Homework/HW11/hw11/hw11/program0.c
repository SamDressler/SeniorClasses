/*
    John Neis
    j.neis@und.edu
    ID: 8003486

    Homework 11
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

sem_t *semaphore_boi1, *semaphore_boi2;
int pip1[2];
int pip2[2];
int shm_fd;

int main(int argc, char** argv)
{
    sem_unlink("/prog0_sem_1");
    sem_unlink("/prog0_sem_2");
    shm_unlink("prog23_mem");

    if(argc != 3)
    {
        printf("Usage: ./program0 <input filename> <output filename>\n");
        exit(EXIT_FAILURE);
    }
    
    semaphore_boi1 = sem_open("/prog0_sem_1", O_CREAT, 0600, 1);
    if(semaphore_boi1 == SEM_FAILED)
    {
        sem_unlink("/prog0_sem_1");
        printf("Unable to open first semaphore!\n");
        exit(1);
    }

    semaphore_boi2 = sem_open("/prog0_sem_2", O_CREAT, 0600, 1);
    if(semaphore_boi2 == SEM_FAILED)
    {
        sem_unlink("/prog0_sem_2");
        printf("Unable to open second semaphore!\n");
        exit(1);
    }

    sem_close(semaphore_boi1);
    sem_close(semaphore_boi2);

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

    shm_fd = shm_open("prog23_mem", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 9);

    char* args[3][7] = {
        // prog name , in file, sem 1 name    , pipe 1
        {"./program1", argv[1], "/prog0_sem_1", pip1_1, NULL, NULL, NULL},
        // prog name , sem 1 name    , sem 2 name    , pipe 1, pipe 2, shared mem id
        {"./program2", "/prog0_sem_1", "/prog0_sem_2", pip1_0, pip2_1, "prog23_mem", NULL},
        // prog name , sem 2 name    , outfile, pipe 2, shared mem id
        {"./program3", "/prog0_sem_2", argv[2], pip2_0, "prog23_mem", NULL, NULL}
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
        close(pip2[0]);
        close(pip2[1]);
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
        close(pip1[1]);
        close(pip2[0]);
        execv(args[1][0], args[1]);
    }
    //printf("PARENT: Forked off child PID: %ld\n", (long)pids[1]);

    if((pids[2] = fork()) < 0)
    {
        perror("fork 3");
        exit(1);
    }
    if(pids[2] == 0)
    {
        close(pip1[0]);
        close(pip1[1]);
        close(pip2[1]);
        execv(args[2][0], args[2]);
    }

    int status;
    waitpid(pids[0], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[0], status);
    waitpid(pids[1], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[1], status);
    waitpid(pids[2], &status, 0);
    //printf("Child with PID %ld, exited with status %d\n", (long)pids[2], status);

    sem_unlink("/prog0_sem_2");
    
    return 0;
}