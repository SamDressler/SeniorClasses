//Sam Dressler 
//CSCI 451
//9/1/2020
#include <iostream>
#include <cstdlib>
#include <fstream>
using std::cout;
int main()
{
	
// Download Webpage
	cout << "hi";
	system("wget -q http://www.senate.gov/general/contact_information/senators_cfm.cfm");
	std::ifstream file;
	file.open("senators_cfm.cfm");
	if(!file.is_open());
	return 0;
}
