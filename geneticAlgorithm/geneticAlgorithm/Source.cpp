/*************************
TSP GA Group 
__________________________

Alex Lee 
Zachary Petrusch 
Eric McAlpine 
Nick Benedict
__________________________
This code executes a genetic algorithm in parallel, using OpenMP to solve the Traveling Salesman Problem. 

Instructions on running and modifying the project can be found in the User and Technical Documents
___________________________

**************************/

#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <omp.h> 
#include <string.h>
#include <time.h>
using namespace std;

const int NUM_GENERATION_STOPPER = 10;	
const int NUM_ROUTES = 1000;		//Size of parent population (usually ~number of cities ^2 is a good starting point) 
const int NUM_CITIES = 70;			//Must be set equal to data set in string FILE_NAME
const string FILE_NAME = "70C675.txt";	//Name of TSPLIB formatted data file 
const int NUM_THREADS = 4;			//Controls the level of parallel (OMP_SET_NUM_THREADS uses this value)  

//Bubble sort for fun
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
	//create city array
	ifstream myReadFile;
	string nextData;
	City theCityArray[NUM_CITIES];
	
	//Set OMP Num Threads 
	omp_set_num_threads(NUM_THREADS);
	omp_set_nested(1);

	//Seed random number generator 
	srand(time(NULL));

	int index;		//Used to create random routes 
	City tempCityAry[NUM_CITIES];	//Temp city array used for temporary storage of Cities 
	Route tempRoute = Route(tempCityAry, NUM_CITIES);	//Temp route array used for temporary storage of routes 

	int generationCounter = 0;	//counts the number of generations since better child was found
	Route * bestRoute;			//Stores the current best route 
	float currentBestChild = 0;	//Stores the fitness score (Distance of route) of the best route
	Route * routeAry = new Route[NUM_ROUTES];	//Array that stores the population 
	Route * nextGeneration = new Route[NUM_ROUTES/2];	//Array that holds new population until they can be placed into routeAry
	Route temp[NUM_ROUTES];	//Used for merge sort
	int iterations = 0;		//Tracks the number of iterations the run took (iteration = 1 cycle through GA While loop)

	/****************************************
	Read in the data from the file specified 
	*****************************************/
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

	/****************************************
	End reading in data 
	*****************************************/

	/****************************************
	Initialize random population
	*****************************************/
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
	/****************************************
	End creation of population 
	*****************************************/

	//Sort the initia population and find the initial best solution 
	mergesort_parallel_omp(routeAry, NUM_ROUTES, temp, NUM_THREADS);
	currentBestChild = routeAry[0].getDistance();
	cout << "Initial best parent: " << currentBestChild << endl; //Print the best child of Gen I

	//Open a parallel region 
	//The GA runs in parallel with the cross-over function called in parallel to create the "next" generation faster
	#pragma omp parallel 
	{
		//Continues to run the GA until no improvement for NUM_GENERATION_STOPPER iterations
		while (generationCounter < NUM_GENERATION_STOPPER) 
		{
			//create new generation 
			#pragma omp for
			for (int i = 0; i < NUM_ROUTES / 2; i++)
			{
				nextGeneration[i] = edgeRecombination(routeAry[i * 2], routeAry[(i * 2) + 1]);
			}

			//Copy the newly generated routes into the population array, replacing the 50% worst solutions 
			#pragma omp for
			for (int i = 0; i < NUM_ROUTES / 2; i++)
				routeAry[(NUM_ROUTES / 2) + i] = nextGeneration[i];

			//Allow one thread to update all the GA variables and call the sort 
			#pragma omp single
			{
				iterations++;  //Increment iterations 

				//Sort the arrays so that the best solutions are at the top of the population array 
				//bubbleSort(routeAry);
				mergesort_parallel_omp(routeAry, NUM_ROUTES, temp, NUM_THREADS);

				//Best route will be the first one in RouteAry after sorting 
				bestRoute = &routeAry[0];

				//cout << bestRoute->getDistance() << endl;
				//See if the best solution is better than it was last generation, if not increment the generationCounter
				//When generationCounter exceeds NUM_GENERATION_STOPPER the GA stops executing 
				if (bestRoute->getDistance() < currentBestChild)  //test for better child
				{
					generationCounter = 0;
					currentBestChild = bestRoute->getDistance();
				}
				else
				{
					generationCounter++;
				}
			}
		}
	}

	//Stop the timer and get how long the program took to execute
	stopTime = clock(); 
	float seconds = (float)(stopTime - startTime) / CLOCKS_PER_SEC;

	//Print the stats for the run 
	cout << "Solution = " << currentBestChild << endl;
	cout << endl << "Solution took: " << seconds << " seconds" << endl;
	cout << "Took " << iterations << " iterations\n";

	//Delete dynamic memory 
	delete [] routeAry;
	delete [] nextGeneration;
	
	system("pause");
	return 0;
}
