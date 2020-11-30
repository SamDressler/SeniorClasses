#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

sem_t sem1;
int p1_w;

int main(int argc, char ** argv){
    if(argc < 3){
        cout << "ERROR : invalid number of arguments" << endl;
        return 1;
    }
    else{
        cout << "yes 1" << endl;
        sleep(2);
    }
}