#ifndef GeneticFuncs
#define GeneticFuncs
#include "Route.h"
#include "City.h"

Route edgeRecombination(Route, Route);  //Performs Edge Recombination Crossover. Parameters are parents, return value is generated child
bool validParents(Route, Route);		//Ensures parents are valid to "mate" (Size must be equal)
#endif
