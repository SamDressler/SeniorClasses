//Sam Dressler
//CSCI 451
//HW 7 
//10-11-2020
//5 little monster - account management between 5 threads with critical section
// Tested 10-12-2020 with the files included in zip and it ran correctly reliably

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <pthread.h>
#include <mutex>
#include <unistd.h>


#define NUM_THREADS 5
#define BUF_SIZE 1024

void * vlad_thread(void *);
void * frank_thread(void *);
void * bigfoot_thread(void *);
void * casper_thread(void *);
void * gomez_thread(void *);
bool thread_read_section(FILE *, int);
void thread_write_section(int);
void update_all_done( struct thread_status *);

pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;

using namespace std;

float balance = 0.0;

struct thread_status {
    bool t1_done = false;
    bool t2_done = false;
    bool t3_done = false;
    bool t4_done = false;
    bool t5_done = false;
    bool all_done = false;
};

int main (){
    thread_status thread_data;
    pthread_mutex_init(&mux, NULL);
    pthread_t monsters[NUM_THREADS];

    pthread_create(&monsters[0],NULL, vlad_thread, (void*) &thread_data);
    pthread_detach(monsters[0]);
    cout << "Vlad thread created" << endl;
    pthread_create(&monsters[1],NULL, frank_thread, (void *) &thread_data);
    pthread_detach(monsters[1]);
    cout << "Frank thread created" << endl;
    pthread_create(&monsters[2],NULL, bigfoot_thread, (void *) &thread_data);
    pthread_detach(monsters[2]);
    cout << "Bigfoot thread created" << endl;
    pthread_create(&monsters[3],NULL, casper_thread, (void *) &thread_data);
    pthread_detach(monsters[3]);
    cout << "Casper thread created" << endl;
    pthread_create(&monsters[4],NULL, gomez_thread, (void*) &thread_data);
    pthread_detach(monsters[4]);
    cout << "Gomez thread created" << endl;
    cout << "----------------------------------------"<<endl;
    
    //Wait for all threads to finish
   while(!thread_data.all_done){
       ;
   }
   cout << "Final account balance: $"<<balance<<endl;
   pthread_mutex_destroy(&mux);
    return 0;
}

void * vlad_thread(void * arg){
    sleep(1);
    struct thread_status * info  = (thread_status *) arg;
    FILE * cmd_file = fopen("Vlad.in", "r");
    int thread_num = 0;
    while(!info->all_done)
    {
            if(!info->t1_done){
                info->t1_done = thread_read_section(cmd_file,thread_num);
            }else{
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
void * frank_thread(void * arg){
    sleep(1);
    struct thread_status * info  = (thread_status *) arg;
    FILE * cmd_file = fopen("Frank.in", "r");
    int thread_num = 1;
    while(!info->all_done)
    {
            if(!info->t2_done){
                info->t2_done = thread_read_section(cmd_file,thread_num);   
            }else{
                update_all_done(info);
            }
    } 
    fclose(cmd_file);
    return NULL;
}
void * bigfoot_thread(void * arg){
    sleep(1);
    struct thread_status * info  = (thread_status *) arg;
    FILE * cmd_file = fopen("Bigfoot.in", "r");
    int thread_num = 2;
    while(!info->all_done)
    {
            if(!info->t3_done){
                info->t3_done = thread_read_section(cmd_file,thread_num);
            }else{
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
void * casper_thread(void * arg){
    sleep(1);
    struct thread_status * info  = (thread_status *) arg;
    FILE * cmd_file = fopen("Casper.in", "r");
    int thread_num = 3;
    while(!info->all_done)
    {
            if(!info->t4_done){
                info->t4_done = thread_read_section(cmd_file,thread_num);
            }else {
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
void * gomez_thread(void * arg){
    sleep(1);
    struct thread_status * info  = (thread_status *) arg;
    FILE * cmd_file = fopen("Gomez.in", "r");
    int thread_num = 4;
    while(!info->all_done)
    {
            if(!info->t5_done){
                info->t5_done = thread_read_section(cmd_file,thread_num);
            }else{
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}

bool thread_read_section(FILE * cmd_file, int thread_num){
    char buf[BUF_SIZE];
    char c;
    float update = 0.0f;
    if(fgets(buf, BUF_SIZE, cmd_file)==NULL){  
        return true;
    }
    sscanf(buf, "%c\n",&c );
    if(c == 'R'){
        pthread_mutex_lock(&mux);
        do{
            if(fgets(buf, BUF_SIZE, cmd_file)!=NULL){
                sscanf(buf, "%c%f\n", &c, &update);
                if(c != 'W'){
                    if(c == '+' || c == '\0'){
                        balance += update;
                        cout << "Deposit: (+) $"<< update << endl;
                    }
                    else if(c == '-'){
                        balance -= update;
                        cout << "Withdraw: (-) $"<< update << endl;
                    }
                }else{
                    thread_write_section(thread_num);
                    sleep(1);
                    pthread_mutex_unlock(&mux);
                    return false;
                }
            }else{   
                pthread_mutex_unlock(&mux);
                return true;
            }
        }while(true);

    }
    return false;
}
void thread_write_section(int thread_num){
    switch(thread_num){
        case 0: 
            cout << "----------------------------------------"<<endl;
            cout << "Account balance after Vlad thread: $" << balance << endl;
            cout << "----------------------------------------"<<endl;
            break;
        case 1:
            cout << "----------------------------------------"<<endl;
            cout << "Account balance after Frank thread: $" << balance << endl;
            cout << "----------------------------------------"<<endl;
            break;
        case 2:
            cout << "----------------------------------------"<<endl;
            cout << "Account balance after Bigfoot thread: $" << balance << endl;
            cout << "----------------------------------------"<<endl;
            break;
        case 3:
            cout << "----------------------------------------"<<endl;
            cout << "Account balance after Casper thread: $" << balance << endl;
            cout << "----------------------------------------"<<endl;
            break;
        case 4:
            cout << "----------------------------------------"<<endl;
            cout << "Account balance after Gomez thread: $" << balance << endl;
            cout << "----------------------------------------"<<endl;
            break;
    }
    return;
}

void update_all_done(thread_status *info){
    //function is called after a thread finishes to see if it was the last one.
    //if the last thread finishes then the all done variable is set to true
    if(info->t1_done == true && info->t2_done == true && info->t3_done ==true &&
        info->t4_done == true && info->t5_done == true){
            info->all_done = true;
            return;
        }    
    else{
        return;
    }
}