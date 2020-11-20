//Sam Dressler
//Program 2
//HW 10

#include "program2.h"
int count1 = 0;
int count2 = 0;
int main(int argc, char * argv[]){
    //0 - read 1 - write
    int fd1 [2];
    int fd2 [2];

    fd1[0] = atoi(argv[0]);
    fd1[1] = atoi(argv[1]);
    close(fd1[1]);
    fd2[0] = atoi(argv[3]);
    fd2[1] = atoi(argv[4]);

    close(fd1[1]);
    close(fd2[0]);
    
    sem_t * sem = sem_open(argv[2], O_CREAT, 0644, 1);
    if(sem == SEM_FAILED){
        cout <<"Error sem : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        close(fd1[0]);
        close(fd2[1]);
        return 1;
    }
    sem_t * sem2 = sem_open(argv[5], O_CREAT, 0644, 1);
    if(sem2 == SEM_FAILED){
        cout <<"Error sem2 : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        sem_unlink("/sem2");
        sem_close(sem2);
        close(fd1[0]);
        close(fd2[1]);
        return 1;
    }
    /*
        Wait for program 1 to signal program 2 to start processing
    */
    if(sem_wait(sem)< 0){
        cout << "Error wait : " << errno << endl;
        sem_unlink("/sem");
        sem_close(sem);
        sem_unlink("/sem2");
        sem_close(sem2);
        close(fd1[0]);
        close(fd2[1]);
        return 2;
    }
    cout <<"IN PROGRAM 2" << endl;
    cout <<"----------------------------------------"<<endl;
    sleep(1);
    /*
        Begin reading words from pipe 1 and writing to pipe 2
    */
    bool run = true;
    int nread;
    char buff[BUFF_SIZE];
    FILE * fout = fopen("output.data", "w");
    FILE * s1out = fopen("shared1.data", "w");
    FILE * s2out = fopen("shared2.data", "w");
    if(!fout || !s1out || !s2out){
        cout << "Error openeing output" << endl;
        sem_unlink("/sem");
        sem_close(sem);
        sem_unlink("/sem2");
        sem_close(sem2);
        close(fd1[0]);
        close(fd2[1]);
        fclose(fout);
        fclose(s1out);
        fclose(s2out); 
    }
    while(run){
        //wait for program 1 to write a word
        if(sem_wait(sem)< 0){
            cout << "Error wait : " << errno << endl;
            sem_unlink("/sem");
            sem_close(sem);
            sem_unlink("/sem2");
            sem_close(sem2);
            close(fd1[0]);
            close(fd2[1]);
            fclose(fout);
            fclose(s1out);
            fclose(s2out);  
            return 3;
        }
        nread = read(fd1[0], &buff, BUFF_SIZE);
        switch(nread){
            case EOF:
                // if(errno == EAGAIN){
                    cout << "Pipe Empty" << endl;
                    run = false;
                    if(sem_post(sem)< 0){
                        cout << "Error post : " << errno << endl;
                        sem_unlink("/sem");
                        sem_close(sem);
                        sem_unlink("/sem2");
                        sem_close(sem2);
                        close(fd1[0]);
                        close(fd2[1]);
                        fclose(fout);
                        fclose(s1out);
                        fclose(s2out);  
                        return 5;
                    }
                    break;
            case 0:
                printf("All entries read\n");
                close(fd1[0]);
                run = false;
                break;
            default:
                //word read from input buffer
                if(strcmp(buff, "END") == 0){
                    cout <<"----------------------------------------" << endl;
                    cout <<"REACHED END OF INPUT" <<endl;
                    cout <<"----------------------------------------" << endl;
                    if(write(fd2[1], &buff, BUFF_SIZE) < 0){
                        cout << "ERROR write 2 : " << errno << endl;
                        close(fd2[1]);
                        close(fd2[0]);
                        sem_unlink("/sem");
                        sem_close(sem);
                        sem_unlink("/sem2");
                        sem_close(sem2);
                        fclose(fout);
                        fclose(s1out);
                        fclose(s2out);      
                    }
                    close(fd2[1]);
                    fprintf(s1out, "%d", count1);
                    fprintf(s2out, "%d", count2);
                    run = false;
                    if(sem_post(sem2)< 0){
                        cout << "Error post 2 : " << errno << endl;
                        sem_unlink("/sem2");
                        sem_close(sem2);
                        sem_unlink("/sem");
                        sem_close(sem);
                        close(fd1[0]);
                        close(fd2[1]);
                        fclose(fout);
                        fclose(s1out);
                        fclose(s2out);  
                        return 5;
                    }
                    break;
                }
                // cout << "READ : " << buff << endl;
                string word;
                int filled_length = 0;
                for(int i = 0; i < BUFF_SIZE; i++){
                    if(buff[i] == '\0'){
                        filled_length++;
                        break;
                    }
                    else{
                        filled_length++;
                    }
                }
                char temp [filled_length];
                strncpy(temp, buff, filled_length);
                // cout << "char * temp : " << temp << endl;
                for(int i = 0; i < filled_length; i++){
                    word += temp[i];
                }
                string word2;
                // cout << "string WORD : " << word << endl;
                word2 = process_word(word);
                cout << "----------------------------------------" << endl;
                cout << "Reading unprocessed Word from pipe 1..." << endl;
                cout << "Unprocessed Word: " << word << endl;
                cout << "Processed Word  : " <<word2 <<endl;
                cout << "Writing processed Word to pipe 2..." << endl;
                cout << "----------------------------------------" << endl;
                usleep(500000);
                fprintf(fout, "%s ", word2.c_str());
                if(write(fd2[1], &buff, BUFF_SIZE) < 0){
                    cout << "ERROR write 2 : " << errno << endl;
                    close(fd2[1]);
                    close(fd2[0]);
                    sem_unlink("/sem");
                    sem_close(sem);
                    sem_unlink("/sem2");
                    sem_close(sem2);
                    fclose(fout);
                    fclose(s1out);
                    fclose(s2out);  
                }

                // cout <<endl<<"WROTE : " << buff << endl;

                // strcpy(buff, temp.c_str());

                if(sem_post(sem)< 0){
                    cout << "Error post : " << errno << endl;
                    sem_unlink("/sem");
                    sem_close(sem);
                    sem_unlink("/sem2");
                    sem_close(sem2);
                    close(fd1[0]);
                    close(fd2[1]);
                    fclose(fout);
                    fclose(s1out);
                    fclose(s2out);  
                    return 5;
                }
                break;
        }
        if(sem_post(sem)< 0){
            cout << "Error post : " << errno << endl;
            sem_unlink("/sem");
            sem_close(sem);
            sem_unlink("/sem2");
            sem_close(sem2);
            close(fd1[0]);
            close(fd2[1]);
            fclose(fout);
            fclose(s1out);
            fclose(s2out);
            return 5;
        }
    }
    fclose(fout);
    fclose(s1out);
    fclose(s2out);
    sem_unlink("/sem");
    sem_close(sem);
    sem_unlink("/sem2");
    sem_close(sem2);
    close(fd1[0]);
    close(fd2[1]);
    cout << "END PROGRAM 2" << endl;
    cout << "------------------------------------------" << endl; 
}

