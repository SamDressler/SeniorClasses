//HW 9
//Sam Dressler
//10/27/2020
//When parameter "mutex" is provided, the words from the input file will be printed in order and by various threads.
//When the parameter "nomutex" is provided, the words can be written to the output file in a different order since the access to the input file is never protected by a mutex, so threads that go first 
//The major difference I see is not when there are mutexes or not since they both tend to print the input file in order in the output file, but when the program runs without the sleep() calls, a single thread
//tends to do all of the writing to the file.
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fstream>

using namespace std;

#define NUM_THREADS 10

void * thread_task(void *);

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
struct thread{
    pthread_t thread_id;
    int thread_num;
    int choice = 0;
    FILE * file_in = NULL;
    FILE * file_out = NULL;
};

int main(int argc, char * argv[]){
    int choice = 0;//Choice 0 for error, 1 for mutex, 2 for nomutex

    if(argc != 2){
        cout << "Error : Must include parameter \"mutex\" or \"nomutex\"" << endl;
        exit(0);
    }
    string param = argv[1];
    if(param == "mutex"){
        choice = 1;
    }
    else if(param == "nomutex"){
        choice = 2;
    }
    else{
        cout << "Error : invalid parameter" << endl;
        cout << "Error : Must include parameter \"mutex\" or \"nomutex\"" << endl;
        exit(0);
    }

    struct thread threads[NUM_THREADS];
    FILE *file_in = fopen("hw9.data", "r");
    FILE * file_out;
    if(choice == 1){
        file_out = fopen("hw9.mutex", "w");
        if(file_out != NULL){
            cout <<"\"hw9.mutex\" opened successfully" << endl;
        }
    }
    else if(choice == 2){
        file_out = fopen("hw9.nomutex", "w");
        if(file_out != NULL){
            cout <<"\"hw9.nomutex\" opened successfully" << endl;
        }
    }

    for(int i = 0; i < NUM_THREADS; i++){
        threads[i].thread_num = i;
        threads[i].choice = choice;
        threads[i].file_in = file_in;
        threads[i].file_out = file_out;
        pthread_create(&(threads[i].thread_id), NULL, thread_task,  (void*) (threads+i));
    }
    cout<<"Reading input..."<<endl;
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i].thread_id, NULL);
    }
    cout<<"Write to file complete!"<<endl;
    fclose(file_in);
    fclose(file_out);
    pthread_mutex_destroy(&mux);
    return 0;
}

void * thread_task(void * arg){
    usleep(500000);
    char* word;
    size_t size = 50;
    struct thread * data = (thread*) arg;
    if(data->choice == 1){
        while(true){
            pthread_mutex_lock(&mux);
            if(getline(&word, &size , data->file_in) != EOF){ 
                // cout << "thread: "<<data->thread_num << " word: "<< word;
                fprintf(data->file_out, "Written by thread #%d\t: %s", data->thread_num, word);
                pthread_mutex_unlock(&mux);
                usleep(500000);
            }else{
                pthread_mutex_unlock(&mux);
                break;
            }
            
        }
    }else if(data->choice == 2){
        while(true){
            if(getline(&word, &size , data->file_in) != EOF){                     
                //cout << "thread: "<<data->thread_num << " word: "<< word << endl;
                if(word[strlen(word)-1] == '\0'){
                    fprintf(data->file_out, "Written by thread #%d\t: %s\n", data->thread_num, word);                    
                }
                else{
                    fprintf(data->file_out, "Written by thread #%d\t: %s", data->thread_num, word);
                }
                usleep(500000);
            }else{
                break;
            }
        }
    }
    else{
        cout << "Error Invalid parameter" << endl;
        pthread_exit(NULL);
    }
    pthread_exit(NULL);
}
