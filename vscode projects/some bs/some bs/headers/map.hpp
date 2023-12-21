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
	short int*** m_mapSort = new short int** [G_mapAlloc];
	short int** m_mapTile = new short int* [G_mapAlloc];
	float** m_map = new float* [G_mapAlloc];
	float** m_moisture = new float* [G_mapAlloc];


	//----CREATE THE MAP----
	void newMap(float _roughness = 40, float _change = 1.6);


	void erode(int _iter);


	void miniMap(sf::RenderTarget& _target);


	int findTile(int _i, int _z);


	//---DRAW WITH VERTICES---
	bool drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _pos, sf::Vector2i _gridSize, const std::string& _tileset = "NULL");




	//----SORT MAP VALUES-----
	void sortMapValue();

private:
	
	virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

	int m_mapSize;
	sf::VertexArray m_vertices[8];
	sf::VertexArray m_vertices2;
	sf::Texture m_tileset;
};