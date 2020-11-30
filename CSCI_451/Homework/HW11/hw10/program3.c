#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int pip2_read;

int main(int argc, char** argv)
{
    if(argc > 1)
    {
        pip2_read = atoi(argv[1]);
    }

    FILE *vowl = fopen("shared1.dat", "r");
    if(!vowl)
    {
        printf("Unable to open file\n");
        exit(1);
    }
    int vowel_type;
    fscanf(vowl, "%d\n", &vowel_type);
    fclose(vowl);
    printf("Type 1: %d\n", vowel_type);

    FILE *cons = fopen("shared2.dat", "r");
    if(!cons)
    {
        printf("Unable to open file\n");
        exit(1);
    }
    int cons_type;
    fscanf(cons, "%d\n", &cons_type);
    fclose(cons);
    printf("Type 2: %d\n", cons_type);

    FILE *out = fopen("output.data", "w");
    char read_c;
    char read_str[100];
    int i = 0;
    while(1)
    {
        read(pip2_read, &read_c, 1);
        if(read_c == ' ')
        {
            read_str[i] = '\0';
            //printf("In child 3: %s\n", read_str);
            if(!strcmp(read_str, "EOF"))
            {
                break;
            }
            fprintf(out, "%s ", read_str);
            i = 0;
            continue;
        }
        read_str[i] = read_c;
        read_str[i + 1] = '\0';
        i++;
    }

    fclose(out);
    close(pip2_read);
    return 0;
}