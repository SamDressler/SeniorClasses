#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 25

using namespace std;

int main(int argc, char ** argv){
    sem_t * sem1;
    int p1_w;
    char buff [BUFF_SIZE];
    if(argc != 4){
        cout << "ERROR : invalid number of arguments" << endl;
        return 1;
    }

    cout << "In Program 1" << endl;
    //Get the filename from command line args
    const char * filename = argv[2];
    FILE *fin = fopen(filename, "r");
    if(!fin){
        cout << "ERROR : File unable to be opened" << endl;
        return 2;
    }
    //Get the semname from command line args
    const char * semname = argv[1];
    // sem1 = sem_open(semname, O_CREAT, 0600, 0);
    sem1 = sem_open(semname, 0);
    if(sem1 == SEM_FAILED){
        cout << "ERROR : sem1 in program 1" << endl;
        return 3;
    }
    //get pipe 1's write identifier from command line args
    char * pipe_num_as_string = argv[3];
    p1_w = atoi(pipe_num_as_string);

    char tempc;
    string tempw;
    int i = 0;
    while(fscanf(fin, "%c", &tempc) != EOF){
        if(tempc == ' '){
            //If the current word is longer than zero
            if(tempw.length() > 0){
                sem_getvalue(sem1, &i);
                while(i == 2){
                    sem_getvalue(sem1, &i);
                }
                const char * word = tempw.c_str();
                strncpy(buff, word, tempw.length()); //copy word to write buffer
                write(p1_w, &buff, BUFF_SIZE); //write word from file to pipe 1
                cout << "WRITING : " << buff << endl;
                memset(buff, 0, BUFF_SIZE); // clear buffer
                sem_post(sem1);
            }
            tempw.clear();
        }
        else{
            tempw += tempc;
        }
    }
    sem_getvalue(sem1, &i);
    while(i == 2)
    {sem_getvalue(sem1, &i);}

    strcpy(buff, "END");
    write(p1_w, &buff, BUFF_SIZE);
    cout << "WRITING : END" << endl;
    sem_post(sem1);

    cout << "Leaving Program 1" << endl;
    sem_unlink(semname);
    sem_close(sem1);
    close(p1_w);
    fclose(fin);
}