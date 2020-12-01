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

int count1 = 0;//vowel
int count2 = 0;//const
void * mem;
void process_word(char * word){
        switch(word[0]){
        case 'A': case 'E': case 'I': case 'O': case 'U':
        case 'a': case 'e': case 'i': case 'o': case 'u':
            {
                count1++; //Increment type 1, starts with vowel
                char p ='\0';
                int idx = 0;
                while(word[idx] != '\0'){
                    if(word[idx] == '.' || word[idx] == ','){
                        p = word[idx];
                        break;
                    }
                    idx++;
                }
                word[idx] = 'r';
                idx++;
                word[idx] = 'a';
                idx++;
                word[idx] = 'y';
                idx++;
                if(p != '\0'){
                    word[idx] = p;
                    idx++;
                }
                word[idx] = '\0';
                cout << "PROCESSED WORD : " << word << endl;
                break;
            }
            default:
                count2++; //Increment type 2, starts with vowel
                
                break;
        }
}
int main(int argc, char ** argv){

    sem_t* sem1;
    sem_t* sem2;
    int p1_r;
    int p2_w;
    int sm;
    char buff[BUFF_SIZE];
    if(argc != 6){
        cout << "ERROR : invalid number of arguments" << endl;
        return 1;
    }
    
    cout << "In Program 2" << endl;
   
    //Open sem1 and sem2 with id's provided from command line arguments
    char * semname1 = argv[1];
    sem1 = sem_open(semname1, O_CREAT, 0600, 1);
    if(sem1 == SEM_FAILED){
        cout << "ERROR : sem 1 in program 2" << endl;
        return 2;
    }
    char * semname2 = argv[2];
    sem2 = sem_open(semname2, O_CREAT, 0600, 1);

    if(sem2 == SEM_FAILED){
        cout << "ERROR : sem 2 in program 2" << endl;
        return 2;
    }
    //convert the command line args for the pipes back to ints that will be used as 
    //          the file descriptors
    char * pipe_num_as_string = argv[3];
    p1_r = atoi(pipe_num_as_string);
    pipe_num_as_string = argv[4];
    p2_w = atoi(pipe_num_as_string);

    char * sm_fd_as_string = argv[5];
    sm = shm_open(sm_fd_as_string, O_RDWR, 0666);
    mem = mmap(0, 9, PROT_WRITE, MAP_SHARED, sm, 0);
    
    int i;
    while(1){
        sem_getvalue(sem1, &i);
        while(i == 1){
            sem_getvalue(sem1, &i);
        }
        read(p1_r, &buff, BUFF_SIZE); // read word from pipe and store in buffer
        cout << "READING : " << buff << "\n";

        sem_wait(sem1);

        if(!strcmp (buff, "END")){
            break;
        }
        process_word(buff);
    }
    // sem_getvalue(sem2, &i);
    // while(i == 1){
    //     sem_getvalue(sem2, &i);
    // }
    // strcpy(buff, "END");
    // write(p2_w, buff, BUFF_SIZE);
    // sem_post(sem2);

    cout << "Leaving Program 2" << endl;
    sem_unlink(semname1);
    sem_close(sem1);
    sem_unlink(semname2);
    sem_close(sem2);

    close(p1_r);
    close(p2_w);
}