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

//define some colors cause the default ones are ugly
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

class Game  
{

public:

	//this function generates a random integer between the 2 provided integer's
	int randRange(int min, int max)
	{
		// obtain a random number from hardware
		std::random_device rd; 
		// seed the generator
		std::mt19937 gen(rd()); 
		// define the range
		std::uniform_int_distribution<> distr(min, max); 
		
		return distr(gen);
	}

	//----INITIALIZE VIEW----

	sf::Vector2i windowSetup(sf::RenderWindow &win, sf::View &vw, sf::RenderTexture &buf, sf::Sprite &spr, int h)
	{
		//get the display dimmensions and calculate the aspect ratio
		float displayWidth = GetSystemMetrics(SM_CXSCREEN);
		float DisplayHeight = GetSystemMetrics(SM_CYSCREEN);
		float aspectRatio = displayWidth / DisplayHeight;

		//create a static window width in pixels and calculate the width based on the aspect ratio
		int bufferH = h;
		int bufferW = abs(bufferH * aspectRatio);

		//center position of the window (needed for view setup)
		int bufferX = bufferW / 2;
		int bufferY = bufferH / 2;

		//initialize the view with the calculated resolution 
		win.create(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Fullscreen);
		//set the frame rate and hide the cursor so we can draw our own
		win.setFramerateLimit(60);
		win.setMouseCursorVisible(false);

		//set the size and position of the view
		vw.setCenter(sf::Vector2f(bufferX, bufferY));
		vw.setSize(sf::Vector2f(bufferW, bufferH));

		//asign the view to the window
		win.setView(vw);

		//create a buffer and sprite to draw to
		buf.create(bufferW, bufferH);
		spr.setTexture(buf.getTexture());

		//return the calculated width and height of the window in pixels
		return sf::Vector2i(bufferW, bufferH);
	}

};

class DiamSquare
{

public:

	//----CREATE THE MAP----

	void newMap(float map[200][200], int size, int high = 20, float roughness = 20, float change = 1.6)
	{

		//---INITIALIZE VARIABLES----

		Game rand;
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
		map[0][0] = rand.randRange(-roughness, roughness);
		map[0][size] = rand.randRange(-roughness, roughness);
		map[size][0] = rand.randRange(-roughness, roughness);
		map[size][size] = rand.randRange(-roughness, roughness);



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
					if (map[(int)round(i + half)][(int)round(z + half)] == empty) map[(int)round(i + half)][(int)round(z + half)] = avg + rand.randRange(-roughness, roughness);
				}
			}
			


			//----DIAMOND----

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
					avg = (x1y + x2y + xy1 + xy2) / 4; 

					//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
					if (map[(int)round(i)][(int)round(z)] == empty) map[(int)round(i)][(int)round(z)] = avg + rand.randRange(-roughness, roughness);
				}
			}
			//Create a smaller chunk to iterate through and use half to get the center again
			chunk /= 2; 
			half /= 2;
			//reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
			roughness /= change; 
		}

	}


	//----GENERATE NEW RANDOM MAP----

	void reGen(float map[200][200],int dsSize, sf::RenderTexture& buffer, sf::RectangleShape& rect, int gridSize, int viewGridX, int viewGridY)
	{
		//clear viewwith a background color
		buffer.clear(sf::Color(c_black.x, c_black.y, c_black.z));
		rect.setOutlineColor(sf::Color::Transparent);
		//iterate throught the diamond square grid and draw squares at different colors
		//for different heights
		for (int i = 0; i < dsSize + 1; i += 1)
		{
			for (int z = 0; z < dsSize + 1; z += 1)
			{
				int tx = floor(viewGridX + i);
				int ty = floor(viewGridY + z);

				//Check the number from the grid and set a corisponding color
				if (map[tx][ty] < -10) { rect.setFillColor(sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
				else if (map[tx][ty] < -5) { rect.setFillColor(sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
				else if (map[tx][ty] < 0) { rect.setFillColor(sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
				else if (map[tx][ty] < 5) { rect.setFillColor(sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
				else if (map[tx][ty] < 10) { rect.setFillColor(sf::Color(c_green.x, c_green.y, c_green.z)); }
				else if (map[tx][ty] < 15) { rect.setFillColor(sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
				else if (map[tx][ty] < 20) { rect.setFillColor(sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
				else { rect.setFillColor(sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }

				//Move the rectangle to the correct position before drawing
				rect.setPosition((1) + (i * gridSize), (1) + (z * gridSize));
				buffer.draw(rect);
			}
		}

		//change the rectangle so it can be used as the mouse position
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineColor(sf::Color::Black);
	}

};
	
