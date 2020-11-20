//Sam Dressler
//HW 10
//CSCI 451 
//11/12/2020

#include "program0.h"

int main(){

    //PIPES 
    // 0 - read
    // 1 - write
    int fd1[2];
    int fd2[2];


    int status = pipe(fd1);
    if(status < 0){
        cout << "Pipe 1 failed to open : " << errno << endl;
        return 1;
    }
    status = pipe(fd2);
    if(status< 0){
        cout << "Pipe 2 failed to open : " << errno << endl;
        return 2;
    }
    sem_t * sem = sem_open("/sem", O_CREAT, 0644, 1);
    if(sem == SEM_FAILED){
        cout << "ERROR sem" << errno << endl;
        return 3;
    }
    sem_t * sem2 = sem_open("/sem2", O_CREAT, 0644, 1);
    if(sem == SEM_FAILED){
        cout << "ERROR sem2" << errno << endl;
        return 3;
    }
    /**
     * Prepare the arguments for the exec calls
     * This block of code takes the pipe fd and converts them to strings so that they can be
     * passed to the functions
     */
    string str1, str2, str3, str4;
    const char * cmd1 = "./program1";
    const char * cmd2 = "./program2";
    const char * cmd3 = "./program3";
    //read end of pipe 1
    str1 = to_string(fd1[0]);
    const char * rdarg1 = str1.c_str();
    //write end of pipe 1
    str2 = to_string(fd1[1]);
    const char * wrarg1 = str2.c_str();
    //read end of pipe 2
    str3 = to_string(fd2[0]);
    const char * rdarg2 = str3.c_str();
    //write end of pipe 2
    str4 = to_string(fd2[1]);
    const char * wrarg2 = str4.c_str();
    //Semaphore used 
    const char * semarg = "/sem";
    const char * sem2arg = "/sem2";

    int id;
    id = fork();
    switch(id){
        /*
            Error case for first fork
        */
        case -1:
            cout << "Error fork : " << errno << endl;
            return 4;
        /*
            Case for first child process
        */
        case 0:
                cout << "Hello from child 1: "<< getpid() << " Parent: "<< getppid() << endl;
                cout << "Starting program 1..." << endl;
                cout << "------------------------------------------" << endl;
                /*
                    Execute program 1 
               */
                execl(cmd1, rdarg1, wrarg1, semarg, rdarg2, wrarg2, (char *) NULL);
                return 0;
        /*
            Case for the parent process
        */
        default:
            id = fork();
            switch(id){
                /*
                    Error case for second fork
                */
                case -1:
                    cout << "Error fork : " << errno << endl;
                    return 4;
                /*
                    Case for second child process
                */
                case 0:
                    cout << "Hello from child 2: "<< getpid() << " Parent: "<< getppid() << endl;
                    cout << "Starting program 2..." << endl;
                    cout << "------------------------------------------" << endl;
                    /*
                        Execute Program 2
                    */  
                    execl(cmd2, rdarg1, wrarg1, semarg, rdarg2, wrarg2, sem2arg, NULL);
                    return 0;
                /*
                    Case for the parent process
                */
                default:
                    while(wait(NULL) != -1 || errno != ECHILD){
                        // cout << "Waited in parent for a child to finish " << endl;
                    }   
                    cout << "------------------------------------------" << endl;
                    cout << "Programs 1 and 2 have completed!" << endl;
                    cout << "------------------------------------------" << endl; 
                    id = fork();
                    switch(id){
                        /*
                            Error case for third fork
                        */
                        case -1:
                            cout << "Error fork : " << errno << endl;
                            return 5;
                        /*
                            Case for third child process
                        */
                        case 0:
                            cout << "Hello from child 3: "<< getpid() << " Parent: "<< getppid() << endl;
                            cout << "Starting program 3..." << endl;
                            cout << "----------------------------------------" << endl;
                            execl(cmd3, rdarg1, rdarg2, sem2arg, rdarg2, wrarg2, NULL);
                            return 0;
                        /*
                            Case for Parent 
                        */
                        default:
                            // while(wait(NULL) != -1 || errno != ECHILD){
                            //     cout << "Waited in parent for a child to finish " << endl;
                            // } 
                            if(wait(NULL) != -1){
                                cout << "Parent Process finished executing" << endl;
                                cout << "----------------------------------------" << endl;
                                close(fd1[0]);
                                close(fd1[1]);
                                close(fd2[0]);
                                close(fd2[1]);
                                sem_unlink("/sem");
                                sem_close(sem);
                                sem_unlink("/sem2");
                                sem_close(sem2);
                                return 0; //program ends here                                
                            }
                    }    
            }
    }
}