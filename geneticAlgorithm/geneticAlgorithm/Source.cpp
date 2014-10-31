#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
//#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <string.h>

/*For purposes of TSP 
	City = a node on a graph 
	Route = a tour of Cities listed in traversal order (linked list, essentially) 
*/

int main() {


	//set up city array
	//generate routes

	//while loop
	//pair best parents
	//cross parents
	//check for best children
	//half best parents, half best children

	//loop ends when 50 successive generations do not generate better child.

	
	
//create city array
	ifstream myReadFile;
	string nextData;
	City theCityArray[29];

	int col = 1;
	int i = 0;

	myReadFile.open("29CitiesData.txt");

	if(myReadFile.is_open()){
		while(myReadFile >> nextData){
			if(col == 1){
				theCityArray[i].setId(atoi(nextData.c_str()));
				cout << nextData << endl;
				col++;
			}
			else if(col == 2){
				theCityArray[i].setX(atof(nextData.c_str()));
				cout << nextData << endl;
				col++;
			}
			else if(col == 3){
				theCityArray[i].setY(atof(nextData.c_str()));
				cout << nextData << endl;
				col=1;
				i++;
			}
			else
				cout << "YA DONE FUCKED UP M8!";
		}//while has next data
	}//if file open

	for(int i = 0; i < 29; i++)
	{
		cout << theCityArray[i].getId() << endl;
	}

	//cout << "City name = " << r.getCityAt(1).getName() << "\n";
	//testFunc();
	system("Pause");
	return 0;
}
