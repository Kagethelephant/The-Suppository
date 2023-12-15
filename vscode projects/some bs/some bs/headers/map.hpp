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
#include "math.hpp"
#include "constants.hpp"


class Map : public sf::Drawable, public sf::Transformable
{


public:

	//----CONSTRUCTOR and DESTRUCTOR----
	Map(const int _size);
	~Map();


	//----ARRAYS----
	int*** m_mapSort = new int** [G_mapAlloc];
	float** m_map = new float* [G_mapAlloc];


	//----CREATE THE MAP----
	void newMap( int _high = 20, float _roughness = 20, float _change = 1.4);



	//---DRAW WITH VERTICES---
	bool drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _pos, sf::Vector2i _gridSize, const std::string& _tileset = "NULL");




	//----SORT MAP VALUES-----
	void sortMapValue();

private:
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	int m_mapSize;
	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};