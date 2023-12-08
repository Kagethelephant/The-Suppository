#pragma once

#include "math.hpp"

//this function generates a random integer between the 2 provided integer's
int randRange(int _min, int _max)
{
	// obtain a random number from hardware
	std::random_device rd;
	// seed the generator
	std::mt19937 gen(rd());
	// define the range
	std::uniform_int_distribution<> distr(_min, _max);

	return distr(gen);
}