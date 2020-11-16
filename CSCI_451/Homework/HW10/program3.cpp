//Sam Dressler
//HW 10
//CSCI 451
#include "program3.h" 
int main(int argc, char * argv[]){
    //0 - read 1 - write
    int fd1 [2];
    int fd2 [2];

    fd1[0] = atoi(argv[0]);
    fd1[1] = atoi(argv[1]);

    fd2[0] = atoi(argv[3]);
    fd2[1] = atoi(argv[4]);

    close(fd1[0]);
    close(fd1[1]);
    sem_t * sem2 = sem_open(argv[2], O_CREAT, 0644, 1);
    if(sem2 == SEM_FAILED){
        cout <<"Error sem2 : " << errno << endl;
        sem_unlink("/sem2");
        sem_close(sem2);
        close(fd2[0]);
        return 1;
    }
    if(sem_wait(sem2)< 0){
        cout << "Error wait : " << errno << endl;
        sem_unlink("/sem2");
        sem_close(sem2);
        close(fd2[0]);
        return 2;
    }
    sleep(1);
    cout << "----------------------------------------" << endl; 
    cout << "BEGIN PROGRAM 3" << endl;
    cout << "----------------------------------------" << endl; 
    cout << "CREATING output" << endl;
    sleep(1);
    char buff [BUFF_SIZE];
    //OPEN shared data 1 and print count

    //OPEN shared data 2 and print count

    //Reconstruct original document

    bool run = true;
    int nread;
    while(run){     
        //WAIT FOR PROGRAM 2 TO FINISH AND THEN READ THE PIPE
        nread = read(fd2[0], &buff, BUFF_SIZE);
        switch(nread){
            case EOF:
                // if(errno == EAGAIN){
                close(fd2[1]);
                close(fd2[0]);
                run = false;
                break;
            case 0:
                close(fd2[0]);
                close(fd2[1]);
                run = false;
                break;
            default:
                //word read from input buffer
                if(strcmp(buff, "END") == 0){
                    close(fd2[0]);
                    close(fd2[1]);
                    run = false;
                    break;
                }
                cout << "Read value : " << buff << endl;

                break;
        }
    }
    FILE *s1in = fopen("shared1.data", "r");
    FILE *s2in = fopen("shared2.data" ,"r");
    if(!s1in || !s2in){
        cout << "ERROR: " << errno <<endl; 
        close(fd2[0]);
        close(fd2[1]);
        return -1;
    }
    int count1 = 0;
    int count2 = 0;
    sleep(1);
    fscanf(s1in, "%d",&count1);
    cout << "Type 1 : " << count1 << endl;
    fscanf(s2in, "%d",&count2);
    cout << "Type 2 : " << count2 << endl;
    sleep(1);

    fclose(s1in);
    fclose(s2in);
    cout << "----------------------------------------" << endl; 
    cout << "END PROGRAM 3" << endl;
    cout << "----------------------------------------" << endl;
    sem_unlink("/sem2");
    sem_close(sem2);
    return 0;
}