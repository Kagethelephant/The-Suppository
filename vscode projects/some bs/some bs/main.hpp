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

	int randRange(int min, int max)
	{
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 gen(rd()); // seed the generator
		std::uniform_int_distribution<> distr(min, max); // define the range
		
		return distr(gen);
	}
};

class DiamSquare
{

public:

	static const int arraySize = 101;
	float map[arraySize][arraySize] = {-1000.0f};
	int cntDiam = 0;
	int cntSqr = 0;
	int itr[6][4];
	int run = 0;

	void newMap(int s, int high = 20, float roughness = 20, float change = 1.6)
	{
		game rand;
		int size = s;
		float chunk = size; //size of the piece you are working with
		float half = size/2; // this helps get the center value
		float empty = -1000.0f;

		for (int z = 0; z < size+1; z += 1) { for (int i = 0; i < size+1; i += 1) { map[i][z] = empty; } }

		map[0][0] = rand.randRange(-roughness, roughness);
		map[0][size] = rand.randRange(-roughness, roughness);
		map[size][0] = rand.randRange(-roughness, roughness);
		map[size][size] = rand.randRange(-roughness, roughness);

		float x1y1; //grab the "X" values shown above
		float x2y1;
		float x1y2;
		float x2y2;

		float x1y;
		float x2y;
		float xy1;
		float xy2;

		float avg;

		//SQUARE
		while (chunk > 1)
		{		
			//cntSqr = 0;
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

					x1y1 = map[(int)round(i)][(int)round(z)]; //grab the "X" values shown above
					x2y1 = map[(int)round(i + chunk)][(int)round(z)];
					x1y2 = map[(int)round(i)][(int)round(z + chunk)];
					x2y2 = map[(int)round(i + chunk)][(int)round(z + chunk)];

					avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

					//set the value "0" with the average + a random value and make sure we are not overwriting other values
					if (map[(int)round(i + half)][(int)round(z + half)] == empty) map[(int)round(i + half)][(int)round(z + half)] = avg + rand.randRange(-roughness, roughness);
				}
			}
			
			//cntDiam = 0;
			//DIAMOND
			for (float z = 0; z < size+1; z += half)
			{
				for (float i = fmodf((z+ half),chunk); i < size+1; i += chunk)
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
					
					if(i-half >=0) x1y = map[(int)round(i - half)][(int)round(z)];  //grab the "X" values shown above
					if (i + half < size+1) x2y = map[(int)round(i + half)][(int)round(z)];
					if (z + half < size+1) xy1 = map[(int)round(i)][(int)round(z + half)];
					if (z - half >= 0) xy2 = map[(int)round(i)][(int)round(z - half)];

					avg = (x1y + x2y + xy1 + xy2) / 4;

					//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
					if (map[(int)round(i)][(int)round(z)] == empty) map[(int)round(i)][(int)round(z)] = avg + rand.randRange(-roughness, roughness);
				}
			}
			chunk /= 2; //this is how we itterate through smaller and smaller chunks
			half /= 2;
			roughness /= change; //reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
		}

	}
};
	
