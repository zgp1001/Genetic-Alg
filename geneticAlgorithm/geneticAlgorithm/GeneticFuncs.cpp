#include "GeneticFuncs.h"
#include <iostream> 
#include <algorithm> // for std::find
#include <iterator>  //for std::begin and end
using namespace std;

Route edgeRecombination(const Route &a, const Route &b);
bool validParents(const Route &a, const Route &b);

/*Performs Edge Recombination Crossover
	Parameters = Pair of parents   
	Return Value = Generated child */
Route edgeRecombination(const Route &a, const Route &b) {  
	/*Edge recombination works by selecting a possible path from a given city based on paths that exist in either parent. 
	  When selecting which city to travel to next out of the possibly cities preference is given to cities with less connections
	  stemming from them. As cities are traveled to they are removed from the list of possibly cities. */

	if (!validParents(a, b)){  //Both routes must have equal amount of cities (if not, exit) 
		cout << "\nINVALID PARENTS PASSED TO edgeRecombination()\n";
		return Route();
	}

	//Begin implemention of cross-over 
	int numCities = a.getNumCities();  
	int **connections = new int*[numCities*2];  //This will hold the possible connections which is the Union of the parent tours
	int possibleConnections = 5;  //Every city can only possibly be connected to 4 other cities when combining parents (5th field is for ID)

	// ADD CITIES FROM ROUTE B TO CONNECTIONS ARRAY AS WELL 

	for (int i = 0; i < numCities; i++){
		connections[i] = new int[numCities];
		connections[i][0] = a.getCityAt(i).getId();  //Store the ID of cities from Tour A in the first column of array
	}

	for (int i = 0; i < numCities; i++){
		int toFind = connections[i][0];		 //ID to search for in this iteration 
		int vals[4] = { 0, 0, 0, 0 };		//Holds connections found for this ID 
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

	//Test print
	for (int i = 0; i < numCities; i++){
		for (int j = 0; j < 5; j++)
			cout << connections[i][j] << "     ";
		cout << endl;
	}
	return Route();  //TODO: Return child route 
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