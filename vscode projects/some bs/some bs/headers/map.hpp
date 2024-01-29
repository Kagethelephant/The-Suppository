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


class Map
{


public:

	bool m_update;

	//----CONSTRUCTOR and DESTRUCTOR----
	Map(const int _size);
	~Map();


	//----ARRAYS----
	short int*** m_mapSort = new short int** [G_mapAlloc];
	short int** m_mapTile = new short int* [G_mapAlloc];
	float** m_moisture = new float* [G_mapAlloc];
	float** m_map = new float* [G_mapAlloc];


	//----CREATE THE MAP----
	void newMap( float _roughness = 30, float _change = 1);


	void mapErode(int _n);


	//----GET INDEX----
	int getIndex(int _x, int _y, int _quad);

	//---DRAW WITH VERTICES---
	bool drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _pos, const std::string& _tileset = "NULL");


	bool drawMiniMap(sf::RenderTarget& _target, int _gridSize, sf::Vector2i _pos);


	//----SORT MAP VALUES-----
	void sortMapValue();


private:
	

	int m_mapSize;

	int m_zLevels;
	int m_zHeight;

	sf::Vector2i m_tileRes;
	sf::Vector2i m_tileSize;
	sf::Vector2i m_textureRes;

	sf::VertexArray m_vertices[8];
	sf::VertexArray m_mapVertices;
	sf::Texture m_tileset;

	sf::Sprite m_mapSprite;
	sf::Texture m_mapTexture;
	sf::RenderTexture m_mapBuffer;
};