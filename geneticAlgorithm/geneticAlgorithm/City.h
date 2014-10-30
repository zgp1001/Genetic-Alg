#include <string> 
#include <cctype>		//Used for type checking 
using namespace std;	

#ifndef CITY
#define CITY

class City
{
public:
	//Constructors 
	City();	  //No-Arg constructor (does not set any values) 
	City(float, float);   //Parameters = X, Y 
	City(string, int, float, float);	//All Parameters
	
	//assignment operator overload
	City operator=(City);

	//Setters
	void setX(float);
	void setY(float);
	void setName(string);
	void setId(int);

	//Getters 
	float getX();
	float getY();
	int getId();
	string getName();  

private:
	float x;
	float y; 
	int id;
	string name;
	const float MAX_X = 10000;
	const float MAX_Y = 10000;
};
#endif
