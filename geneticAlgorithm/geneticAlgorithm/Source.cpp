#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 
#include <fstream>
#include <string.h>
#include <time.h>
using namespace std;
#include <conio.h>

const int NUM_GENERATION_STOPPER = 100;
const int NUM_ROUTES = 100;
const int NUM_CITIES = 29;

/*For purposes of TSP 
	City = a node on a graph 
	Route = a tour of Cities listed in traversal order (linked list, essentially) 
*/

void sortRouteArray();
/*
 * C++ Program to Implement Merge Sort
 */


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

/* MERGE SORT STUFFS
void merge(Route *,int, int , int );
void mergesort(Route *a, int low, int high)
{
    int mid;
    if (low < high)
    {
        mid=(low+high)/2;
        mergesort(a,low,mid);
        mergesort(a,mid+1,high);
        merge(a,low,high,mid);
    }
    return;
}
//MERGE SORT THE ARRAYS
void merge(Route *a, int low, int high, int mid)
{
    int i, j, k;
	Route *c = new Route[high-low];
    i = low;
    k = low;
    j = mid + 1;
	
    while (i <= mid && j <= high)
    {
        if (a[i].getDistance() < a[j].getDistance())
        {
            c[k] = a[i];
            k++;
            i++;
        }
        else
        {
            c[k] = a[j];
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        c[k] = a[i];
        k++;
        i++;
    }
    while (j <= high)
    {
        c[k] = a[j];
        k++;
        j++;
    }
    for (i = low; i < k; i++)
    {
        a[i] = c[i];
    }
}*/

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

	Route p1 = Route();
	Route p2 = Route();
	Route p3 = Route();
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

	myReadFile.open("29C27603.txt");

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

	/*
	TESTING GA CX	
	Route test;
	
	for (int i = 0; i < 29; i++){
		cout << routeAry[0].getCityAt(i).getId() << ",";
	}
	cout << "\nRoute 2\n";
	for (int i = 0; i < 29; i++){
		cout << routeAry[1].getCityAt(i).getId() << ",";
	}
	cout << endl;
	test = edgeRecombination(routeAry[0], routeAry[1]);

	cout << "\nRoute A Rating = " << routeAry[0].getDistance();
	cout << "\nRoute B Rating = " << routeAry[1].getDistance();
	cout << "\nRoute C Rating = " << test.getDistance() << endl;*/

	delete [] routeAry;
	delete [] tempRouteAry;
	
	system("pause");
	return 0;
}
