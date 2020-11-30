#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

sem_t *semaphore_boi;
int pip1_read, pip2_write;
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
        dprintf(pip2_write, "%s ", word);
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
        dprintf(pip2_write, "%s ", word);
        return;
    }
    
}

int main(int argc, char** argv)
{
    vowel_type = 0; cons_type = 0;
    if(argc > 1)
    {
        semaphore_boi = sem_open(argv[1], 0);
        if(semaphore_boi == SEM_FAILED)
        {
            printf("Unable to open semaphore\n");
            exit(1);
        }

        
        pip1_read = atoi(argv[2]);
        pip2_write = atoi(argv[3]);
    }
    else
    {
        printf("Incorrect number of arguments supplied\n");
        exit(1);
    }

    char read_c;
    char read_str[100];
    int i = 0;
    while(1)
    {   
        sem_wait(semaphore_boi);
        read(pip1_read, &read_c, 1);
        //scanf("%s", read_str);
        sem_post(semaphore_boi);
        
        if(read_c == ' ')
        {
            read_str[i] = '\0';
            //printf("In child 2: %s\n", read_str);
            if(!strcmp(read_str, "EOF"))
            {
                break;
            }
            convert_word(read_str);
            i = 0;
            continue;
        }
        read_str[i] = read_c;
        read_str[i + 1] = '\0';
        i++;
    }
    
    FILE *vowl = fopen("shared1.dat", "w");
    fprintf(vowl, "%d\n", vowel_type);
    fclose(vowl);

    FILE *cons = fopen("shared2.dat", "w");
    fprintf(cons, "%d\n", cons_type);
    fclose(cons);

    dprintf(pip2_write, "EOF ");
    close(pip2_write);
    return 0;
}