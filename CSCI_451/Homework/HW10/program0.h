//Sam Dressler
//header for program 0
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <thread>
#include <errno.h>
#include <cstring>
#include <string.h>
using namespace std;

int get_word_count();
char * get_word();