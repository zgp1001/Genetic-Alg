#include "City.h"

//No-Arg constructor 
City::City()
{
	//Do nothing
}

//Argumented constructor 
City::City(float xVal, float yVal){
	if (xVal >= 0 && xVal < MAX_X)
		x = xVal;
	if (yVal >= 0 && yVal < MAX_Y)
		y = yVal;
}

//Setters defined below 
void City::setId(int i){ id = i; }
void City::setName(string n){ name = n; }

void City::setX(float newX){
	if (newX >= 0 && newX < MAX_X)
		x = newX;
}

void City::setY(float newY){
	if (newY >= 0 && newY < MAX_Y)
		y = newY;
}

//Getters 
int City::getId(){ return id; }
float City::getX() { return x; }
float City::getY() { return y; }
string City::getName() { return name; }

