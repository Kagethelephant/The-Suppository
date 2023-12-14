#pragma once

#include "map.hpp"

Map::Map(const int _size)
{
	m_mapSize = _size;



	for (int i = 0; i <= m_mapSize; i++)
	{
		m_mapSort[i] = new int* [m_mapSize+1];

		for (int z = 0; z <= m_mapSize; z++)
		{
			m_mapSort[i][z] = new int[2];
		}
	}


	int c = 0;
	for (int i = 0; i <= m_mapSize; i++)
	{
		for (int j = 0; j <= m_mapSize; j++)
		{
			m_mapSort[i][j][0] = ++c;
			m_mapSort[i][j][1] = c;
		}
	}



	for (int i = 0; i <= m_mapSize; i++)
	{
		m_map[i] = new float [m_mapSize+1];
	}


	c = 0;
	for (int i = 0; i <= m_mapSize; i++)
	{
		for (int j = 0; j <= m_mapSize; j++)
		{
			m_map[i][j] = ++c;
		}
	}
}

Map::~Map()
{
	for (int i = 0; i <= m_mapSize; i++)
	{
		for (int j = 0; j <= m_mapSize-1; j++)
		{
			delete[] m_mapSort[i][j];
		}
		delete[] m_mapSort[i];
		delete[] m_map[i];
	}
	
	delete[] m_mapSort;
	delete[] m_map;

}






void Map::newMap(int _high, float _roughness, float _change)
{

	//---INITIALIZE VARIABLES----

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
	float chunk = m_mapSize;
	//to find the center of the chunk we are working with
	float half = m_mapSize / 2;
	//to initialize the grid so we know if we missed one
	float empty = -1000.0f;



	//---INITIALIZE MAP----

	//initialize the grid (1 cell grater then the size in the x and y ge
	for (int z = 0; z < m_mapSize; ++z)
	{
		for (int i = 0; i < m_mapSize; ++i)
		{
			m_map[i][z] = empty;
		}
	}

	//set initial corner values to extrapolate from at each corner
	m_map[0][0] = randRange(-_roughness, _roughness);
	m_map[0][m_mapSize] = randRange(-_roughness, _roughness);
	m_map[m_mapSize][0] = randRange(-_roughness, _roughness);
	m_map[m_mapSize][m_mapSize] = randRange(-_roughness, _roughness);


	//----SQUARE-----

	while (chunk > 1)
	{
		for (float z = 0; z < m_mapSize; z += chunk)
		{
			for (float i = 0; i < m_mapSize; i += chunk)
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
				x1y1 = m_map[(int)round(i)][(int)round(z)];
				x2y1 = m_map[(int)round(i + chunk)][(int)round(z)];
				x1y2 = m_map[(int)round(i)][(int)round(z + chunk)];
				x2y2 = m_map[(int)round(i + chunk)][(int)round(z + chunk)];

				avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

				//set the value "0" with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)round(i + half)][(int)round(z + half)] == empty) m_map[(int)round(i + half)][(int)round(z + half)] = avg + randRange(-_roughness, _roughness);
			}
		}



		//----DIAMOND----

		for (float z = 0; z < m_mapSize ; z += half)
		{
			for (float i = fmodf((z + half), chunk); i < m_mapSize ; i += chunk)
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
					x1y = m_map[(int)round(i - half)][(int)round(z)];
					div += 1;
				}

				if (i + half < m_mapSize+1)
				{
					x2y = m_map[(int)round(i + half)][(int)round(z)];
					div += 1;
				}

				if (z + half < m_mapSize+1)
				{
					xy1 = m_map[(int)round(i)][(int)round(z + half)];
					div += 1;
				}

				if (z - half >= 0)
				{
					xy2 = m_map[(int)round(i)][(int)round(z - half)];
					div += 1;
				}

				//calculate the average and use div to only average the amount of values grabbed
				avg = (x1y + x2y + xy1 + xy2) / 4;

				//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)round(i)][(int)round(z)] == empty) m_map[(int)round(i)][(int)round(z)] = avg + randRange(-_roughness, _roughness);
			}
		}

		//Create a smaller chunk to iterate through and use half to get the center again
		chunk /= 2;
		half /= 2;

		//reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
		_roughness /= _change;
	}

}




//---DRAW WITH VERTICES---

