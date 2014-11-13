#include "Route.h"
#include <math.h>

//no-arg Constructor
Route::Route(){
	//nothing to do
	numCities = 0;
}

//Argumented Constructor
Route::Route(City* newCities, int numOfCities){
	if (numOfCities > 0){
		numCities = numOfCities;
		cities = new City[numCities];
		setCities(newCities);
	}
	else {
		numCities = 0;
	}
}

//copy constructor
Route::Route(Route& toBeCopied){
	setNumCities(toBeCopied.numCities);
	setCities(toBeCopied.cities);
}
/*
//Destructor
Route::~Route(){
	delete[] cities;
}*/

//assignment operator overload
Route& Route::operator=(const Route& toBeCopied){
	if (this != &toBeCopied){
		this->setNumCities(toBeCopied.numCities);
		this->setCities(toBeCopied.cities);
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
			if (numCities > 0){
				City* temp = cities;
				cities = new City[newNum];
				for (int i = 0; i < numCities; i++){	//for every existing city
					cities[i] = temp[i];//copy over city into new array
				}
				delete[] temp;
			}
			else
				cities = new City[newNum];
		}
		numCities = newNum;
	}
}

void Route::addCity(City  c){
	this->setNumCities(this->getNumCities() + 1);
	this->setCityAt(c, this->getNumCities() - 1);
}

//**********************Getters
City* Route::getCities() const{ return cities; }
City Route::getCityAt(int x) const { return cities[x]; }
City Route::getCityByID(int id) const {
	for (int i = 0; i < this->getNumCities(); i++)
		if (this->getCityAt(i).getId() == id)
			return this->getCityAt(i);
}
int Route::getNumCities() const { return numCities; }

float Route::getDistance() const {	//easiest to calculate whenever asked than to
	float distance=0.0;		//recalculate every time cities change
	for (int i = 0; i < numCities - 1; i++){
		distance += sqrt(((cities[i + 1].getX() - cities[i].getX())*
			(cities[i + 1].getX() - cities[i].getX()))
			+ ((cities[i + 1].getY() - cities[i].getY())*
			(cities[i + 1].getY() - cities[i].getY())));
	}
	distance += sqrt(((cities[0].getX() - cities[numCities - 1].getX())*
		(cities[0].getX() - cities[numCities - 1].getX()))
		+ ((cities[0].getY() - cities[numCities - 1].getY())*
		(cities[0].getY() - cities[numCities - 1].getY())));
	return distance;
}
