#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <omp.h> 
#include <string.h>
#include <time.h>
using namespace std;

const int NUM_GENERATION_STOPPER = 100;	
const int NUM_ROUTES = 200;		//Size of parent population (usually ~number of cities ^2 is a good starting point) 
const int NUM_CITIES = 29;			//Must be set equal to data set in string FILE_NAME
const string FILE_NAME = "29C27603.txt";	//Name of TSPLIB formatted data file 
const int NUM_THREADS = 4;			//Controls the level of parallel (OMP_SET_NUM_THREADS uses this value)  

//Bubble sort 
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

	/*for (int i = 0; i < NUM_ROUTES - 1; i++)
		cout << a[i].getDistance() << endl;
	cout << endl << endl << endl << endl;*/
}


void swap(Route* a, Route* b)
{
	Route* temp;
	temp = a;
	a = b;
	b = temp;
	delete temp; 
}

/*
Merge Sort adapted from code found here (https://github.com/cjenkin1/parallel-j/blob/master/OpenMP/mergeSort-omp.c) 

Code needed to be changed a bit to work for our problem. 
*/
void merge(Route *a, int size, Route *temp) {
	int i1 = 0;
	int i2 = size / 2;
	int it = 0;

	while (i1 < size / 2 && i2 < size) {
		if (a[i1].getDistance() <= a[i2].getDistance()) {
			temp[it] = a[i1];
			i1 += 1;
		}
		else {
			temp[it] = a[i2];
			i2 += 1;
		}
		it += 1;
	}

	while (i1 < size / 2) {
		temp[it] = a[i1];
		i1++;
		it++;
	}
	while (i2 < size) {
		temp[it] = a[i2];
		i2++;
		it++;
	}

	for (int i = 0; i < size; i++){
		a[i] = temp[i];
	}
}

void mergesort_serial(Route *a, int size, Route *temp) {

	if (size <= 2) {
		if (a[0].getDistance() <= a[1].getDistance())
			return;
		else {
			swap(a[0], a[1]);
			return;
		}
	}

	mergesort_serial(a, size / 2, temp);
	mergesort_serial(a + size / 2, size - size / 2, temp);
	merge(a, size, temp);
}



void mergesort_parallel_omp
(Route *a, int size, Route *temp, int threads) {

	if (threads == 1) {
		mergesort_serial(a, size, temp);
	}
	else if (threads > 1) {
#pragma omp parallel sections
		{
#pragma omp section
			mergesort_parallel_omp(a, size / 2, temp, threads / 2);
#pragma omp section
			mergesort_parallel_omp(a + size / 2, size - size / 2,
				temp + size / 2, threads - threads / 2);
		}
		merge(a, size, temp);
	} // threads > 1
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
	
	//Set OMP Num Threads 
	omp_set_num_threads(NUM_THREADS);
	omp_set_nested(1);

	//Seed random number generator 
	srand(time(NULL));

	int index;
	City tempCityAry[NUM_CITIES];
	Route tempRoute = Route(tempCityAry, NUM_CITIES);

	int generationCounter = 0;//counts the number of generations since better child was found
	Route * bestRoute;
	float currentBestChild = 0;
	Route * routeAry = new Route[NUM_ROUTES];
	Route * nextGeneration = new Route[NUM_ROUTES/2];
	Route temp[NUM_ROUTES];

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

	mergesort_parallel_omp(routeAry, NUM_ROUTES, temp, NUM_THREADS);
	currentBestChild = routeAry[0].getDistance();
	cout << "Initial best parent: " << currentBestChild << endl;
	
	while (generationCounter < NUM_GENERATION_STOPPER)
	{
		//create new generation
		#pragma omp parallel for
		for (int i = 0; i < NUM_ROUTES/2; i++)
		{
			nextGeneration[i] = edgeRecombination(routeAry[i*2], routeAry[(i*2)+1]);
		}

		#pragma omp parallel for
		for(int i = 0; i < NUM_ROUTES/2; i++)
			routeAry[(NUM_ROUTES/2)+i] = nextGeneration[i];

		//sort routes
		//bubbleSort(routeAry);
		mergesort_parallel_omp(routeAry, NUM_ROUTES, temp, NUM_THREADS);

		//Best route will be the first one in RouteAry after sorting 
		bestRoute = &routeAry[0];
		//cout << bestRoute->getDistance() << endl;
		if(bestRoute->getDistance() < currentBestChild)  //test for better child
		{
			generationCounter = 0;
			currentBestChild = bestRoute->getDistance();
		}
		else
		{
			generationCounter++;
		}
	}

	cout << currentBestChild << endl;

	stopTime = clock();
	float seconds = (float)(stopTime - startTime) / CLOCKS_PER_SEC;
	cout << endl << "Solution took: " << seconds << " second" << endl;

	delete [] routeAry;
	delete [] nextGeneration;
	
	system("pause");
	return 0;
}
