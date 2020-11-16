//Sam Dressler
//Header for program2.cpp
//HW10
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstddef>
#include <semaphore.h>
#include <sys/wait.h>


using namespace std;

#define BUFF_SIZE 30

string process_word(string word);