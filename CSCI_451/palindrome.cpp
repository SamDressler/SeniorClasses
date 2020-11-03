#include <iostream>
#include <bits/stdc++.h> 
#include <string>
using namespace std;
int main(){
    stack <char> s;
    string word;
    int index = 0;
    char tempchar;
    bool is_palindrome = true;
    cout << "Enter a string: "; 
    cin >> word;
    int len = word.length();
    int mid = len / 2;
    for(index = 0; index < mid ; index++){
        tempchar = word[index];
        s.push(tempchar);
    }
    if(len % 2 != 0){
        index++;
    }
    while(!s.empty()){
         if(s.top() == word[index]){
             s.pop();
             index++;
             continue;
         }
         else{
             is_palindrome = false;
             break;
         } 
    }
    if(is_palindrome){
        cout << "\"" << word << "\" is a palindrome" << endl;
    }
    else{
        cout << "\"" << word << "\" is not a palindrome" << endl;
    }
    return 0;
}