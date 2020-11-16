//Sam Dressler
//Program 1
//HW 10

#include "program1.h"

int main(int argc, char * argv[]){
    char buff [BUFF_SIZE];
    //0 - read 1 - write
    int fd1 [2];
    int fd2 [2];

    fd1[0] = atoi(argv[0]);
    fd1[1] = atoi(argv[1]);

    fd2[0] = atoi(argv[3]);
    fd2[1] = atoi(argv[4]);
    
    sem_t * sem = sem_open(argv[2], O_RDWR, 0644, 1);
    if(sem == SEM_FAILED){
        cout <<"Error sem : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        close(fd1[0]);
        close(fd1[1]);

        return 1;
    }
    cout << "IN PROGRAM 1" << endl;
    cout << "------------------------------------------" << endl; 
    sleep(1);
    // cout << "Arg count : " << argc << endl;
    // cout << "PIPE 1 : rd " << fd1[0] << " wr " << fd1[1] <<endl;
    // cout << "Semaphore : " << argv[2] << endl;

    /*
        Get the word count
    */
      int word_count = 10;
      FILE * filein = fopen("input.data", "r");
      if(!filein){
          cout << "File didn't open" << endl;
          sem_unlink("/sem");
          sem_close(sem);
          close(fd1[0]);
          close(fd1[1]);
          return 3;
      }
      word_count = get_word_count(filein);
     if(word_count < 0){
        cout << "Error getting word_count : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        close(fd1[0]);
        close(fd1[1]);
        return 4;
     }
    cout << "WORDS IN FILE : " << word_count << endl;
    char tempc;
    string tempw;
    int idx= 0;
    rewind(filein);
    /* 
        create array for words in the file that will be written to pipe
    */
    string words[word_count+1];
    while(fscanf(filein, "%c", &tempc) != EOF){
        if(tempc == ' '){
            //If the current word is longer than zero
            if(tempw.length() > 0){
                words[idx] = tempw;
            }
            tempw.clear();
            idx++;
        }
        else{
            tempw += tempc;
        }
    }
    //Add the end flag to the list of words;
    word_count ++;
    words[idx] = "END";
    //Print all of the read in words
    // for (int i = 0; i < word_count; i ++){
    //     cout << words[i] << endl;
    // }
    if(sem_post(sem)< 0){
        cout << "Error post : " << errno << endl;
            sem_unlink("/sem");
            sem_close(sem);
            close(fd1[0]);
            close(fd1[1]);
        return 5;
    }
    /*
        Write the words from the file one at a time to be read
    */
    for(int i = 0; i < word_count; i ++){
        if(sem_wait(sem)< 0){
            cout << "Error wait : " << errno << endl;
            sem_unlink("/sem");
            sem_close(sem);
            close(fd1[0]);
            close(fd1[1]);
            return 6;
        }
        if(i == word_count){
            close(fd1[1]);
        }
        else{
            /*
                Write to pipe
            *///const_cast<char *>
            const char * temp = (words[i].c_str());
            strncpy(buff, temp, strlen(temp)+1);
            if(write(fd1[1], &buff , BUFF_SIZE) < 0){
                cout << "Error write : " << errno << endl;
                sem_unlink("/sem");
                sem_close(sem);
                close(fd1[0]);
                close(fd1[1]);
                return 7;
            }
        }
        if(sem_post(sem)< 0){
            cout << "Error post : " << errno << endl;
            sem_unlink("/sem");
            sem_close(sem);
            close(fd1[0]);
            close(fd1[1]);
            return 8;
        }
    }
    cout << "END PROGRAM 1" << endl << "----------------------------------------" << endl;
    sleep(1);
    if(sem_post(sem)< 0){
        cout << "Error post : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        close(fd1[0]);
        close(fd1[1]);
        return 9;
    }

    sem_unlink("/sem");
    sem_close(sem);
    close(fd1[0]);
    close(fd2[0]);
    close(fd2[1]);

    fclose(filein);
    }

//Reads the input file and returns the word count
//Returns -1 if the file is invalid
int get_word_count(FILE * file){
    char tempc;
    int count = 0;
    while(fscanf(file, "%c", &tempc)!=EOF){
        if(tempc == ' ' || tempc =='\n'){
            count ++;
        }
    }
    return count;
}