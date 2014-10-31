#include "City.h"			//Defines City Class
#include "Route.h"			//Defines Route Class
#include "GeneticFuncs.h"	//Holds GA Functions
#include <iostream> 

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
	
	City a = City(0.5, 120);
	a.setName("45");
	Route r = Route();
	r.addCity(a);
	r.addCity(a);
	City *ary = r.getCities();
	int size = r.getNumCities();
	for (int i = 0; i < size; i++){
		cout << "City name = " << ary[i].getName() << "\n";
	}
	//cout << "City name = " << r.getCityAt(1).getName() << "\n";
	testFunc();
	system("Pause");
	return 0;
}
