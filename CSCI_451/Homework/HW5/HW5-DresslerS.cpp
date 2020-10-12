//Sam Dressler
// HW 5
// CSCI 451
// 9/30/2020

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <pthread.h>
#include <mutex>

//DEFINES
#define NUM_THREADS 3

//Namespace
using namespace std;

//Function definitions
void * thread1(void * arg);
void * thread2(void * arg);
void * thread3(void * arg);
void write_char(FILE *);
void get_next_char(FILE *, int);
//Structures
//struct to help with passing the out file to the function
struct thread_data {
    FILE * out_file;
};

//Variable Definitions
//Value of shared resource initially.
char resource = '\0';
bool print;
bool finished1 = false;
bool finished2 = false;
bool finished3 = false;;
pthread_mutex_t m_write = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c_write = PTHREAD_COND_INITIALIZER;
pthread_cond_t c1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t c3 = PTHREAD_COND_INITIALIZER;

//In this program the main() thread will be the consumer and 
//the three thread functions will be the producers
int main(){
    cout <<"---------------------------------"<<endl;
    cout << "Opening File for Output..."<<endl;
    cout <<"---------------------------------"<<endl;
    FILE * outfile = fopen("hw5.out","w");
    pthread_mutex_init(&m_write, NULL);
    pthread_mutex_init(&m1, NULL);
    pthread_mutex_init(&m2, NULL);
    pthread_mutex_init(&m3, NULL);
    pthread_cond_init(&c_write, NULL);
    pthread_cond_init(&c1, NULL);
    pthread_cond_init(&c2, NULL);
    pthread_cond_init(&c3, NULL);
    
    pthread_t threads [NUM_THREADS];
    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL); 
    
    while(true){
        //While not finished1, finished2, nor finished3
        if(!finished1 || !finished2 || !finished3){
            //Unlock thread 1 to read character
            pthread_mutex_unlock(&m1);
            pthread_cond_signal(&c1);
            pthread_cond_wait(&c_write,&m_write);
            if(print){
                write_char(outfile);
            }
            
            //Unlock thread 2 to read character
            pthread_mutex_unlock(&m2);
            pthread_cond_signal(&c2);
            pthread_cond_wait(&c_write, &m_write);
            if(print){
                write_char(outfile);
            }

            //Unlock thread 3 to read character
            pthread_mutex_unlock(&m3);
            pthread_cond_signal(&c3);
            pthread_cond_wait(&c_write, &m_write);
            if(print){
                write_char(outfile);
            }
        }
        else{
            cout << "All threads have finished" << endl;
            break;
        }
    }
}
//Thread to read from the hw5-1.in
void * thread1(void * arg){
    pthread_cond_wait(&c1, &m1);
    int thread_num = 1;
    cout <<"---------------------------------"<<endl;
    cout << "Opening File 1 for Input..."<<endl;
    cout <<"---------------------------------"<<endl;
    FILE * filein1 = fopen("hw5-1.in","r");
    
    if(filein1){
        do{
            get_next_char(filein1, thread_num);
        }while(!finished1);
    }
    else{
        cout << "Error: File 1 could not be opened"<<endl;
        exit(0);
    }
}

//Thread to read from hw5-2.in
void * thread2(void * arg){
    pthread_cond_wait(&c2, &m2);
    int thread_num = 2;
    cout <<"---------------------------------"<<endl;
    cout << "Opening File 2 for Input..."<<endl;
    cout <<"---------------------------------"<<endl;
    FILE * filein2 = fopen("hw5-2.in","r");
    
    if(filein2){
        do{
            get_next_char(filein2, thread_num);
        }while(!finished2);
    }
    else{
        cout << "Error: File 2 could not be opened"<<endl;
        exit(0);
    }
}


//Thread to read from hw5-3.in
void * thread3(void * arg){
    pthread_cond_wait(&c3, &m3);
    int thread_num = 3;
    cout <<"---------------------------------"<<endl;
    cout << "Opening File 3 for Input..."<<endl;
    cout <<"---------------------------------"<<endl;

    FILE * filein3 = fopen("hw5-3.in","r");
    
    if(filein3){
        do{
            get_next_char(filein3, thread_num);
        }while(!finished3);
    }
    else{
        cout << "Error: File 3 could not be opened"<<endl;
        exit(0);
    }
}

void get_next_char(FILE * in, int tnum){
    if(tnum == 1){
        while(1){
                if((fscanf(in, "%c\n",&resource)) != EOF){
                    cout << "Reading char from file: \"hw5-1.in\"" << endl;
                    print = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    pthread_cond_wait(&c1, &m1);
                }
                else{
                    print = false;
                    cout <<"---------------------------------"<<endl;
                    cout<<"Reached end of file for thread 1"<<endl;
                    if(fclose(in) != 0){
                        cout<<"Closing of file 1 failed!"<<endl;
                    }else{
                        cout << "Closing Input File 1 ..."<<endl;
                        cout <<"---------------------------------"<<endl;
                    }
                    finished1 = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    // pthread_cond_wait(&c1, &m1);

                    pthread_exit(0);
                    break;
                }
        }
        return;
    }
    if(tnum == 2){
        while(1){
                if((fscanf(in, "%c\n",&resource)) != EOF){
                    cout << "Reading char from file: \"hw5-2.in\"" << endl;
                    print = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    pthread_cond_wait(&c2, &m2);
                }
                else{
                    cout <<"---------------------------------"<<endl;
                    cout<<"Reached end of file for thread 2"<<endl;
                    if(fclose(in) != 0){
                        cout<<"Closing of file 2 failed!"<<endl;
                    }else{
                        cout << "Closing Input File 2 ..."<<endl;
                        cout <<"---------------------------------"<<endl;
                    }
                    print = false;
                    finished2 = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    //pthread_cond_wait(&c2, &m2);

                    pthread_exit(0);
                    break;
                }
        }
        return;
    }
    else if(tnum == 3){
        while(1){
                if((fscanf(in, "%c\n",&resource)) != EOF){
                    cout << "Reading char from file: \"hw5-3.in\"" << endl;
                    print = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    pthread_cond_wait(&c3, &m3);
                }
                else{
                    cout <<"---------------------------------"<<endl;
                    cout<<"Reached end of file for thread 3"<<endl;
                    if(fclose(in) != 0){
                        cout<<"Closing of file 3 failed!"<<endl;
                    }else{
                        cout << "Closing Input File 3 ..."<<endl;
                        cout <<"---------------------------------"<<endl;
                    }
                    print = false;
                    finished3 = true;
                    pthread_mutex_unlock(&m_write);
                    pthread_cond_signal(&c_write);
                    //pthread_cond_wait(&c3, &m3);

                    pthread_exit(0);
                    break;
                }
        }
        return;
    }
}

void write_char(FILE * out){
    //cout <<"---------------------------------"<<endl;
    //cout<<"Writing character to output: \""<< resource <<"\""<< endl;
   // cout <<"---------------------------------"<<endl;
    fprintf(out, "%c\n", resource);
    resource = '\0';
    print = false;
}