string process_word(string word){
        // cout << "here" << endl;
        string temp;
        size_t pos;
            switch(word.at(0)){
            case 'A':
            case 'E':
            case 'I':
            case 'O':
            case 'U':
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
            {
                count1++; //Increment type 1, starts with vowel
                temp = word; //working with strings
                if((pos = temp.find_first_of(".,")) != std::string::npos){
                    char tempc = word[pos];
                    temp[pos] = 'r';
                    temp.append("ay");
                    if(tempc == ','){
                        temp.append(",");
                    }
                    else if(tempc == '.'){
                        temp.append(".");
                    }
                }
                else{
                    pos = temp.length()-1;
                        temp[pos] = 'r';
                        temp.append("ay");
                }
                
                word = temp; 
                // cout << "PROCESSED WORD V: " << word << endl;                

                break;
            }
            default:
                count2++; //Increment type 2, starts with vowel
                string result = "";
                word+= " ";
                temp = "";
                // Character to hold the first character of the word
                char tempc1 = word[0];
                // Character to hold punctuation
                char tempc2;
                //code for removing first letter of the word
                for(size_t i = 0; i < word.length(); i ++){
                    if(word[i] == ' '){
                        result += temp.substr(1) ;
                        temp = "";
                    }
                    else{
                        temp += word[i];
                    }
                }
                //result from removing first letter but need to remove added space
                temp = "";
                for(size_t i = 0; i <result.length()-1; i++){
                    temp += result[i];
                }

                if((pos = temp.find_first_of(".,")) != std::string::npos){
                    tempc2 = temp.at(pos);
                    temp.at(pos) = tempc1;
                    temp.append("ay");
                    string t1;
                    t1 += tempc2;
                    temp.append(t1);
                    // temp.append(" ");
                    // temp.at(temp.length()-1) = tempc2;
                }
                else{
                    // temp += " ";
                    temp.at(temp.length()-1) = tempc1;
                    temp.append("ay");
                }
                word = temp;
                // cout << "PROCESSED WORD C : " << word << endl;  
                break;
        }
        return word;
}