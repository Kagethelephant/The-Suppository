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

	int GetResW()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}
	int GetResH()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}
	void colorInit()
	{
		sf::Vector3i c_black(13, 14, 26);
		sf::Vector3i c_dkblue(33, 47, 106);
		sf::Vector3i c_blue(47, 80, 118);
		sf::Vector3i c_ltblue(70, 113, 128);
		sf::Vector3i c_tan(116, 113, 89);
		sf::Vector3i c_green(49, 83, 76);
		sf::Vector3i c_dkgreen(34, 53, 59);
		sf::Vector3i c_dkpurple(43, 37, 49);
		sf::Vector3i c_purple(77, 61, 85);
		sf::Vector3i c_snow(182, 182, 182);
	}
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
	void newMap(int s, int high = 20, float roughness = 20, float change = 1.6)
	{
		game rand;
		const int arraySize = 100;
		int size = s ^ 2 + 1;
		int chunk = (size); //size of the piece you are working with
		int half = chunk / 2; // this helps get the center value
		int low = 0; //bottom end of the random range to set the initial value
		float empty = -1000;
		//DIAMOND SQUARE ALGORITH this is where the magic happens
		float map[arraySize][arraySize];
		for (int z = 0; z < size; z += chunk) { for (int i = 0; i < size; i += chunk) { map[i][z] = empty; } }

		//SQUARE
		while (chunk > 1) //this is toggled so we can see each iteration
		{
			for (int z = 0; z < size; z += chunk)
			{
				for (int i = 0; i < size; i += chunk)
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

					float x1y1 = *map[i, z]; //grab the "X" values shown above
					float x2y1 = *map[i + chunk, z];
					float x1y2 = *map[i, z + chunk];
					float x2y2 = *map[i + chunk, z + chunk];

					float avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

					//set the value "0" with the average + a random value and make sure we are not overwriting other values
					if (*map[i + half, z + half] == empty) *map[i + half, z + half] = avg + rand.randRange(-roughness, roughness);
				}
			}

			//DIAMOND
			for (int z = 0; z < size + 1; z += half)
			{
				for (int i = (z + half) % chunk; i < size + 1; i += chunk)
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


					float x1y = *map[i - half, z]; //grab the "X" values shown above
					float x2y = *map[i + half, z];
					float xy1 = *map[i, z + half];
					float xy2 = *map[i, z - half];

					float avg = (x1y + x2y + xy1 + xy2) / 4;

					//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
					if (*map[i, z] == empty) *map[i, z] = avg + rand.randRange(-roughness, roughness);
				}
			}
			chunk /= 2; //this is how we itterate through smaller and smaller chunks
			half /= 2;
			roughness /= change; //reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
		}

	}
};
	
