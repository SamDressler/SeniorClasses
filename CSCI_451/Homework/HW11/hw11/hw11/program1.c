#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

sem_t *semaphore_boi;
int pip1_write;

int main(int argc, char** argv)
{
    if(argc == 4)
    {
        semaphore_boi = sem_open(argv[2], 0);
        if(semaphore_boi == SEM_FAILED)
        {
            printf("Program 1: Unable to open semaphore\n");
            exit(1);
        }

        pip1_write = atoi(argv[3]);
        int i = 0;
        sem_getvalue(semaphore_boi, &i);
        //printf("Program 1: Semaphore 1 initial value is %d\n", i);
        //exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Program 1: Incorrect number of arguments supplied - %d supplied\n", argc);
        exit(1);
    }
    
    FILE *fptr = fopen(argv[1], "r");
    if(!fptr)
    {
        printf("Unable to open file\n.");
        exit(1);
    }

    char input_str[20];
    int i = 0;
    while(1)
    {
        fscanf(fptr, "%s", input_str);
        sem_getvalue(semaphore_boi, &i);
        //printf("Program 1: Found semaphore value %d\n", i);
        //printf("Program 1: Want 1 to continue\n");
        while(i == 2)
        {
            sem_getvalue(semaphore_boi, &i);
            //printf("Program 1: Found semaphore value %d\n", i);
            //printf("Program 1: Want 1 to continue\n");
        }
        // printf("Program 1: Printing \'%s\' to pipe. Length is %ld\n", input_str, strlen(input_str));
        dprintf(pip1_write, "%s`", input_str);
        sem_post(semaphore_boi);

        if(feof(fptr)) break;
    }

    fclose(fptr);

    sem_getvalue(semaphore_boi, &i);
    //printf("Program 1: Found semaphore value %d\n", i);
    //printf("Program 1: Want 1 to continue\n");
    while(i == 2)
    {
        sem_getvalue(semaphore_boi, &i);
        //printf("Program 1: Found semaphore value %d\n", i);
        //printf("Program 1: Want 1 to continue\n");
    }
    //printf("Program 1: Printing \'EOF\' to pipe. Length is %ld\n", strlen("EOF"));
    dprintf(pip1_write, "EOF`");
    sem_post(semaphore_boi);

    sem_close(semaphore_boi);
    close(pip1_write);

    return 0;
}