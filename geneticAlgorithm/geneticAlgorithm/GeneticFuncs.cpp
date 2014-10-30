#include "GeneticFuncs.h"
#include <iostream> 
using namespace std;

Route edgeRecombination(Route a, Route b);
bool validParents(Route a, Route b);

/*Performs Edge Recombination Crossover
	Parameters = Pair of parents   
	Return Value = Generated child */
Route edgeRecombination(Route a, Route b) {  
	/*Edge recombination works by selecting a possible path from a given city based on paths that exist in either parent. 
	  When selecting which city to travel to next out of the possibly cities preference is given to cities with less connections
	  stemming from them. As cities are traveled to they are removed from the list of possibly cities. */
	if (validParents(a, b))  //Both routes must have equal amount of cities 
		int numCities = a.getNumCities();
	else
		cout << "\nINVALID PARENTS PASSED TO edgeRecombination()\n";
} 

//Ensures that parents are valid match to "mate". Routes must have the same number of cities to be mateable 
bool validParents(Route a, Route b){
	bool ret = false;
	if (a.getNumCities() == b.getNumCities())
		ret = true;
	return ret;
}