#pragma once

#include "headers/display.hpp"


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

sf::Vector2i windowSetup(sf::RenderWindow& win, sf::View& vw, int h, bool fullscreen = true, int frame = 60)
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
	if (fullscreen) win.create(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Fullscreen);
	else win.create(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Default);
	//set the frame rate and hide the cursor so we can draw our own
	win.setFramerateLimit(60);
	win.setMouseCursorVisible(false);

	//set the size and position of the view
	vw.setCenter(sf::Vector2f(bufferX, bufferY));
	vw.setSize(sf::Vector2f(bufferW, bufferH));

	//asign the view to the window
	win.setView(vw);

	//return the calculated width and height of the window in pixels
	return sf::Vector2i(bufferW, bufferH);
}

