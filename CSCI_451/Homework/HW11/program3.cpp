#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BUFF_SIZE 25

using namespace std;

sem_t *sem2;
int p2_r;
int sm_fd;
void * mem;
char buff [BUFF_SIZE];
int main(int argc, char ** argv){

    if(argc != 5){
        cout << "ERROR : invalid number of arguments" << endl;
        return 1;
    }

    cout << "[Program 3] In Program 3" << endl;
    //open semaphore from command line args
    char * semname = argv[1];
    sem2 = sem_open(semname, O_CREAT, 0600, 1);
    if(sem2 == SEM_FAILED){
        cout << "ERROR : Sem 2 in Program 3" << endl;
        return 2;
    }

    //Open output file from command line args
    char * fileout = argv[2];
    FILE * fout = fopen(fileout, "w");
    if(!fout){
        cout << "ERROR : Output file could not be opened" << endl;
        return 3;
    }

    //open pipe 2 read end from command line args
    char * pipe_read_end_as_string = argv[3];
    p2_r = atoi(pipe_read_end_as_string);

    //Open shared mem
    char * sm_key = argv[4];
    sm_fd = shm_open(sm_key, O_RDONLY, 0666);
    mem = mmap(0, 9, PROT_READ, MAP_SHARED, sm_fd, 0);
    int s = 0;
    while(1){
        sem_getvalue(sem2, &s);
        while(s == 1){
            sem_getvalue(sem2, &s);
        }
        read(p2_r, buff, BUFF_SIZE);
        

        if(!strcmp (buff, "END")){
            break;
        }
        cout << "[Program 3] Reading from P2 : " << buff << endl;
        cout << "------------------------------------------" << endl;
        fprintf(fout, "%s ",buff);
        sem_wait(sem2);
    }
    close(p2_r);
    fclose(fout);


    //Read values from shared mem
    char *m;
    char *tok;
    m = (char *) malloc(sizeof(char) * 9);
    memcpy(m, (char*)mem, 9);

    //printf("Program 3: Complete step 1 - copy shared mem");
    cout << "------------------------------------------" << endl;
    tok = strtok(m, " ");
    cout << "[Program 3] (Vowel) Type 1: " << atoi(tok) << endl;
    tok = strtok(NULL, " ");
    cout << "[Program 3] (Const) Type 2: " << atoi(tok) << endl;
    
    cout << "------------------------------------------" << endl;
    cout << "[Program 3] Leaving Program 3" << endl;
    sem_unlink(semname);
    sem_close(sem2);
    shm_unlink(sm_key);
}