#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;

const int NUM_GENERATION_STOPPER = 100;	
const int NUM_ROUTES = 3000;		//Size of parent population (usually ~number of cities ^2 is a good starting point) 
const int NUM_CITIES = 51;		//Must be set equal to data set in string FILE_NAME
const string FILE_NAME = "51City426.txt";

/*
* C++ Program to Implement Merge Sort
*/
void sortRouteArray();

void bubbleSort(Route *a)
{
	Route swap;
	for (int c = 0; c < NUM_ROUTES - 1; c++)
	{
		for (int d = 0; d < NUM_ROUTES - c - 1; d++)
		{
			if (a[d].getDistance() > a[d + 1].getDistance()) /* For decreasing order use < */
			{
				swap = a[d];
				a[d] = a[d + 1];
				a[d + 1] = swap;
			}
		}
	}

	for (int i = 0; i < NUM_ROUTES - 1; i++)
		cout << a[i].getDistance() << endl;
	cout << endl << endl << endl << endl;
}

int main() {
	//while loop
	//pair best parents
	//cross parents
	//check for best children
	//half best parents, half best children

	//loop ends when NUM_GENERATIONS successive generations do not generate better child.

	
	
	//create city array
	ifstream myReadFile;
	string nextData;
	City theCityArray[NUM_CITIES];
	
	//Seed random number generator 
	srand(time(NULL));

	int index;
	City tempCityAry[NUM_CITIES];
	Route tempRoute = Route(tempCityAry, NUM_CITIES);

	int generationCounter = 0;//counts the number of generations since better child was found
	Route * bestRoute;
	float currentBestChild = 0;
	Route * routeAry = new Route[NUM_ROUTES];
	Route * tempRouteAry = new Route[NUM_ROUTES];

	int col = 1;
	int i = 0;

	myReadFile.open(FILE_NAME);

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

	clock_t stopTime, startTime;
	startTime = clock();  //Start the timer
	//fill routes
	for (int i=0; i<NUM_ROUTES; i++)//for each route
	{
		for (int j = 0; j < NUM_CITIES; j++){	//set temporary city array
			tempCityAry[j] = theCityArray[j];
		}
		for (int j = 0; j < NUM_CITIES; j++){	//loop through cities
			//pick a city and put it in route
			index = rand() % (NUM_CITIES - j);
			tempRoute.setCityAt(tempCityAry[index], j);

			//shrink array
			for (int k = index; k < NUM_CITIES - (j + 1); k++){//gets shorter every time
				tempCityAry[k] = tempCityAry[k + 1];
			}
		}
		routeAry[i] = tempRoute;	//assign route into array
		routeAry[i].setNumCities(NUM_CITIES);//can probably delete (not sure)
	}

	
	
	while (generationCounter < NUM_GENERATION_STOPPER)
	{
		//create new generation
		for (int i = 0; i < NUM_ROUTES/2; i++)
		{
			tempRouteAry[i] = edgeRecombination(routeAry[i*2], routeAry[(i*2)+1]);
		}

		//sort routes
		//mergesort(routeAry, 0, NUM_CITIES - 1);
		bubbleSort(routeAry);

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

	cout << bestRoute->getDistance() << endl;

	stopTime = clock();
	float seconds = (float)(stopTime - startTime) / CLOCKS_PER_SEC;
	cout << endl << "Solution took: " << seconds << " second" << endl;

	delete [] routeAry;
	delete [] tempRouteAry;
	
	system("pause");
	return 0;
}
