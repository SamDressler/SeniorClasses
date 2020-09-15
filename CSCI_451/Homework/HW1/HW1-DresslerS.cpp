//Sam Dressler 
//CSCI 451
//9/3/2020
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
using namespace std;
int main()
{
	
	// Download Webpage
	system("wget -q http://www.senate.gov/general/contact_information/senators_cfm.cfm");
	std::ifstream file;
	//Parse downloaded file and moving to results file
	system("grep -Eo \"(http|https)://((\\w)*|([0-9]*)|([-|_])*)+([\\.|/]((\\w)*|([0-9]*)|([-|_])*))+\" senators_cfm.cfm > results.txt"); 
	//Open results file
	file.open("results.txt");
	if(!file){
		cout << "File could not be opened\n";
		exit(1);
	}
	else{
	//Display each of the resulting web files
		cout << "Download successful!\nParsed results:\n";
		string temp;
		int count =0;
		while(getline(file,temp))
		{
			cout << temp << "\n";	
			count++;
		}	
		cout << "--------------------------------\n";
		cout << "Total web adresses found: "<<count <<"\n";
		cout << "Removing temp files...\n";
	}
	unlink("results.txt");
	unlink("senators_cfm.cfm");
	return 0;
}
