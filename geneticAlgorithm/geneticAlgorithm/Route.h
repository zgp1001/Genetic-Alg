#include <string.h>
#include <cctype>
#include "City.h"
using namespace std;

#ifndef ROUTE
#define ROUTE

class Route{
private:
	City* cities;
	int numCities;

public:
	Route();	//no arg constructor
	Route(City*, int);	//Argumented constructor
	Route(Route&);	//copy Constructor
	~Route();	//destructor

	//operator overload
	Route& operator=(const Route&);

	//setters
	void setCities(City*);
	void setCityAt(City, int);
	void setNumCities(int);

	//getters
	City* getCities();
	City getCityAt(int);
	int getNumCities();
	float getDistance();

};
#endif
