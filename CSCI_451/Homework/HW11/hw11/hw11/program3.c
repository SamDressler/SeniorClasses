#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

int pip2_read;
sem_t *semaphore_boi2;
void *mem_ptr;

int main(int argc, char** argv)
{
    if(argc == 5)
    {
        semaphore_boi2 = sem_open(argv[1], 0);
        if(semaphore_boi2 == SEM_FAILED)
        {
            printf("Program 3: Unable to open second semaphore\n");
            exit(1);
        }
        pip2_read = atoi(argv[3]);
        int shm_fd = shm_open(argv[4], O_RDONLY, 0666);
        mem_ptr = mmap(0, 8, PROT_READ, MAP_SHARED, shm_fd, 0);
    }
    else 
    {
        printf("Program 3: Incorrect number of arguments supplied - %d supplied\n", argc);
        exit(1);
    }

    FILE *out = fopen(argv[2], "w");

    char read_c;
    char read_str[100];
    int i = 0;
    while(1)
    {   
        //sem_wait(semaphore_boi1);
        sem_getvalue(semaphore_boi2, &i);
        //printf("Program 2: Found semaphore value %d\n", i);
        //printf("Program 2: Want 2 to continue\n");
        while(i == 1)
        {
            sem_getvalue(semaphore_boi2, &i);
            //printf("Program 2: Found semaphore value %d\n", i);
            //printf("Program 2: Want 2 to continue\n");
        }
        read(pip2_read, &read_c, 1);
        i = 0;
        while(read_c != '`')
        {
            read_str[i] = read_c;
            read(pip2_read, &read_c, 1);
            i++;
        }
        read_str[i] = '\0';
        
        sem_wait(semaphore_boi2);
        
        if(!strcmp(read_str, "EOF"))
        {
            break;
        }
        fprintf(out, "%s ", read_str);
    }

    fclose(out);
    close(pip2_read);

    //printf("Program 3: About to work with shared memory\n");
    char *memory, *token;
    memory = malloc(sizeof(char) * 9);
    memcpy(memory, (char*)mem_ptr, 9);

    //printf("Program 3: Complete step 1 - copy shared mem");
    token = strtok(memory, " ");
    printf("[Program 3] Type 1: %d\n", atoi(token));

    token = strtok(NULL, " ");
    printf("[Program 3] Type 2: %d\n", atoi(token));

    close(pip2_read);
    sem_close(semaphore_boi2);
    sem_unlink(argv[1]);
    shm_unlink(argv[4]);
    return 0;
}