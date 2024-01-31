#pragma once

//SFML
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"

//General
#include "wtypes.h"
#include "winuser.h"


//game header files
#include "constants.hpp"


class Canvas
{

public:

	Canvas(int _w,int _h);

	~Canvas();


	sf::Vector2i m_canvasSize;
	//main render texture
	sf::RenderTexture m_renderer;
	//layers or textures to be used to draw (10 max)
	int const m_n = 10;
	sf::Texture* m_layers = new sf::Texture [m_n];
	bool* m_reset = new bool[m_n];

	sf::Texture layer;


	void draw(sf::Drawable& _sprite, int _layer, bool _reset = true);


	void clearLayer(int _layer, sf::Color _clear = sf::Color::Transparent);


	void display(sf::RenderWindow& _window);


	void dig(sf::RenderWindow& _window, sf::RenderTexture& _ren, sf::Texture& _text);
};

//sets up window with a pixel scale and returns the dimmensions of the window
//in a 2 dimmensional vector
sf::Vector2i windowSetup(sf::RenderWindow&, int, bool, int);