bool Map::drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _position, sf::Vector2i _gridSize, bool _solidColor, const std::string& _tileset)
{


	// load the tileset texture
	if (!_solidColor)
	{
		if (!m_tileset.loadFromFile(_tileset))
			return false;
	}

	//temporary
	if (!m_tileset.loadFromFile(_tileset))
		return false;


	//gap in between the cells
	int gap = 1;

	// resize the vertex array to fit the level size
	m_vertices.setPrimitiveType(sf::Triangles);
	m_vertices.resize(_gridSize.x * _gridSize.y * 6);

	// populate the vertex array, with two triangles per tile
	for (int i = 0; i < _gridSize.x; ++i)
	{
		for (int z = 0; z < _gridSize.y; ++z)
		{


			// get the current tile number
			int iX = _position.x - z + floor((i + 1) / 2);
			int iY = _position.y + z + floor(i / 2);



			int tileNumber;
			if (iX > 0 && iX < m_mapSize && iY > 0 && iY < m_mapSize)
			{
				tileNumber = m_map[m_mapSort[iX][iY][0]][m_mapSort[iX][iY][1]];
				//tileNumber = m_map[iX][iY];
			}
			else tileNumber = -10000;



			int posX = (i * _tileSize.x / 2);
			int posY = z * _tileSize.y - ((i % 2) * 8);



			// get a pointer to the triangles' vertices of the current tile
			sf::Vertex* triangles = &m_vertices[(i + z * _gridSize.x) * 6];

			//tempprary
			int gap = 1;

			triangles[0].position = sf::Vector2f(posX + gap, posY);
			triangles[1].position = sf::Vector2f((posX + gap + (_tileSize.x / 2)), (posY - (_tileSize.y / 2)));
			triangles[2].position = sf::Vector2f((posX + gap + (_tileSize.x / 2)), (posY + (_tileSize.y / 2)));
			triangles[3].position = sf::Vector2f((posX + (_tileSize.x / 2)), (posY - (_tileSize.y / 2)));
			triangles[4].position = sf::Vector2f((posX + (_tileSize.x / 2)), (posY + (_tileSize.y / 2)));
			triangles[5].position = sf::Vector2f(posX + _tileSize.x, posY);


			if (_solidColor)
			{
				sf::Color color;

				if (tileNumber != -10000)
				{
					int index;
					// get the current tile number

					if (tileNumber < 15 && tileNumber >10)
					{
						// find its position in the tileset texture
						//int tu = index % (m_tileset.getSize().x / tileSize);
						//int tv = index / (m_tileset.getSize().x / tileSize);
						//int tu = tileNumber % (m_tileset.getSize().x / tileSize);
						//int tv = tileNumber / (m_tileset.getSize().x / tileSize);

						// define the 6 matching texture coordinates
						triangles[0].texCoords = sf::Vector2f(0, 0);
						triangles[1].texCoords = sf::Vector2f(32, 0);
						triangles[2].texCoords = sf::Vector2f(0, 32);
						triangles[3].texCoords = sf::Vector2f(0, 32);
						triangles[4].texCoords = sf::Vector2f(32, 0);
						triangles[5].texCoords = sf::Vector2f(32, 32);

					}

					else
					{
						if (tileNumber < -10) { color = (sf::Color(G_dkblue_x, G_dkblue_y, G_dkblue_z)); }
						else if (tileNumber < -5) { color = (sf::Color(G_blue_x, G_blue_y, G_blue_z)); }
						else if (tileNumber < 0) { color = (sf::Color(G_ltblue_x, G_ltblue_y, G_ltblue_z)); }
						else if (tileNumber < 5) { color = (sf::Color(G_tan_x, G_tan_y, G_tan_z)); }
						else if (tileNumber < 10) { color = (sf::Color(G_green_x, G_green_y, G_green_z)); }
						else if (tileNumber < 15) { color = (sf::Color(G_dkgreen_x, G_dkgreen_y, G_dkgreen_z)); }
						else if (tileNumber < 20) { color = (sf::Color(G_purple_x, G_purple_y, G_purple_z)); }
						else { color = (sf::Color(G_dkpurple_x, G_dkpurple_y, G_dkpurple_z)); }
					}

				}
				else color = sf::Color(G_black_x, G_black_y, G_black_z);

				for (int k = 0; k < 6; k += 1)
				{
					triangles[k].color = sf::Color(color);
				}

			}


			else
			{
				int index;
				// get the current tile number

				if (tileNumber < 15 && tileNumber >10) { index = 1; }
				else { index = 0; }

				// find its position in the tileset texture
				//int tu = index % (m_tileset.getSize().x / tileSize);
				//int tv = index / (m_tileset.getSize().x / tileSize);
				//int tu = tileNumber % (m_tileset.getSize().x / tileSize);
				//int tv = tileNumber / (m_tileset.getSize().x / tileSize);

				// define the 6 matching texture coordinates
				triangles[0].position = sf::Vector2f(0, 0);
				triangles[1].position = sf::Vector2f(32, 0);
				triangles[2].position = sf::Vector2f(0, 32);
				triangles[3].position = sf::Vector2f(0, 32);
				triangles[4].position = sf::Vector2f(32, 0);
				triangles[5].position = sf::Vector2f(32, 32);

			}

		}
	}

	if (_solidColor)_target.draw(m_vertices);
	else _target.draw(m_vertices, &m_tileset); //Need to include the texture???
	return true;
}

void Map::sortMapValue()
{

	int next;
	int iIndex;
	int zIndex;
	int qIndex;
	int mIndex;

	for (int i = 0; i < m_mapSize; ++i)
	{
		for (int z = 0; z < m_mapSize; ++z)
		{
			m_mapSort[i][z][0] = i;
			m_mapSort[i][z][1] = z;
		}
	}


	int test;

	for (int i = 0; i < m_mapSize; ++i)
	{	
		for (int z = 0; z < m_mapSize; ++z)
		{
			next = z + 1;

			iIndex = m_mapSort[i][z][0];
			zIndex = m_mapSort[i][z][1];

			for (int q = i; q < m_mapSize; ++q)
			{
				for (int m = next; m < m_mapSize; ++m)
				{
					qIndex = m_mapSort[q][m][0];
					mIndex = m_mapSort[q][m][1];

					if (m_map[iIndex][zIndex] < m_map[qIndex][mIndex])
					{
						m_mapSort[q][m][0] = iIndex;
						m_mapSort[q][m][1] = zIndex;

						m_mapSort[i][z][0] = qIndex;
						m_mapSort[i][z][1] = mIndex;

						iIndex = qIndex;
						zIndex = mIndex;
					}
				}
				next = 0;
			}
		}
	}
}




void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}


