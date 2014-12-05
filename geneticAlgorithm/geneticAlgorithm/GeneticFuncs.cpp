#include "GeneticFuncs.h"
#include <iostream> 
#include <algorithm> // for std::find
using namespace std;

Route edgeRecombination(const Route &a, const Route &b);
bool validParents(const Route &a, const Route &b);
int getIndex(int id, int **edges, int cities);

/*Performs Edge Recombination Crossover
	Parameters = Pair of parents   
	Return Value = Generated child */
Route edgeRecombination(const Route &a, const Route &b) {  
	/*Edge recombination works by selecting a possible path from a given city based on paths that exist in either parent. 
	  When selecting which city to travel to next out of the possibly cities preference is given to cities with less connections
	  stemming from them. As cities are traveled to they are removed from the list of possibly cities. */

	Route child = Route();

	if (!validParents(a, b)){  //Both routes must have equal amount of cities (if not, exit) 
		cout << "\nINVALID PARENTS PASSED TO edgeRecombination()\n";
		return child;
	}

	//Begin implemention of cross-over 
	int numCities = a.getNumCities();  
	int **connections = new int*[numCities];  //This will hold the possible connections which is the Union of the parent tours
	int possibleConnections = 5;  //Every city can only possibly be connected to 4 other cities when combining parents (5th field is for ID)

	for (int i = 0; i < numCities; i++){
		connections[i] = new int[numCities];
		connections[i][0] = a.getCityAt(i).getId();  //Store the ID of cities from Tour A in the first column of array
	}

	for (int i = 0; i < numCities; i++){
		int toFind = connections[i][0];		 //ID to search for in this iteration 
		int vals[4] = { 0, 0, 0, 0 };		//Holds connections found for this ID 
		#pragma omp parallel for
		for (int j = 0; j < numCities; j++){
			if (a.getCityAt(j).getId() == toFind)
				if (j == numCities - 1){
					vals[0] = a.getCityAt(0).getId();
					vals[1] = a.getCityAt(j - 1).getId();
				}
				else if (j == 0){
					vals[0] = a.getCityAt(j + 1).getId();
					vals[1] = a.getCityAt(numCities - 1).getId();
				}
				else{
					vals[0] = a.getCityAt(j + 1).getId();
					vals[1] = a.getCityAt(j - 1).getId();
				}
			if (b.getCityAt(j).getId() == toFind)
				if (j == numCities - 1){
					vals[2] = b.getCityAt(0).getId();
					vals[3] = b.getCityAt(j - 1).getId();
				}
				else if (j == 0){
					vals[2] = b.getCityAt(j + 1).getId();
					vals[3] = b.getCityAt(numCities - 1).getId();
				}
				else{
					vals[2] = b.getCityAt(j + 1).getId();
					vals[3] = b.getCityAt(j - 1).getId();
				}
		}
		for (int l = 1; l < possibleConnections; l++){
			if (vals[l - 1] != 0 && !inArray(vals[l - 1], connections[i], possibleConnections))
				connections[i][l] = vals[l - 1];
			else
				connections[i][l] = NULL;
		}
	}

	//Pick a random start city from the Routes 
	int index;
	int nextIndex;
	int min;
	int bestId;
	int count;
	int numCons;
	int random = rand() % 2; 
	
	if (random == 0){
		child.addCity(a.getCityAt(0));
		bestId = a.getCityAt(0).getId();
	}
	else{
		child.addCity(b.getCityAt(0));
		bestId = b.getCityAt(0).getId();
	}

	//Now remove references to this city from other cities 
	#pragma omp parallel for
	for (int cities = 0; cities < numCities; cities++){
		for (int links = 1; links < possibleConnections; links++){
			if (connections[cities][links] == bestId)
				connections[cities][links] = NULL;
		}
	}

	for (int i = 0; i < numCities-1; i++){
		index = getIndex(child.getCityAt(i).getId(), connections, numCities);
		//cout << index << endl;
		min = possibleConnections - 1;
		numCons = 0;
		bestId = NULL;
		for (int cons = 1; cons < possibleConnections; cons++){
			if (connections[index][cons] != NULL){
				//cout << "connection found between city " << connections[index][0] << " and city " << connections[index][cons] << endl;
				numCons++;
				nextIndex = getIndex(connections[index][cons], connections, numCities);
				count = 0;
				for (int test = 1; test < possibleConnections; test++){
					if (connections[nextIndex][test] != NULL){
						count++;
					}
				}
				if (count == min){
					bestId = NULL;
				}
				else if (count < min){
					min = count;
					bestId = connections[index][cons];
				}
			}
		}
		//Add best city if one was found
		if (min != possibleConnections - 1 && bestId != NULL){
			//cout << "adding child" << endl;
			child.addCity(a.getCityByID(bestId));
		}
		//Add random city (no clear best)
		else if(numCons > 0){
			random = rand() % (possibleConnections - 1) + 1; 
			while (connections[index][random] == NULL)
				random = rand() % (possibleConnections - 1) + 1;
			bestId = connections[index][random];
			//cout << "Best city found = " << a.getCityByID(bestId).getId() << endl;
			child.addCity(a.getCityByID(bestId));
		}
		else {
			//cout << "No connections on " << child.getCityAt(i).getId() << endl;
			int childCities = child.getNumCities();
			int *posIds = new int[numCities - childCities];
			int search;
			int foundNum = 0;
			bool found = false;
			for (int c = 0; c < numCities; c++){
				search = connections[c][0]; 
				for (int n = 0; n < childCities; n++){
					if (child.getCityAt(n).getId() == search)
						found = true;
				}
				if (!found){
					posIds[foundNum] = search;
					foundNum++;
				}
				else {
					found = false;
				}
			}
			random = rand() % (numCities - childCities);
			bestId = posIds[random];
			child.addCity(a.getCityByID(bestId));
		}
		//Now remove references to this city from other cities 
		#pragma omp parallel for
		for (int cities = 0; cities < numCities; cities++){
			for (int links = 1; links < possibleConnections; links++){
				if (connections[cities][links] == bestId)
					connections[cities][links] = NULL;
			}
		}
	}

	//Delete dynamically created memory to avoid leaks
	for (int i = 0; i < numCities; i++){
		delete [] connections[i];
	}
	delete [] connections;

	return child;  
} 

int getIndex(int id, int** edges, int cities){
	int ret;
	for (int i = 0; i < cities; i++){
		if (edges[i][0] == id)
			ret = i;
	}
	return ret;
}

bool inArray(int search, int *row, int len){
	bool ret = false;
	for (int i = 0; i < len; i++){
		if (row[i] == search)
			ret = true;
	}
	return ret;
}

//Ensures that parents are valid match to "mate". Routes must have the same number of cities to be mateable 
bool validParents(const Route &a, const Route &b){
	bool ret = false;
	if (a.getNumCities() == b.getNumCities())
		ret = true;
	return ret;
}