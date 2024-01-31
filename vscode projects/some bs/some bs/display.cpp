#pragma once

#include "display.hpp"


Canvas::Canvas(int _w, int _h)
{
	m_canvasSize.x = _w;
	m_canvasSize.y = _h;

	m_renderer.create(m_canvasSize.x, m_canvasSize.y);

	for (int i = 0; i < m_n; i++)
	{
		m_renderer.clear(sf::Color::Transparent);
		m_renderer.display();
		m_layers[i] = m_renderer.getTexture();
		m_reset[i] = true;
	}

}


Canvas::~Canvas()
{
	delete[] m_layers;
}



void Canvas::draw(sf::Drawable& _sprite, int _layer, bool _reset)
{
	m_renderer.clear(sf::Color::Transparent);
	m_renderer.draw(sf::Sprite(m_layers[_layer]));
	m_renderer.draw(_sprite);
	m_renderer.display();
	m_layers[_layer] = m_renderer.getTexture();
}


                                                                       
void Canvas::clearLayer(int _layer, sf::Color _clear)
{
	m_renderer.clear(_clear);
	m_renderer.display();
	m_layers[_layer] = m_renderer.getTexture();
}



void Canvas::display(sf::RenderWindow& _window)
{
	m_renderer.clear(sf::Color::Transparent);
	for(int i = 0; i < m_n; i++)
	{
		m_renderer.draw(sf::Sprite(m_layers[i]));
	}
	m_renderer.display();
	_window.draw(sf::Sprite(m_renderer.getTexture()));

	// Display the window to the screen
	_window.display();

	m_renderer.clear(sf::Color::Transparent);
	for (int i = 0; i < m_n; i++)
	{
		if(m_reset[i] = true) m_layers[i] = m_renderer.getTexture();
	}
}







//----INITIALIZE VIEW----

sf::Vector2i windowSetup(sf::RenderWindow& m_window, int m_height, bool m_fullscreen = true, int m_fps = 60)
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
	sf::View view;
	view.setCenter(sf::Vector2f(bufferX, bufferY));
	view.setSize(sf::Vector2f(bufferW, bufferH));

	//asign the view to the window
	m_window.setView(view);

	//return the calculated width and height of the window in pixels
	return sf::Vector2i(bufferW, bufferH);
}


