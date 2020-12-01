#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

sem_t *semaphore_boi1, *semaphore_boi2;
int pip1_read, pip2_write;
void *mem_ptr;
int vowel_type, cons_type;

void convert_word(char word[])
{
    char c = word[0];
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
       c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
    {
        //printf("In C2: Before conversion  %s\n", word);
        vowel_type++;
        int i = 0;
        char punc = '\0';
        while(word[i] != '\0')
        {
            if(word[i] == '.' || word[i] == ',')
            {
                punc = word[i];
                break;
            }
            i++;
        }

        word[i] = 'r'; i++;
        word[i] = 'a'; i++;
        word[i] = 'y'; i++;
        if(punc != '\0')
        {
            word[i] = punc; i++;
        }
        word[i] = '\0';

        i = 0;
        sem_getvalue(semaphore_boi2, &i);
        while(i == 2)
        {
            sem_getvalue(semaphore_boi2, &i);
        }
        //printf("Program 2: Printing \'%s\' to pipe. Length is %ld\n", word, strlen(word));
        dprintf(pip2_write, "%s`", word);
        sem_post(semaphore_boi2);

        //dprintf(pip2_write, "%s ", word);
        return;
    }
    else
    {
        cons_type++;
        char first = word[0];
        int i = 0;
        while(word[i + 1] != '\0')
        {
            word[i] = word[i + 1];
            i++;
        }
        char punc = '\0';
        if(word[i] == '.' || word[i] == ',')
        {
            punc = word[i];
            i--;
        }
        //i--;   
        word[i] = first; i++;
        word[i] = 'a'; i++;
        word[i] = 'y'; i++;
        
        if(punc != '\0')
        {
            word[i] = punc; i++;
        }
        word[i] = '\0';

        i = 0;
        sem_getvalue(semaphore_boi2, &i);
        while(i == 2)
        {
            sem_getvalue(semaphore_boi2, &i);
        }
        //printf("Program 2: Printing \'%s\' to pipe. Length is %ld\n", word, strlen(word));
        dprintf(pip2_write, "%s`", word);
        sem_post(semaphore_boi2);

        //dprintf(pip2_write, "%s ", word);
        return;
    }
    
}

int main(int argc, char** argv)
{
    vowel_type = 0; cons_type = 0;
    if(argc == 6)
    {
        semaphore_boi1 = sem_open(argv[1], 0);
        if(semaphore_boi1 == SEM_FAILED)
        {
            printf("Program 2: Unable to open first semaphore\n");
            exit(1);
        }

        semaphore_boi2 = sem_open(argv[2], 0);
        if(semaphore_boi2 == SEM_FAILED)
        {
            printf("Program 2: Unable to open second semaphore\n");
            exit(1);
        }
        
        pip1_read = atoi(argv[3]);
        pip2_write = atoi(argv[4]);

        int shm_fd = shm_open(argv[5], O_RDWR, 0666);
        mem_ptr = mmap(0, 9, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    }
    else
    {
        printf("Program 2: Incorrect number of arguments supplied - %d supplied\n", argc);
        exit(1);
    }

    char read_c;
    char read_str[100];
    int i = 0;
    while(1)
    {   
        //sem_wait(semaphore_boi1);
        sem_getvalue(semaphore_boi1, &i);
        //printf("Program 2: Found semaphore value %d\n", i);
        //printf("Program 2: Want 2 to continue\n");
        while(i == 1)
        {
            sem_getvalue(semaphore_boi1, &i);
            //printf("Program 2: Found semaphore value %d\n", i);
            //printf("Program 2: Want 2 to continue\n");
        }
        read(pip1_read, &read_c, 1);
        int j = 0;
        while(read_c != '`')
        {
            read_str[j] = read_c;
            read(pip1_read, &read_c, 1);
            j++;
        }
        read_str[j] = '\0';
        printf("Program 2: Read \'%s\' from pipe. Length is %ld\n", read_str, strlen(read_str));
        //scanf("%s", read_str);
        sem_wait(semaphore_boi1);
        
        if(!strcmp(read_str, "EOF"))
        {
            break;
        }
        convert_word(read_str);
    }

    sem_getvalue(semaphore_boi2, &i);
    while(i == 2)
    {
        sem_getvalue(semaphore_boi2, &i);
    }
    dprintf(pip2_write, "EOF`");
    sprintf(mem_ptr, "%d %d", vowel_type, cons_type);
    sem_post(semaphore_boi2);

    sem_close(semaphore_boi1);
    sem_close(semaphore_boi2);

    sem_unlink(argv[1]);
    close(pip1_read);
    close(pip2_write);
    return 0;
}