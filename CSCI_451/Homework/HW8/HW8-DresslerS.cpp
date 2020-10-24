//Sam Dressler
//CSCI 451
//HW 8 
//10-21-2020

#include "HW8-DresslerS.h"

int main (){

    thread_data thread_data;
    page_node * root = NULL;
    struct cll *list = NULL;    
    list = new cll;                 //create the initial struct for the circular linked list data type
    thread_data.list = list;        //assign the cll struct pointer in the thread data to the cll just created
    thread_data.list->head = root;   //assign the initial value of the head node.

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
    int r = -1;
    int r2 = -1;
    //Wait for all threads to finish
    srand(time(0));
    sleep(2);
   while(!thread_data.all_done){
       r = rand() % 9;
        if(r == 0 || r == 1){
            r2 = (rand() % 10);
            if(r2 == 3 ){
                reset_all_R_bits(&thread_data);
                r = -1;
                r2 = -1;
                sleep(3);
            }
       }
   }
   cout << "Final Account Balance: $"<<balance<<endl;
   cout << "----------------------------------------"<<endl;
   print_cll_data(&thread_data);
   pthread_mutex_destroy(&mux);
    return 0;
}
//thread 1
void * vlad_thread(void * arg){
    sleep(1);
    struct thread_data * info  = (thread_data *) arg;
    FILE * cmd_file = fopen("Vlad.in", "r");
    if(!cmd_file){
        cout << "error opening file" << endl;
    }
    int thread_num = 0;
    int r = -1;
    int r1 = -1;
    create_page(info, thread_num);
    sleep(1);
    while(!info->all_done)
    {
            if(!info->t1_done){
            r = rand() % 7;
                //cout << "r: " << r << endl;
            if(r == 0 || r == 6 || r == 4){               
            r1 = (rand() % 5);
            //cout << "V1 = "<< r1 << endl;
            switch(r1){
                case 0:   
                    r1 = 1; 
                case 1:            
                    if(info->t2_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 2:
                    if(info->t3_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 3:                
                    if(info->t4_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 4:
                    if(info->t5_done!= true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                }
            }
                info->t1_done = thread_read_section(cmd_file,thread_num,info);
            }else{
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
//thread 2
void * frank_thread(void * arg){
    sleep(1);
    struct thread_data * info  = (thread_data *) arg;
    FILE * cmd_file = fopen("Frank.in", "r");
    if(!cmd_file){
        cout << "error opening file" << endl;
    }
    int thread_num = 1;
    int r = -1;
    int r1 = -1;
    create_page(info, thread_num);
    sleep(1);
    while(!info->all_done)
    {
        if(!info->t2_done){
                r = rand() % 7;
                //cout << "r: " << r << endl;
            if(r == 0 || r == 6 || r == 4){   
            r1 = (rand() % 5);
            //cout << "V1 = "<< r1 << endl;
            switch(r1){
                case 0:    
                    if(info->t1_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 1:            
                    r1 =2;
                case 2:
                    if(info->t3_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 3:                
                    if(info->t4_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 4:
                    if(info->t5_done!= true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                }
            }
            info->t2_done = thread_read_section(cmd_file,thread_num,info); 
        }else{
            update_all_done(info);
        }
    } 
    fclose(cmd_file);
    return NULL;
}
//thread 3
void * bigfoot_thread(void * arg){
    sleep(1);
    struct thread_data * info  = (thread_data *) arg;
    FILE * cmd_file = fopen("Bigfoot.in", "r");
    if(!cmd_file){
        cout << "error opening file" << endl;
    }
    int thread_num = 2;
    int r = -1;
    int r1 = -1;
    create_page(info, thread_num);
    sleep(1);
    while(!info->all_done)
    {
            if(!info->t3_done){
            r = rand() % 7;
                //cout << "r: " << r << endl;
            if(r == 0 || r == 6 || r == 4){     
            r1 = (rand() % 5);
            //cout << "V1 = "<< r1 << endl;
            switch(r1){
                case 0:    
                    if(info->t1_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 1:            
                    if(info->t2_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 2:
                    r1 =3;
                case 3:                
                    if(info->t4_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 4:
                    if(info->t5_done!= true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                }
            }
                info->t3_done = thread_read_section(cmd_file,thread_num,info);
            }else{
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
//thread 4
void * casper_thread(void * arg){
    sleep(1);
    struct thread_data * info  = (thread_data *) arg;
    FILE * cmd_file = fopen("Casper.in", "r");
    if(!cmd_file){
        cout << "error opening file" << endl;
    }
    int thread_num = 3;
    int r = -1;
    int r1 = -1;
    create_page(info, thread_num);
    sleep(1);
    while(!info->all_done)
    {
            if(!info->t4_done){
            r = rand() % 7;
            if(r == 0 || r == 6 || r == 4){   
            r1 = (rand() % 5);
            switch(r1){
                case 0:    
                    if(info->t1_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 1:            
                    if(info->t2_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 2:
                    if(info->t3_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 3:                
                    r1 = 4;
                case 4:
                    if(info->t4_done != true){
                    if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                }
            }
            info->t4_done = thread_read_section(cmd_file,thread_num,info);
            }else {
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}
//thread 5
void * gomez_thread(void * arg){
    sleep(1);
    struct thread_data * info  = (thread_data *) arg;
    FILE * cmd_file = fopen("Gomez.in", "r");
    int thread_num = 4;
    int r = -1;
    int r1 = -1;
    bool flag = false;
    create_page(info, thread_num);
    sleep(1);
    while(!info->all_done)
    {
            if(!info->t5_done){
            r = rand() % 7;
            if(r == 0 || r == 6 || r == 4){               
            r1 = (rand() % 5);
            switch(r1){
                case 0:   
                    if(info->t1_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 1:            
                    if(info->t2_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 2:
                    if(info->t3_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 3:                
                    if(info->t4_done != true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                case 4:
                    r1 = 0;
                    if(info->t1_done!= true){
                        if(page_num >6) cout << "random fault" << endl;
                        replace_page(info, r1);
                        r1 = -1;
                    }
                    break;
                }
            }
                info->t5_done = thread_read_section(cmd_file,thread_num,info);
                flag = info->t5_done;
            }else{
                if(flag){
                    //print_cll_data(info);
                    flag = false;
                }
                update_all_done(info);
            }  
    } 
    fclose(cmd_file);
    return NULL;
}

bool thread_read_section(FILE * cmd_file, int thread_num, struct thread_data * info){
    char buf[BUF_SIZE];
    char c;
    float update = 0.0f;
    bool found = false;
    struct page_node * current = info->list->head;
    if(fgets(buf, BUF_SIZE, cmd_file)==NULL){  
        return true;
    }
    sscanf(buf, "%c\n",&c );
    if(c == 'R'){
        pthread_mutex_lock(&mux);
        pthread_mutex_lock(&list_lock);
        do{
            // current = info->list->head;
            // while(current->next != info->list->head){
            //     if(current->owner == get_monster(thread_num)){
            //         found = true;
            //         break;
            //     }
            //     else{
            //         current = current->next;

            //     }
            // }
            // if(found == false){
            //     if(current->owner == get_monster(thread_num)){
            //         found = true;
            //     }
            //     else{
            //         found = false;
            //     }
            // } 
            if(fgets(buf, BUF_SIZE, cmd_file)!=NULL){
                sscanf(buf, "%c%f\n", &c, &update);
                if(c != 'W'){
                    if(c == '+' || c == '\0'){
                        balance += update;
                       // cout << "Deposit: (+) $"<< update << endl;
                    }
                    else if(c == '-'){
                        balance -= update;
                        //cout << "Withdraw: (-) $"<< update << endl;
                    }
                }
                else{
                        if(balance < 0){
                            current->r = 1;
                            current->m = 1;
                        // cout << "Monster: " << current->owner << "- r: " << current->r << endl;
                        //  cout << "Monster: " << current->owner << "- m: " << current->m << endl;
                        }
                        else{
                            current->r = 1;
                        //  cout << "Monster: " << current->owner << "- r: " << current->r << endl;
                        }
                        
                        thread_write_section(thread_num);
                        usleep(U_ONE_SECOND);
                        pthread_mutex_unlock(&list_lock);
                        pthread_mutex_unlock(&mux);
                        usleep(U_THREE_QUARTER_SECOND);
                        return false;
                    
                }
            }else{  
                pthread_mutex_unlock(&list_lock); 
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

void update_all_done(thread_data *info){
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

string get_monster(int thread_num){
    switch(thread_num){
        case 0:
            return "Vlad";
            break;
        case 1:
            return "Frank";
            break;
        case 2:
            return "Bigfoot";
            break;
        case 3: 
            return "Casper";
            break;
        case 4:
            return "Gomez";
            break;
        default:
            cout << "Error: invalid thread num " << endl;
            exit(0);
    }
}

bool create_page(thread_data * info, int thread_num){
    pthread_mutex_lock(&mux);
    string monster = get_monster(thread_num);
    struct cll *list = info->list;
    struct page_node *current_page = new page_node;
    //If we havne't used all the pages 
    if(page_num < 7){      
        if(list->head == NULL){
            cout << "Creating page: "<<page_num<< endl;
            //initialize some data for the first page
            current_page->owner = monster; //indicate which monster owns the page
            current_page->page_id = page_num; // give the node an ID that will be used to know when the max number of pages has been met.        
            current_page->r = 0;
            current_page->m = 0;
            list->head = current_page;  //set where the head node is in the cll
        }
        else{
            cout << "Creating page: "<<page_num<<endl;
            current_page->owner = monster;
            current_page->page_id = page_num;
            current_page->r = 0;
            current_page->m = 0;
            list->tail->next = current_page;
        }
        list->tail = current_page;
        list->tail->next = list->head;
        page_num ++; // once the 6th page is created this loop no code block is no longer reached
       // print_cll_data(info); 
    }

    //find a place to 
   // print_cll_data(info);
    pthread_mutex_unlock(&mux);
    usleep(U_QUARTER_SECOND);

    return true;
}

void print_cll_data(thread_data * info){
    cout << "<======================================" << endl;
    struct cll *list = info->list;
    struct page_node * current = list->head;
    // cout << "Current pages in circular linked list" << endl;
    while(current->next != list->head){
        cout << "page: "<<current->page_id << "-> owner: " << current->owner << endl;
        cout << "R " << current->r << endl;
        cout << "M " << current->m << endl;
        current = current->next;
    }
    cout << "page: "<<current->page_id << "-> owner: " << current->owner << endl;
    cout << "R " << current->r << endl;
    cout << "M " << current->m << endl;
    cout << "======================================>" << endl;
    return;
}

void replace_page(thread_data * info, int thread_num){
    
    string monster = get_monster(thread_num);
    struct cll *list = info->list;
    struct page_node *current_page = new page_node;
    if(page_num > 6){
        pthread_mutex_lock(&list_lock);
        cout << "Page fault in thread " << monster <<"!"<<endl;
        cout << "----------------------------------------"<<endl;
        struct page_node * temp = list->head;    
        //assign classes so we know what node to remove
        while(temp->next != list->head){
            if(temp->r == 0 && temp->m == 0){
                temp->_class = 0;
            }
            else if(temp->r == 0 && temp->m == 1){
                temp->_class = 1;
            }
            else if(temp->r == 1 && temp->m == 0){
                temp->_class = 2;
            }
            else if(temp->r == 1 && temp->m == 1){
                temp->_class = 3;
            }
            temp = temp->next;
        }
        if(temp->r == 0 && temp->m == 0){
                temp->_class = 0;
            }
            else if(temp->r == 0 && temp->m == 1){
                temp->_class = 1;
            }
            else if(temp->r == 1 && temp->m == 0){
                temp->_class = 2;
            }
            else if(temp->r == 1 && temp->m == 1){
                temp->_class = 3;
            }
        temp = list->head;
        //find a page with a class of 0
        bool found_lowest_class = false;
        int lowest_class_found = -1;
        if(!found_lowest_class){
            while(temp->next != list->head){
                if(temp->_class == 0){
                    lowest_class_found = 0;
                    found_lowest_class = true;
                    // cout << "Lowest class = 0" << endl;
                    break;
                }
                else{
                    temp = temp->next;
                }
            }
        }
        temp = list->head;
        if(!found_lowest_class){
            while(temp->next != list->head){
                if(temp->_class == 1){
                    lowest_class_found = 1;
                    found_lowest_class = true;
                    // cout << "Lowest class = 1" << endl;
                    break;
                }
                else{
                    temp = temp->next;
                }
            }
        }
        temp = list->head;
        if(!found_lowest_class){
            while(temp->next != list->head){
                if(temp->_class == 2){
                    lowest_class_found = 2;
                    found_lowest_class = true;
                    // cout << "Lowest class = 2" << endl;
                    break;
                }
                else{
                    temp = temp->next;
                }
            }
        }
        temp = list->head;
        if(!found_lowest_class){
            while(temp->next != list->head){
                if(temp->_class == 3){
                    lowest_class_found = 3;
                    found_lowest_class = true;
                    // cout << "Lowest class = 3" << endl;
                    break;
                }
                else{
                    temp = temp->next;
                }
            }
        }
        temp = list->head;
        if(found_lowest_class){
            while(temp->next != list->head){
                if(temp->_class == lowest_class_found && temp->owner != monster){
                    cout <<"----------------------------------------"<<endl;
                    cout << "Old Page"<< temp->page_id <<" Owner : " << temp->owner << endl;
                    cout << "Old R: " << temp->r << endl;
                    cout << "Old M: " << temp->m << endl;
                    temp->owner = monster;
                    temp->r = 1;
                    temp->m = 1;
                    cout << "New Page"<< temp->page_id <<" Owner : " << temp->owner << endl;
                    cout <<"----------------------------------------"<<endl;
                   // print_cll_data(info);
                    sleep(2);
                    break;
                }
                else{
                    temp = temp->next;
                }
            }
            if(temp->_class == lowest_class_found && temp->owner != monster){
                    cout <<"----------------------------------------"<<endl;
                    cout << "Old Page"<< temp->page_id <<" Owner : " << temp->owner << endl;
                    cout << "R: " << temp->r << endl;
                    cout << "M: " << temp->m << endl;
                    temp->owner = monster;
                    temp->r = 1;
                    temp->m = 1;
                    cout << "New Page"<< temp->page_id <<" Owner : " << temp->owner << endl;
                    cout <<"----------------------------------------"<<endl;
                    //print_cll_data(info);
                    sleep(2);
                }
                else{
                    temp = temp->next;
                }
            while(temp->next != list->head){
                //reset the rest of the pages owned by current monster getting new page
                if(temp->owner == monster){
                    temp->r = 0;
                    temp->m = 0;
                  //  cout << "info for page: " << temp->page_id << endl;
                  //  cout << "reseting r for "<<monster<<" pages to " << temp->r << endl;
                  //  cout << "reseting m for "<<monster<<" pages to " << temp->m << endl << endl;
                    temp = temp->next;
                    
                }
                else{
                    temp = temp->next;
                }
            }
            if(temp->owner == monster){
                    temp->r = 0;
                    temp->m = 0;
                  //  cout << "info for page: " << temp->page_id << endl;
                  //  cout << "reseting r for "<<monster<<" pages to " << temp->r << endl;
                  //  cout << "reseting m for "<<monster<<" pages to " << temp->m << endl << endl;
                    temp = temp->next;
                    
                }
                else{
                    temp = temp->next;
                }
        }

        pthread_mutex_unlock(&list_lock);
    }
    else{
        create_page(info, thread_num);
    }
        

}

void reset_all_R_bits(struct thread_data * info){
    pthread_mutex_lock(&list_lock);
    struct page_node * temp = info->list->head;
    cout << "----------------------------------------"<< endl;
    cout << "Resetting all R bits" << endl;
    cout << "----------------------------------------"<< endl;
    while(temp-> next != info->list->head){
        temp->r = 0;
        if(temp->r == 0 && temp->m == 0){
                temp->_class = 0;
            }
            else if(temp->r == 0 && temp->m == 1){
                temp->_class = 1;
            }
            else if(temp->r == 1 && temp->m == 0){
                temp->_class = 2;
            }
            else if(temp->r == 1 && temp->m == 1){
                temp->_class = 3;
            }
            temp = temp->next;
    }
        temp->r = 0;
        if(temp->r == 0 && temp->m == 0){
            temp->_class = 0;
        }
        else if(temp->r == 0 && temp->m == 1){
            temp->_class = 1;
        }
        else if(temp->r == 1 && temp->m == 0){
            temp->_class = 2;
        }
        else if(temp->r == 1 && temp->m == 1){
            temp->_class = 3;
        }
        temp = temp->next;
    
    //print_cll_data(info);
    pthread_mutex_unlock(&list_lock);
    sleep(1);
    return;
}