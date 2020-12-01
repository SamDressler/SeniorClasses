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
sem_t* sem1;
sem_t* sem2;
int p1_r;
int p2_w;
int sm_fd;
char buff[BUFF_SIZE];

void process_word(char []);

int main(int argc, char ** argv){

    if(argc != 6){
        cout << "ERROR : invalid number of arguments" << endl;
        return 1;
    }
    
    cout << "[Program 2] In Program 2" << endl;
    sleep(1);
   
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

    char * sm_key = argv[5];
    sm_fd = shm_open(sm_key, O_RDWR, 0666);
    mem = mmap(0, 9, PROT_WRITE, MAP_SHARED, sm_fd, 0);
    
    int i;
    while(1){
        sem_getvalue(sem1, &i);
        while(i == 1){
            sem_getvalue(sem1, &i);
        }
        read(p1_r, &buff, BUFF_SIZE); // read word from pipe and store in buffer


        sem_wait(sem1);

        if(!strcmp (buff, "END")){
            break;
        } 
        cout << "[Program 2] Reading from P1 : " << buff << "\n";
        process_word(buff);
    }
    sem_getvalue(sem2, &i);
    strcpy(buff, "END");
    write(p2_w, buff, BUFF_SIZE);
    sem_post(sem2);
    sprintf((char *)mem, "%d %d", count1, count2);

    cout << "[Program 2] Leaving Program 2" << endl;
    sem_unlink(semname1);
    sem_close(sem1);
    sem_unlink(semname2);
    sem_close(sem2);

    close(p1_r);
    close(p2_w);
}
void process_word(char word []){
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
                // cout << "PROCESSED WORD : " << word << endl;
                break;
            }
            default:
                count2++; //Increment type 2, starts with vowel
                char first_char = word[0];
                char punc = '\0';
                char temp [BUFF_SIZE];
                int last_char = strlen(word)-1;
                punc = word[last_char];
                int i = 0;
                int j = 1;
                for(i = 0 ; i < last_char; i ++){
                    temp[i] = word[j];
                    j++;
                }
                if(punc == ',' || punc == '.'){
                    temp[last_char-1] = first_char;
                    temp[last_char] = 'a';
                    temp[last_char+1] = 'y';
                    temp[last_char+2] = punc;
                    temp[last_char+3] = '\0';
                }
                else{
                    temp[last_char] = first_char;
                    temp[last_char+1] = 'a';
                    temp[last_char+2] = 'y';
                    temp[last_char+3] = '\0';
                }
                memset(word, 0, BUFF_SIZE);
                strcpy(word, temp);
                // cout << "PROCESSED WORD : " << word << endl;
                break;
        }
        int s = 0 ;
        sem_getvalue(sem2, &s);
        while(s == 2){
            sem_getvalue(sem2, &s);
        }
        strcpy(buff, word);
        write(p2_w, buff, BUFF_SIZE);
        cout << "[Program 2] Writing to P2 : " << buff << endl;
        sem_post(sem2);


}