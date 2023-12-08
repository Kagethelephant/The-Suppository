#pragma once

#include "display.hpp"


//----INITIALIZE VIEW----

sf::Vector2i windowSetup(sf::RenderWindow& m_window, sf::View& m_view, int m_height, bool m_fullscreen = true, int m_fps = 60)
{
	//get the display dimmensions and calculate the aspect ratio
	float displayWidth = GetSystemMetrics(SM_CXSCREEN);
	float DisplayHeight = GetSystemMetrics(SM_CYSCREEN);
	float aspectRatio = displayWidth / DisplayHeight;

	//create a static window width in pixels and calculate the width based on the aspect ratio
	int bufferH = m_height;
	int bufferW = abs(bufferH * aspectRatio);

	//center position of the window (needed for view setup)
	int bufferX = bufferW / 2;
	int bufferY = bufferH / 2;

	//initialize the view with the calculated resolution 
	if (m_fullscreen) m_window.create(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Fullscreen);
	else m_window.create(sf::VideoMode(bufferW, bufferH), "Some BS", sf::Style::Default);
	//set the frame rate and hide the cursor so we can draw our own
	m_window.setFramerateLimit(60);
	m_window.setMouseCursorVisible(false);

	//set the size and position of the view
	m_view.setCenter(sf::Vector2f(bufferX, bufferY));
	m_view.setSize(sf::Vector2f(bufferW, bufferH));

	//asign the view to the window
	m_window.setView(m_view);

	//return the calculated width and height of the window in pixels
	return sf::Vector2i(bufferW, bufferH);
}

