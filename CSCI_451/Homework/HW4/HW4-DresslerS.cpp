//Sam Dressler
//HW 4
//CSCI 451
//9/23/2020

#include <iostream>
#include <stdio.h>
#include <thread>
#include <unwind.h>
#include <fstream>
#include <string>
#include <pthread.h>
#include <mutex>

#define NUM_THREADS 2
using namespace std;

//Fucntion definition
void * process_reading(void *);
void * process_writing(void *);
void getNextInt(FILE *);
void writeNextInt(FILE *);

//global variable to store the current value read in from the file.
int num = 0;

//1 == reading, 2 == writing
int favored_process = 1;
//let both process know when we've reached the end of the file.
bool ready_to_write = false;
bool finished = false;
//Create instance of mutual exclusion
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//struct to help with passing the out file to the function
struct thread_data {
    FILE * out_file;
};
int main(){
    
    /**
    * Opening file for output
    */
    cout << "Opening File for Output..." << endl;
    FILE *file_out = NULL;
    file_out = fopen("hw4.out","w");
    
    if(!file_out){
        perror("fopen");
    }
    /**
     * Initialize pthread mutex and pthread cond variables
     */
    pthread_mutex_init(&mux, NULL);
    pthread_cond_init(&cond, NULL);
    /**
     * Creating 2 threads that will be doing reading and writing from input  
    */ 
    pthread_t thread_read;
    pthread_t thread_write;
    struct thread_data thread_data; 
    thread_data.out_file = file_out;
        
        pthread_create(&thread_read, NULL, process_reading, (void*)&thread_data);
        pthread_create(&thread_write, NULL, process_writing, (void *)&thread_data);


    pthread_join(thread_read, NULL);
    pthread_join(thread_write, NULL);

    cout<<"Closing File for Output..."<<endl;
    fclose(file_out);
    pthread_mutex_destroy(&mux);
    pthread_exit(NULL);
    return 0;
}
/**
 * Thread to read data
 */
void * process_reading(void * arg){
    pthread_mutex_lock(&mux);
    FILE * file_in = fopen("hw4.in", "r");
    cout << "Opening File for Input..."<<endl;
    cout <<"-----------------------------"<<endl;
    //Enter critical Section
    do{
        getNextInt(file_in);
    }while(!finished);

    pthread_exit(NULL);
    return NULL;
}

void getNextInt(FILE * file_in){
 if(file_in){
     while(fscanf(file_in, "%d\n",&num)!=EOF){
        cout<<"Reading Data from File..."<<endl;
        ready_to_write = true;
        pthread_mutex_unlock(&mux);
        pthread_cond_signal(&cond);
        pthread_cond_wait(&cond,&mux);
     }
       pthread_mutex_unlock(&mux);
       pthread_cond_signal(&cond);
        finished = true;
        ready_to_write = false;
        cout<<"Reached end of file"<<endl;
        cout <<"-----------------------------"<<endl;
        
        if(fclose(file_in) != 0){
            cout<<"Close failed!"<<endl;
        }else{
            cout << "Closing Input File..."<<endl;
        }

        return;
 }else{
     cout<<"ERROR: File could not be opened"<<endl;
     exit(0);
 }
}
/**
 * Thread to write data
 */
void * process_writing(void * arg){
    pthread_mutex_lock(&mux);
    thread_data *temp = (thread_data *)arg;
    FILE * file_out = temp->out_file;
   
    do {
            
            //Critical section to write number into file
            if(ready_to_write == true){
                writeNextInt(file_out);
                if(!finished){
                    pthread_mutex_unlock(&mux);
                    pthread_cond_signal(&cond);
                    pthread_cond_wait(&cond, &mux);

                }else{
                    pthread_exit(0);
                }
            }else{
                pthread_mutex_unlock(&mux);
            }
    }while(!finished);
    if(fclose(file_out) != 0){
        cout<<"Out File failed to close"<<endl;
    }
    pthread_exit(0);
}
void writeNextInt(FILE * file_out){
    if(file_out){
        cout<<"Writing Data to File..."<<endl;
        if(num % 2 == 0){            
            fprintf(file_out, "%d\n%d\n",num,num);
            printf("%d\n%d\n",num,num);
            ready_to_write = false;
        }else if (num % 2 == 1){
            fprintf(file_out, "%d\n", num);
            printf("%d\n",num);
            ready_to_write = false;
        }
    }
    else{
        cout << "ERROR: Write file could not be opened"<<endl;
        exit(0);
    }
}