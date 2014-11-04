#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <string.h>

const int NUM_GENERATION_STOPPER = 100;
const int NUM_ROUTES = 100;
const int NUM_CITIES = 29;

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
	City theCityArray[NUM_CITIES];

	int col = 1;
	int i = 0;

	myReadFile.open("29CitiesData.txt");

	if(myReadFile.is_open()){
		while(myReadFile >> nextData){
			if(col == 1){
				theCityArray[i].setId(atoi(nextData.c_str()));
				//cout << nextData << endl;
				col++;
			}
			else if(col == 2){
				theCityArray[i].setX(atof(nextData.c_str()));
				//cout << nextData << endl;
				col++;
			}
			else if(col == 3){
				theCityArray[i].setY(atof(nextData.c_str()));
				//cout << nextData << endl;
				col=1;
				i++;
			}
			else
				cout << "Problem";
		}//while has next data
	}//if file open
	else {
		cout << "File failed to open\n";
	}

	Route p1 = Route();
	Route p2 = Route();
	Route p3 = Route();
	for (int i = 0; i < NUM_CITIES; i++){
		p1.addCity(theCityArray[i]);
		p2.addCity(theCityArray[28 - i]);
	}

	p3 = edgeRecombination(p1, p2);
	
	int generationCounter=0;//counts the number of generations since better child was found
	Route * bestRoute;
	float currentBestChild=0;
	Route * routeAry = new Route[NUM_ROUTES];
	Route * tempRouteAry = new Route[NUM_ROUTES];

	//fill routes
	for (int i=0; i<NUM_ROUTES; i++)
	{
		routeAry[i] = Route(theCityArray, NUM_CITIES);
	}

	while (generationCounter < NUM_GENERATION_STOPPER)
	{
		//create new generation
		for (int i = 0; i < NUM_ROUTES/2; i++)
		{
			tempRouteAry[i] = edgeRecombination(routeAry[i*2], routeAry[(i*2)+1]);
		}
		for (int j = NUM_ROUTES/2; j < NUM_ROUTES; j++)
		{
			//routeAry.sort(); a sort function would be nice smallest distance first
			tempRouteAry[j] = routeAry[j-(NUM_ROUTES/2)];//should be parent. find best parents idk
		}

		//update Route ary
		for (int l = 0; l < NUM_ROUTES; l++)
		{
			routeAry[l] = tempRouteAry[l];
		}

		//find best route
		bestRoute = routeAry;
		for (int k = 1; k < NUM_ROUTES; k++)
		{
			if (routeAry->getDistance() < bestRoute->getDistance())
			{
				bestRoute = &routeAry[k];
			}
		}

		if(bestRoute->getDistance() < currentBestChild)//test for better child
		{
			generationCounter = 0;
			currentBestChild = bestRoute->getDistance();
		}
		else
		{
			generationCounter++;
		}
	}

	delete [] routeAry;
	delete [] tempRouteAry;
	
	system("Pause");
	return 0;
}
