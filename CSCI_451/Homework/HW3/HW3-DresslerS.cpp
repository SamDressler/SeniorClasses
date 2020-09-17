//Sam Dressler
//HW 3
//CSCI 451
//9/16/2020

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SEARCH_WORDS 2
#define NUM_THREADS 2

using namespace std;

struct thread_data 
{	
	//Give the data a thread number so we know whose data this is
	int thread_num;
	//put a copy of the words from the file into the data structure for each thread
	string file_contents;
	//word to be searched for
	string search_word;
	//number of matches in the file that matches the search word
	int num_matches;
};



char * get_char_array(ifstream &file);
void * search_for_word(void *);

int main()
{
	//get webpage
	system("wget -q http://undcemcs01.und.edu/~ronald.marsh/CLASS/CS451/hw3-data.txt");
	//open data from webpage to work with
 	ifstream file;
	file.open("hw3-data.txt");
	if(!file.is_open())
	{
		cout<<"File failed to open" <<endl;
		exit(1);
	}
	
	//read file to get number of characters and place in character array	
	char* chars_in_file = get_char_array(file);
	//convert the character array to a string to make it easier to work with
	string chars_as_string = chars_in_file + '\0';
	//Uncomment to see what the array is that get returned from the function.	
	//cout << chars_in_file<<endl;
	//Create an array of words to be searched for. 
	//To increase the number of words searched for just change the defines 
	//at the beginning of the file to the desired number of words and add the words to this array.
	string search_words[NUM_SEARCH_WORDS] = {"easy","polar"} ;
	cout <<"Searching for words: " <<search_words[0]<<" and "<<search_words[1]<<endl;
	cout<<"-----------------------------------------"<<endl;
	
	//Start doing things to set up threads
	struct thread_data data[NUM_THREADS];	
	pthread_t threads[NUM_THREADS];	
	
	//Create the threads
	for(int i = 0; i < NUM_THREADS; i++)
	{
		cout << "Creating Thread: " << i+1 <<endl;		
		cout << "Searching for word: " << search_words[i] << endl;
		//putting data into struct for each thread			
		data[i].search_word = search_words[i];
		data[i].file_contents = chars_as_string;
		data[i].thread_num = i+1;
		//function call that creates the threads
		pthread_create(&threads[i], NULL, search_for_word, (void *)&data[i]);
	}
	for(int i = 0; i < NUM_THREADS; i++)
	{
		//once threads finish executing, rejoin them with the main thread
		pthread_join(threads[i], NULL);
	}

	//Display results
	for(int i = 0; i < NUM_THREADS; i++){
		cout<<"-----------------------------------------"<<endl;		
		cout<<"Results for thread " <<data[i].thread_num <<endl;
		cout<<"Matches for the word \""<< data[i].search_word<<"\" in file: "<<data[i].num_matches<<endl;
	}
	//exit threads
	pthread_exit(NULL);

	//Clean up
	delete [] chars_in_file;
	unlink("hw3-data.txt");
	file.close();
	return 0;
}
void * search_for_word(void * arg)
{
	//declare variables
	int count=0;
	int array_size = 0;
	int exit_code = 1;
	string search_word;
	//create data structure and convert from void * to something more useful
	struct thread_data *data;
	data = ((thread_data*)arg);
	search_word = data->search_word;
	string data_as_string = data->file_contents;

	//indicate which thread is executing
	cout << "Thread # "<<data->thread_num<<" is executing: " <<"..."<<endl;

	//break the scentence into words to compare to search word
	string temp = "";
	for (auto i : data_as_string)
	{
		if(i == ' ')
		{
			if(temp == search_word)
			{	
				//uncomment to display word each time a match is found		
				//cout << temp << endl;
				count ++;
			}
			temp = "";		
		}
		else
		{
			temp = temp + i;
		}
	}



	data->num_matches = count;
	pthread_exit(&exit_code);

}
char * get_char_array(ifstream &file){
	int num_char=0;
	char temp_char;
	char* char_array = NULL;	
	while(!file.eof())
	{
		file.get(temp_char);
		if(!(temp_char == '\n'))
		{
			if(char_array == NULL)
			{
				char_array = new char[1];
				char_array[num_char] = char(tolower(temp_char));		
				num_char++;
			}
			else
			{
				char * temp_char_array = char_array;
				char_array = new char[num_char+1];
				for(int i = 0; i < num_char; i++)
				{
					char_array[i] = temp_char_array[i];			
				}
				char_array[num_char] = char(tolower(temp_char));
				num_char++;
				delete [] temp_char_array;
			}
			
		}
		else 
		{
			if(char_array == NULL)
			{
				char_array = new char[1];
				char_array[0] = temp_char;
				num_char++;
			}
			else
			{
				char * temp_char_array = char_array;
				char_array = new char[num_char+1];
				for(int i = 0; i < num_char; i++)
				{
					char_array[i] = temp_char_array[i];			
				}
				char_array[num_char] = temp_char;
				num_char++;
				delete [] temp_char_array;
			}
		}
	}
	cout<< "Number of characters in file: "<<num_char<<endl;
	return char_array;
}
