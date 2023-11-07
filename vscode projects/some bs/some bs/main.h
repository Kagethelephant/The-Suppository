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

	static const int arraySize = 100;
	float map[arraySize][arraySize] = {-1000.0f};

	void newMap(int s, int high = 20, float roughness = 20, float change = 1.6)
	{
		game rand;
		const int arraySize = 100;
		int size = pow(s,2) + 1;
		int chunk = (size-1); //size of the piece you are working with
		int half = chunk / 2; // this helps get the center value
		int low = 0; //bottom end of the random range to set the initial value
		float empty = -1000.0f;
		//DIAMOND SQUARE ALGORITH this is where the magic happens

		for (int z = 0; z < size; z += 1) { for (int i = 0; i < size; i += 1) { map[i][z] = empty; } }

		map[0][0] = 20;
		map[0][size-1] = 20;
		map[size-1][0] = 20;
		map[size-1][size-1] = 20;

		//SQUARE
		while (chunk > 1)
		{
			for (int z = 0; z < size; z += chunk)
			{
				for (int i = 0; i < size; i += chunk)
				{
					float x1y1 = map[i][z]; //grab the "X" values shown above
					float x2y1 = map[i + chunk][z];
					float x1y2 = map[i][z + chunk];
					float x2y2 = map[i + chunk][z + chunk];

					float avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

					//set the value "0" with the average + a random value and make sure we are not overwriting other values
					//
					if (map[i + half][z + half] == empty) map[i + half][z + half] = avg + rand.randRange(-roughness, roughness);
				}
			}

			//DIAMOND
			for (int z = 0; z < size+1; z += half)
			{
				for (int i = (z + half) % chunk; i < size+1; i += chunk)
				{

					float x1y = 0;
					float x2y = 0;
					float xy1 = 0;
					float xy2 = 0;

					int cnt = 4; 
					if (map[i - half][z]) { x1y = map[i - half][z]; } //grab the "X" values shown above
					else { cnt -= 1; }
					if (map[i + half][z]) { x2y = map[i + half][z]; }
					else { cnt -= 1; }
					if (map[i][z + half]) { xy1 = map[i][z + half]; }
					else { cnt -= 1; }
					if (map[i][z - half]) { xy2 = map[i][z - half]; }
					else { cnt -= 1; }

					float avg = (x1y + x2y + xy1 + xy2) / cnt;

					//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
					//
					if (map[i][z] == empty) map[i][z] = avg + rand.randRange(-roughness, roughness);
				}
			}
			chunk /= 2; //this is how we itterate through smaller and smaller chunks
			half /= 2;
			roughness /= change; //reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
		}

	}
};
	
