/**
 * Sam Dressler
 * CSCI 451 HW 11
 * 11/29/2020
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>
using namespace std;

sem_t * sem1;
sem_t * sem2;
//1 - write
//0 - read
int p1[2];
int p2[2];

int main(int argc, char ** argv){

    sem_unlink("/sem1");
    sem_unlink("/sem2");
    sem1 = sem_open("/sem1", O_CREAT, 0600, 1);
    sem2 = sem_open("/sem2", O_CREAT, 0600, 1);

    if(sem1 == SEM_FAILED || sem2 == SEM_FAILED){
        cout << "ERROR: semaphores failed to open" << endl;
        return 1;
    }
    if(pipe(p1) == -1)
    {
        cout << "Pipe 1 failed" << endl;
        return 2;
    }
    if(pipe(p2) == -1)
    {
        cout << "Pipe 2 failed" << endl;
        return 3;
    }
    char p1_r[5];
    char p1_w[5];
    char p2_r[5];
    char p2_w[5];

    sprintf(p1_r, "%d", p1[0]);
    sprintf(p1_w, "%d", p1[1]);
    sprintf(p2_r, "%d", p2[0]);
    sprintf(p2_w, "%d", p2[1]);

    char const* prog1_args[4] = {"./program1", "/sem1", p1_w, NULL};
    char const* prog2_args[6] = {"./program2", "/sem1", "/sem2", p1_r, p2_w, NULL};
    char const* prog3_args[5] = {"./program3", "/sem2", p2_r, NULL};

    pid_t procs[3];

    if((procs[0] = fork())< 0){
        cout << "ERROR : fork 1" << endl;
        return 4;
    }
    else if(procs[0] == 0){
        close(p1[0]); // close read
        close(p2[1]);
        close(p2[0]);
        cout << prog1_args[0]<<" " << prog1_args[1] <<" "<< prog1_args[2] << endl;
        cout << "Starting program 1" << endl;
        execv(prog1_args[0], (char * const*)prog1_args);
    }
    
    if((procs[1] = fork()) < 0){
        cout << "ERROR : fork 2" << endl;
        return 5;
    }
    else if(procs[1] == 0){
        cout << prog2_args[0]<<" " << prog2_args[1] <<" "<< prog2_args[2] << endl;
        cout << "Starting program 2" << endl;
        execv(prog2_args[0], (char * const*)prog2_args);
    }

    while(wait(NULL) != -1 || errno != ECHILD){
        ;
    }
        cout << "------------------------------------------" << endl;
        cout << "Programs 1 and 2 have completed!" << endl;
        cout << "------------------------------------------" << endl; 
    if((procs[2] = fork())<0){
        cout << "ERROR : fork " << endl;
        return 6;
    }
    else if(procs[2] == 0){
        cout << prog2_args[0]<<" " << prog2_args[1] <<" "<< prog2_args[2] << endl;
        cout << "Starting program 3" << endl;
        execv(prog3_args[0], (char* const*)prog3_args);
    }
    while(wait(NULL) != -1 || errno != ECHILD){
        ;
    }
    cout << "Parent Process finished executing" << endl;
    cout << "------------------------------------------" << endl; 
    close(p1[0]);
    close(p1[1]);
    close(p2[0]);
    close(p2[1]);
    sem_close(sem1);
    sem_close(sem2);
    sem_unlink("/sem0");
    sem_unlink("/sem1");
    return 0; // program ends here 

}