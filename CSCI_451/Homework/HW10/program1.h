//Sam Dressler
//Header for prog 1
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <bits/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

using namespace std;

#define BUFF_SIZE 30

int get_word_count(FILE *);