//Header for HW8 
// Sam Dressler

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 5
#define BUF_SIZE 1024
#define U_ONE_SECOND 1000000
#define U_QUARTER_SECOND 250000
#define U_HALF_SECOND 500000
#define U_THREE_QUARTER_SECOND 750000
using namespace std;

//GLOBALS
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t list_lock = PTHREAD_MUTEX_INITIALIZER;
float balance = 0.0;
int page_num = 1;

//DATA STRUCTURES
struct page_node{    
    page_node *next;
    int page_id = 0;
    string owner = "open"; //what monster owns this page
    int _class = 0;
    int r = 0; //referenced bit
    int m = 0; //modified
};

struct cll { //structure for the circular linked list
    struct page_node *head;
    struct page_node *tail;
};

struct thread_data {
    struct cll *list;
    bool t1_done = false;
    bool t2_done = false;
    bool t3_done = false;
    bool t4_done = false;
    bool t5_done = false;
    bool all_done = false;
};

//FUCNTION DEFINITIONS
void * vlad_thread(void *);
void * frank_thread(void *);
void * bigfoot_thread(void *);
void * casper_thread(void *);
void * gomez_thread(void *);
bool thread_read_section(FILE *, int, struct thread_data *);
void thread_write_section(int);
void update_all_done( struct thread_data *);
bool create_page(thread_data *, int);
void print_cll_data(thread_data *);
string get_monster(int);
void reset_all_R_bits(struct thread_data *);
void replace_page(thread_data *, int);