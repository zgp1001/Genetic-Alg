#include "Route.h"
#include <math.h>

//no-arg Constructor
Route::Route(){
	//nothing to do
}

//Argumented Constructor
Route::Route(City* newCities, int numOfCities){
	if (numOfCities > 0){
		numCities = numOfCities;
		cities = new City[numCities];
		setCities(newCities);
	}
}

//copy constructor
Route::Route(Route& toBeCopied){
	setNumCities(toBeCopied.numCities);
	setCities(toBeCopied.cities);
}

//Destructor
Route::~Route(){
	delete[] cities;
}

//assignment operator overload
Route& Route::operator=(const Route& toBeCopied){
	if (this != &toBeCopied){
		this->numCities = toBeCopied.numCities;
		this->cities = new City[this->numCities];
		for (int i = 0; i < this->numCities; i++){
			this->cities[i] = toBeCopied.cities[i];
		}
	}
	return *this;
}

//***********************Setters
void Route::setCities(City* newCities){
	for (int i = 0; i < numCities; i++){
		cities[i] = newCities[i];
	}
}

void Route::setCityAt(City newCity, int index){
	if (index >= 0 && index < numCities){	//bounds check
		cities[index] = newCity;
	}
}

void Route::setNumCities(int newNum){
	if (newNum > 0){	//bounds check
		//recreate cities array if size is bigger
		//if size is smaller, rest of array can remain empty
		if (newNum > numCities){
			City* temp = cities;
			cities = new City[newNum];
			for (int i = 0; i < numCities; i++){	//for every existing city
				cities[i] = temp[i];//copy over city into new array
			}
			delete[] temp;
		}
		numCities = newNum;
	}
}

//**********************Getters
City* Route::getCities(){ return cities; }
City Route::getCityAt(int x){ return cities[x]; }
int Route::getNumCities(){return numCities;}

float Route::getDistance(){	//easiest to calculate whenever asked than to
	float distance=0.0;		//recalculate every time cities change
	for (int i = 0; i < numCities - 1; i++){
		distance += sqrt(((cities[i + 1].getX() - cities[i].getX())*
			(cities[i + 1].getX() - cities[i].getX()))
			+ ((cities[i + 1].getY() - cities[i].getY())*
			(cities[i + 1].getY() - cities[i].getY())));
	}
	return distance;
}
