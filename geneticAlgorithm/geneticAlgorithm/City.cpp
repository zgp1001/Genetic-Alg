#include "City.h"

//No-Arg constructor 
City::City()
{
	//Do Nothing
}

//Argumented constructor 
City::City(float xVal, float yVal){
	setX(xVal);
	setY(yVal);
}

//Fully-Argumented Constructor
City::City(string n, int i, float xIn, float yIn){
	name = n;
	id = i;
	setX(xIn);
	setY(yIn);
}

//assignment operator overload (makes route simpler)
City City::operator=(City toBeCopied){
	if (this != &toBeCopied){
		this->id = toBeCopied.id;
		this->name = toBeCopied.name;
		this->x = toBeCopied.x;
		this->y = toBeCopied.y;
	}
	return *this;
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

