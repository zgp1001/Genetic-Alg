#ifndef GeneticFuncs
#define GeneticFuncs
#include "Route.h"
#include "City.h"

Route edgeRecombination(const Route &, const Route &);  //Performs Edge Recombination Crossover. Parameters are parents, return value is generated child
bool validParents(const Route &, const Route &);		//Ensures parents are valid to "mate" (Size must be equal)
bool inArray(int, int*, int);
#endif
