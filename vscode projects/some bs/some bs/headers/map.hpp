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
#include <cmath>


//game libraries
#include "headers/display.hpp"

//alocate space for the grid
static const int mapSize = 1000;


class Map : public sf::Drawable, public sf::Transformable
{

public:

	//----CREATE THE MAP----
	void newMap(float map[mapSize][mapSize], int size, int high = 20, float roughness = 20, float change = 1.4);


	//---DRAW WITH VERTICES---
	bool drawMap(sf::RenderTarget& target, float map[mapSize][mapSize], sf::Vector2i tileSize, sf::Vector2i pos, sf::Vector2i size, bool solidColor = true, const std::string& tileset = "NULL");



private:

	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;


	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};