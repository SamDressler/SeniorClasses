//Sam Dressler
//Header for final code generation using 3 address code
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <tuple>


using namespace std;

//DEFINES
#define BUFF_SIZE 256

//FUNCTIONS
void process_line(char line [BUFF_SIZE]);

//GLOBALS
int curr_reg_num = 0;

//DRIVER FOR FCG
void fcg_driver(){
    FILE * sym_table_in = fopen("output_sym_table.txt","r");
    FILE * tac_in = fopen("output_icg.txt","r");
    FILE * fcg_out = fopen("fcg.txt", "w");
    if(!tac_in || !sym_table_in){
        cout << "ERROR : File(s) could not be opened" << endl;
        exit(-1); 
    }
    char tempw [BUFF_SIZE];
    char tempc;
    int i = 0;
    //Read from output_icg.txt to fill the stack used  in final code generation
    while(fscanf(tac_in, "%c", &tempc)!=EOF){
        if(tempc != '\n'){
            tempw[i] = tempc;
            i++;
        }
        else{
            tempw[i++] = '\n';
            process_line(tempw);
            i = 0;
            memset(tempw, 0, BUFF_SIZE);
        }
        
    }
    fclose(sym_table_in);
    fclose(tac_in);
    fclose(fcg_out);
}
void process_line(char line [BUFF_SIZE]){
    // cout << "Line : " << line;
    char *tok;
    tok = strtok(line, " ");
    cout << tok;
    while(tok != NULL){
        tok = strtok(NULL, " ");
        cout << tok << endl;
    }
    // strtok(line, " ")
}