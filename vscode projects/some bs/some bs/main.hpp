#pragma once

//SFML
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"

//General
#include "wtypes.h"
#include "winuser.h"
#include <iostream>
#include <string>
#include <random>

class game  
{

public:
	
	//this function generates a random integer between the 2 provided integer's
	int RandRange(int min, int max)
	{
		// obtain a random number from hardware
		std::random_device rd; 
		// seed the generator
		std::mt19937 gen(rd()); 
		// define the range
		std::uniform_int_distribution<> distr(min, max); 
		
		return distr(gen);
	}

};

class DiamSquare
{
private:
	
public:

	 //alocate space for the grid
	static const int arraySize = 200;
	//create the grid here so it stays in the heap
	float map[arraySize][arraySize] = { -1000.0f };

	//----CREATE THE MAP----
	void newMap(int size, int high = 20, float roughness = 20, float change = 1.6)
	{
		//---INITIALIZE VARIABLES----
		game rand;
		float avg;
		int div;

		//points to extrapolate from for square step
		float x1y1;
		float x2y1;
		float x1y2;
		float x2y2;

		//points to extrapolate from for diamond step
		float x1y;
		float x2y;
		float xy1;
		float xy2;

		//create a chunk the size of the grid that will be halfed every itteration
		float chunk = size; 
		//to find the center of the chunk we are working with
		float half = size/2; 
		//to initialize the grid so we know if we missed one
		float empty = -1000.0f;

		//---INITIALIZE MAP----

		//initialize the grid (1 cell grater then the size in the x and y ge
		for (int z = 0; z < size+1; z += 1) { for (int i = 0; i < size+1; i += 1) { map[i][z] = empty; } }

		//set initial corner values to extrapolate from at each corner
		map[0][0] = rand.RandRange(-roughness, roughness);
		map[0][size] = rand.RandRange(-roughness, roughness);
		map[size][0] = rand.RandRange(-roughness, roughness);
		map[size][size] = rand.RandRange(-roughness, roughness);

		//----SQUARE-----

		while (chunk > 1)
		{		
			for (float z = 0; z < size; z += chunk)
			{
				for (float i = 0; i < size; i += chunk)
				{

					// 0 - - - 0	This is the square portion of the code the "X" represents
					// - - - - -	points that have already been populated. these points are 
					// - - - - -	averaged and a random value is added "roughness" to generate
					// - - - - -	the "0" values. This is the initialization done above
					// 0 - - - 0

					// X - 0 - X	This is what the first iteration in this loop will look like
					// - - - - -	 
					// 0 - X - 0	
					// - - - - -	
					// X - 0 - X
					

					//grab the "X" values shown above
					x1y1 = map[(int)round(i)][(int)round(z)]; 
					x2y1 = map[(int)round(i + chunk)][(int)round(z)];
					x1y2 = map[(int)round(i)][(int)round(z + chunk)];
					x2y2 = map[(int)round(i + chunk)][(int)round(z + chunk)];

					avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

					//set the value "0" with the average + a random value and make sure we are not overwriting other values
					if (map[(int)round(i + half)][(int)round(z + half)] == empty) map[(int)round(i + half)][(int)round(z + half)] = avg + rand.RandRange(-roughness, roughness);
				}
			}
			
			//cntDiam = 0;
			//DIAMOND
			for (float z = 0; z < size + 1; z += half)
			{
				for (float i = fmodf((z + half), chunk); i < size + 1; i += chunk)
				{
					// X - - - X	This is the diamond portion of the code the "X" represents
					// - - - - -	points that have already been populated. these points are 
					// - - 0 - -	averaged and a random value is added "roughness" to generate
					// - - - - -	the "0" values.
					// X - - - X

					// X - X - X	This is what the next iteration would look like
					// - 0 - 0 -	 
					// X - X - X	
					// - 0 - 0 -	
					// X - X - X

					x1y = 0;
					x2y = 0;
					xy1 = 0;
					xy2 = 0;

					div = 0;

					//grab the "X" values shown above if they are in the map bounds
					if (i - half >= 0)
					{
						x1y = map[(int)round(i - half)][(int)round(z)];
						div += 1;
					}
						
					if (i + half < size + 1)
					{
						x2y = map[(int)round(i + half)][(int)round(z)];
						div += 1;
					}

					if (z + half < size + 1)
					{
						xy1 = map[(int)round(i)][(int)round(z + half)];
						div += 1;
					}

					if (z - half >= 0)
					{
						xy2 = map[(int)round(i)][(int)round(z - half)];
						div += 1;
					}

					//calculate the average and use div to only average the amount of values grabbed
					avg = (x1y + x2y + xy1 + xy2) / div; 

					//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
					if (map[(int)round(i)][(int)round(z)] == empty) map[(int)round(i)][(int)round(z)] = avg + rand.RandRange(-roughness, roughness);
				}
			}
			//Create a smaller chunk to iterate through and use half to get the center again
			chunk /= 2; 
			half /= 2;
			//reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
			roughness /= change; 
		}

	}
};
	
