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
    if(argc > 1)
    {
        semaphore_boi = sem_open(argv[1], 0);
        if(semaphore_boi == SEM_FAILED)
        {
            printf("Unable to open semaphore\n");
            exit(1);
        }

        pip1_write = atoi(argv[2]);
    }
    else
    {
        printf("Incorrect number of arguments supplied\n");
        exit(1);
    }
    
    FILE *fptr = fopen("input.data", "r");
    if(!fptr)
    {
        printf("Unable to open file\n.");
        exit(1);
    }

    char input_str[20];
    while(1)
    {
        fscanf(fptr, "%s", input_str);
        
        sem_wait(semaphore_boi);
        dprintf(pip1_write, "%s ", input_str);
        //printf("C1: %s\n", input_str);
        sem_post(semaphore_boi);

        if(feof(fptr)) break;
    }

    fclose(fptr);

    sem_wait(semaphore_boi);
    dprintf(pip1_write, "EOF ");
    sem_post(semaphore_boi);

    return 0;
}