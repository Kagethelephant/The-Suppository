#pragma once

#include "map.hpp"



//-----CONSTRUCTOR------

Map::Map(const int _size)
{
	m_mapSize = _size;

	m_layers = 8;
	m_tileRes.x = 32;
	m_tileRes.y = 32;
	m_tileBaseRes.x = 32;
	m_tileBaseRes.y = 16;
	m_layerJump = 16;


	for (int i = 0; i <= m_mapSize; i++)
	{
		m_mapSort[i] = new short int* [m_mapSize+1];

		for (int z = 0; z <= m_mapSize; z++)
		{
			m_mapSort[i][z] = new short int[2];
			m_mapSort[i][z][0] = -1000;
			m_mapSort[i][z][1] = -1000;
		}
	}

	for (int i = 0; i <= m_mapSize; i++)
	{
		m_map[i] = new float [m_mapSize+1];
		for (int j = 0; j < m_mapSize + 1; j++)
		{
			m_map[i][j] = -1000;
		}
	}


	for (int i = 0; i <= m_mapSize; i++)
	{
		m_mapTile[i] = new short int[m_mapSize + 1];
		for (int j = 0; j < m_mapSize + 1; j++)
		{
			m_mapTile[i][j] = -1000;
		}
	}

	for (int i = 0; i <= m_mapSize; i++)
	{
		m_moisture[i] = new float[m_mapSize + 1];
		for (int j = 0; j < m_mapSize + 1; j++)
		{
			m_moisture[i][j] = -1000;
		}
	}

}


//------DESTRUCTOR------

Map::~Map()
{
	for (int i = 0; i <= m_mapSize; i++)
	{
		for (int j = 0; j <= m_mapSize; j++)
		{
			delete[] m_mapSort[i][j];
		}
		delete[] m_mapSort[i];
		delete[] m_mapTile[i];
		delete[] m_map[i];
		delete[] m_moisture[i];
	}
	

	delete[] m_mapSort;
	delete[] m_mapTile;
	delete[] m_map;
	delete[] m_moisture;

	std::cout << "***** MAP DESTROYED *****" << std::endl;

}








//------DIAMOND SQUARE------

void Map::newMap( float _roughness, float _change)
{

	std::cout << "MAP GENERATING ";

	float change2 = 1.2;
	int div;

	//points to extrapolate from for square step
	float avg, x1y1, x2y1, x1y2, x2y2, x1y, x2y, xy1, xy2;

	int step1 = -25;
	int step2 = -15;
	int step3 = -5;
	int step4 = 5;
	int step5 = 15;
	int step6 = 25;
	int step7 = 35;


	//create a chunk the size of the grid that will be halfed every itteration
	float chunk = m_mapSize;
	//to find the center of the chunk we are working with
	float half = m_mapSize / 2;
	//to initialize the grid so we know if we missed one
	float empty = -1000.0;



	//---INITIALIZE MAP----

	//initialize the grid (1 cell grater then the size in the x and y ge
	for (int z = 0; z <= m_mapSize; ++z)
	{
		for (int i = 0; i <= m_mapSize; ++i)
		{
			m_map[i][z] = empty;
			m_mapTile[i][z] = empty;
			m_mapSort[i][z][0] = empty;
			m_mapSort[i][z][1] = empty;
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
		std::cout << ". ";
	
		for (float z = 0; z < m_mapSize; z += chunk)
		{
			for (float i = 0; i < m_mapSize; i += chunk)
			{

				// 0 - - - 0	This is the square portion of the code the "X" represents
				// - - - - -	points that have already been populated. these points are 
				// - - - - -	averaged and a random value is added "roughness" to generate
				// - - - - -	the "0" values. This is the initialization done above
				// 0 - - - 0
				//
				// X - 0 - X	This is what the first iteration in this loop will look like
				// - - - - -	 
				// 0 - X - 0	
				// - - - - -	
				// X - 0 - X


				//grab the "X" values shown above
				x1y1 = m_map[(int)floor(i)][(int)floor(z)];
				x2y1 = m_map[(int)floor(i + chunk)][(int)floor(z)];
				x1y2 = m_map[(int)floor(i)][(int)floor(z + chunk)];
				x2y2 = m_map[(int)floor(i + chunk)][(int)floor(z + chunk)];


				avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;
				avg += randRange(-_roughness, _roughness);

				//set the value "0" with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)floor(i + half)][(int)floor(z + half)] == empty)
				{
					m_map[(int)floor(i + half)][(int)floor(z + half)] = avg;

					if (avg < step1)     { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 0; }
					else if (avg < step2) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 1; }
					else if (avg < step3)  { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 2; }
					else if (avg < step4)  { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 3; }
					else if (avg < step5) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 4; }
					else if (avg < step6) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 5; }
					else if (avg < step7) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 6; }
					else               { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 7; }
				}
			}
		}


		std::cout << ". ";

		//----DIAMOND----

		for (float z = 0; z < m_mapSize +1; z += half)
		{

			for (float i = fmodf((z + half), chunk); i < m_mapSize +1; i += chunk)
			{

				// X - - - X	This is the diamond portion of the code the "X" represents
				// - - - - -	points that have already been populated. these points are 
				// - - 0 - -	averaged and a random value is added "roughness" to generate
				// - - - - -	the "0" values.
				// X - - - X
				//
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
				if (floor(i - half) >= 0)
				{
					x1y = m_map[(int)floor(i - half)][(int)floor(z)];
					div += 1;
				}

				if (floor(i + half) < m_mapSize + 1)
				{
					x2y = m_map[(int)floor(i + half)][(int)floor(z)];
					div += 1;
				}

				if ((int)floor(z + half) < m_mapSize + 1)
				{
					xy1 = m_map[(int)floor(i)][(int)floor(z + half)];
					div += 1;
				}

				if ((int)floor(z - half) >= 0)
				{
					xy2 = m_map[(int)floor(i)][(int)floor(z - half)];
					div += 1;
				}


				//calculate the average and use div to only average the amount of values grabbed
				avg = (x1y + x2y + xy1 + xy2) / 4;
				avg += randRange(-_roughness, _roughness);



				//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)floor(i)][(int)floor(z)] == empty)
				{
					m_map[(int)floor(i)][(int)floor(z)] = avg;

					if (avg < step1)     { m_mapTile[(int)floor(i)][(int)floor(z)] = 0; }
					else if (avg < step2) { m_mapTile[(int)floor(i)][(int)floor(z)] = 1; }
					else if (avg < step3)  { m_mapTile[(int)floor(i)][(int)floor(z)] = 2; }
					else if (avg < step4)  { m_mapTile[(int)floor(i)][(int)floor(z)] = 3; }
					else if (avg < step5) { m_mapTile[(int)floor(i)][(int)floor(z)] = 4; }
					else if (avg < step6) { m_mapTile[(int)floor(i)][(int)floor(z)] = 5; }
					else if (avg < step7) { m_mapTile[(int)floor(i)][(int)floor(z)] = 6; }
					else               { m_mapTile[(int)floor(i)][(int)floor(z)] = 7; }
				}
			}
		}

		//Create a smaller chunk to iterate through and use half to get the center again
		chunk /= 2;
		half /= 2;

		//reduce the amount of change each iteration (higher value is smoother because its reducing the change faster)
		_roughness /= _change;
		_change *= change2;

	}
	std::cout << std::endl << std::endl;
}









void Map::mapErode(int _n)
{
	int iX, iY;

	int offsetX = 0;
	int offsetY = 0;

	sf::Vector2i smallIndex(0,0);
	float smallValue = 0;

	int temp[3][3][3];

	while (_n > 0)
	{
		_n--;

		for (int i = 0; i < m_mapSize; i++)
		{
			for (int j = 0; j < m_mapSize; j++)
			{

				
				iX = m_mapSort[i][j][0];
				iY = m_mapSort[i][j][1];

				temp[1][1][0] = m_map[iX][iY];
				temp[1][1][1] = m_moisture[iX][iY]+randRange(0,2);

				smallValue = -1000;
				smallIndex.x = 0;
				smallIndex.y = 0;

				for (int k = 0; k < 3; k++)
				{
					for (int z = 0; z < 3; z++)
					{

						// [0][0]  [0][1]  [0][2] 
						//
						// [1][0]  [1][1]  [1][2] 
						// 
						// [2][0]  [2][1]  [2][2] 
						// 
						if (k != 1 && z != 1)
						{
							temp[k][z][0] = 1000;
							temp[k][z][1] = 0;
							temp[k][z][2] = 1000;

							offsetX = iX + k - 1;
							offsetY = iY + z - 1;

							if (offsetX > 0 && offsetX < m_mapSize && offsetY > 0 && offsetY < m_mapSize)
							{
								temp[k][z][0] = m_map[offsetX][offsetY];
								temp[k][z][1] = m_moisture[offsetX][offsetY];
								temp[k][z][2] = m_map[offsetX][offsetY]+m_moisture[offsetX][offsetY];
								//if()
							}


						}
					}
				}

				
			}
		}
	}	
}










//---DRAW WITH VERTICES---

int Map::getIndex(int _x, int _y, int _quad)
{
	int index = m_mapTile[_x][_y];
	int newIndex = index;

	bool c1 = 1, c2 = 1, c3 = 1, c4 = 1, c5 = 1, c6 = 1, c7 = 1, c8 = 1;

	if (_x > 1)
	{
		if (_y > 1) c1 = (index <= m_mapTile[_x - 1][_y - 1]);
		c8 = (index <= m_mapTile[_x - 1][_y]);
		if (_y < m_mapSize-1) c7 = (index <= m_mapTile[_x - 1][_y + 1]);
	}

	if (_x < m_mapSize-1)
	{
		if (_y > 1) c3 = (index <= m_mapTile[_x + 1][_y - 1]);
		c4 = (index <= m_mapTile[_x + 1][_y]);
		if (_y < m_mapSize-1) c5 = (index <= m_mapTile[_x + 1][_y + 1]);
	}

	if (_y > 1) c2 = (index <= m_mapTile[_x][_y - 1]);
	if (_y < m_mapSize-1) c6 = (index <= m_mapTile[_x][_y + 1]);


	// 1 | 3
	// -----
	// 2 | 4


	if (_quad == 1)
	{
		if (c8 && c1 && c2) newIndex = 0;
		else if (!c8 && c2) newIndex = 4;
		else if (c8 && !c2) newIndex = 8;
		else if (!c8 && !c2) newIndex = 12;
		else if (c8 && !c1 && c2) newIndex = 16;
	}

	else if (_quad == 3)
	{
		if (c8 && c7 && c6) newIndex = 1;
		else if (!c8 && c6) newIndex = 5;
		else if (c8 && !c6) newIndex = 9;
		else if (!c8 && !c6) newIndex = 13;
		else if (c8 && !c7 && c6) newIndex = 17;
	}

	else if (_quad == 0)
	{
		if (c4 && c3 && c2) newIndex = 2;
		else if (!c4 && c2) newIndex = 6;
		else  if (c4 && !c2) newIndex = 10;
		else if (!c4 && !c2) newIndex = 14;
		else if (c4 && !c3 && c2) newIndex = 18;
	}

	else if (_quad == 2)
	{
		if (c4 && c5 && c6) newIndex = 3;
		else if (!c4 && c6) newIndex = 7;
		else if (c4 && !c6) newIndex = 11;
		else if (!c4 && !c6) newIndex = 15;
		else if (c4 && !c5 && c6) newIndex = 19;
	}

	return newIndex;
}








bool Map::drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _position, sf::Vector2i _gridSize, const std::string& _tileset)
{
	if (!m_tileset.loadFromFile(_tileset))
		return -1;

	_gridSize.x += 2;
	_gridSize.y += 18;
	_position.y -= 2;

	int tileNumber;
	int index;

	int quad[4];

	int iX, iY;
	int posX, posY;
	int tu, tv;


	sf::Color color;


	// populate the vertex array, with two triangles per tile
	for (int k = 0; k < 8; k++)
	{
		// resize the vertex array to fit the level size
		m_vertices[k].setPrimitiveType(sf::Triangles);
		m_vertices[k].resize((_gridSize.x) * (_gridSize.y) * 24);

		for (int z = 0; z < _gridSize.y; ++z)
		{
			for (int i = 0; i < _gridSize.x; ++i)
			{


				iX = _position.x - floor(z / 2) + i;
				iY = _position.y + floor((z + 1) / 2) + i;


				if (iX > 0 && iX < m_mapSize && iY > 0 && iY < m_mapSize)
					tileNumber = m_mapTile[iX][iY];
				else tileNumber = -1000;


				// get a pointer to the triangles' vertices of the current tile
				sf::Vertex* triangles = &m_vertices[k][(i + z * _gridSize.x) * 24];

				posX = (((i)*_tileSize.x) + ((z % 2) * _tileSize.x) / 2) - _tileSize.x;
				posY = (((z)*_tileSize.y) / 2) - _tileSize.y;


				for (int j = 0; j < 4; j++)
				{
					triangles[(j * 6)].position = sf::Vector2f(posX, posY - (k * 16));
					triangles[(j * 6) + 1].position = sf::Vector2f(posX + 32, posY - (k * 16));
					triangles[(j * 6) + 2].position = sf::Vector2f(posX, posY + 32 - (k * 16));
					triangles[(j * 6) + 3].position = sf::Vector2f(posX, posY + 32 - (k * 16));
					triangles[(j * 6) + 4].position = sf::Vector2f(posX + 32, posY - (k * 16));
					triangles[(j * 6) + 5].position = sf::Vector2f(posX + 32, posY + 32 - (k * 16));
				}




				index = 0;
				color = (sf::Color::Transparent);

				if (tileNumber != -1000)
				{					
					if (tileNumber >= k)
					{

						//this is where we would select the different tiles from the tileset
						//but we are just changing the colors so we dont have to draw everyting yet
						if (k == 0) { color = (sf::Color(G_dkblue_x, G_dkblue_y, G_dkblue_z)); }
						else if (k == 1) { color = (sf::Color(G_blue_x, G_blue_y, G_blue_z)); }
						else if (k == 2) { color = (sf::Color(G_ltblue_x, G_ltblue_y, G_ltblue_z)); }
						else if (k == 3) { color = (sf::Color(G_tan_x, G_tan_y, G_tan_z)); }
						else if (k == 4) { color = (sf::Color(G_green_x, G_green_y, G_green_z)); }
						else if (k == 5) { color = (sf::Color(G_dkgreen_x, G_dkgreen_y, G_dkgreen_z)); }
						else if (k == 6) { color = (sf::Color(G_purple_x, G_purple_y, G_purple_z)); }
						else { color = (sf::Color(G_dkpurple_x, G_dkpurple_y, G_dkpurple_z)); }

					}

				}
				else if(k == 0)
				{
					index = 22;
					color = sf::Color(G_black_x, G_black_y, G_black_z);
				}

				for (int k = 0; k < 24; k += 1)
				{
					triangles[k].color = color;
				}

				if (iX > 0 && iX < m_mapSize && iY > 0 && iY < m_mapSize)
				{
					for (int j = 0; j < 4; j++)
					{
						if (k == tileNumber) index = getIndex(iX, iY, j);
						else
						{
							switch (j) {
							case 0:
								index = 2;
								break;
							case 1:
								index = 0;
								break;
							case 2:
								index = 3;
								break;
							case 3:
								index = 1;
								break;
							}
						}


						tu = 32 * (index % (m_tileset.getSize().x / 32));
						tv = 32 * (index / (m_tileset.getSize().x / 32));

						// define the 6 matching texture coordinates
						triangles[(j * 6)].texCoords = sf::Vector2f(tu + 0, tv + 0);
						triangles[(j * 6) + 1].texCoords = sf::Vector2f(tu + 32, tv + 0);
						triangles[(j * 6) + 2].texCoords = sf::Vector2f(tu + 0, tv + 32);
						triangles[(j * 6) + 3].texCoords = sf::Vector2f(tu + 0, tv + 32);
						triangles[(j * 6) + 4].texCoords = sf::Vector2f(tu + 32, tv + 0);
						triangles[(j * 6) + 5].texCoords = sf::Vector2f(tu + 32, tv + 32);
					}
				}
				else
				{
					for (int j = 0; j < 4; j++)
					{
						tu = 32 * (index % (m_tileset.getSize().x / 32));
						tv = 32 * (index / (m_tileset.getSize().x / 32));

						// define the 6 matching texture coordinates
						triangles[(j * 6)].texCoords = sf::Vector2f(tu + 0, tv + 0);
						triangles[(j * 6) + 1].texCoords = sf::Vector2f(tu + 32, tv + 0);
						triangles[(j * 6) + 2].texCoords = sf::Vector2f(tu + 0, tv + 32);
						triangles[(j * 6) + 3].texCoords = sf::Vector2f(tu + 0, tv + 32);
						triangles[(j * 6) + 4].texCoords = sf::Vector2f(tu + 32, tv + 0);
						triangles[(j * 6) + 5].texCoords = sf::Vector2f(tu + 32, tv + 32);
					}
				}
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		_target.draw(m_vertices[i], &m_tileset); //Need to include the texture???
	}

	return true;
}








bool Map::drawMiniMap(sf::RenderTarget& _target, int _gridSize, sf::Vector2i _pos)
{

	int tileNumber;

	int posX;
	int posY;

	int s = 1;

	int xOffset = _target.getSize().x - _gridSize * s;
	int yOffset = _target.getSize().y - _gridSize * s;

	sf::Color color;


		// resize the vertex array to fit the level size
		m_mapVertices.setPrimitiveType(sf::Triangles);
		m_mapVertices.resize((_gridSize) * (_gridSize) * 6);

		for (int z = 0; z < _gridSize; ++z)
		{
			for (int i = 0; i < _gridSize; ++i)
			{
				tileNumber = m_mapTile[i][z];

				// get a pointer to the triangles' vertices of the current tile
				sf::Vertex* triangles = &m_mapVertices[(i + z * _gridSize) * 6];

				posX = xOffset + i * s;
				posY = yOffset + z * s;

				triangles[0].position = sf::Vector2f(posX, posY);
				triangles[1].position = sf::Vector2f(posX + s, posY);
				triangles[2].position = sf::Vector2f(posX, posY + s);
				triangles[3].position = sf::Vector2f(posX, posY + s);
				triangles[4].position = sf::Vector2f(posX + s, posY);
				triangles[5].position = sf::Vector2f(posX + s, posY + s);


				if (tileNumber != -1000)
				{

					//this is where we would select the different tiles from the tileset
					//but we are just changing the colors so we dont have to draw everyting yet
					if (tileNumber == 0) { color = (sf::Color(G_dkblue_x, G_dkblue_y, G_dkblue_z)); }
					else if (tileNumber == 1) { color = (sf::Color(G_blue_x, G_blue_y, G_blue_z)); }
					else if (tileNumber == 2) { color = (sf::Color(G_ltblue_x, G_ltblue_y, G_ltblue_z)); }
					else if (tileNumber == 3) { color = (sf::Color(G_tan_x, G_tan_y, G_tan_z)); }
					else if (tileNumber == 4) { color = (sf::Color(G_green_x, G_green_y, G_green_z)); }
					else if (tileNumber == 5) { color = (sf::Color(G_dkgreen_x, G_dkgreen_y, G_dkgreen_z)); }
					else if (tileNumber == 6) { color = (sf::Color(G_purple_x, G_purple_y, G_purple_z)); }
					else { color = (sf::Color(G_dkpurple_x, G_dkpurple_y, G_dkpurple_z)); }
				}


				if (i == _pos.x && z == _pos.y) color = sf::Color::White;

				for (int k = 0; k < 6; k += 1)
				{
					triangles[k].color = color;
				}
			}
		}
	


	_target.draw(m_mapVertices); //Need to include the texture???


	return true;
}









//-----SORT ARRAY LARGE TO SMALL ------
void Map::sortMapValue()
{
	std::cout << "SORTING MAP ";

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
		if(i % 10 == 0)std::cout << ". ";

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

	std::cout << std::endl << std::endl;
}







//------DRAW VERTICE ARRAY-------

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	for (int i = 0; i < 8; i++)
	{
		target.draw(m_vertices[i], states);
	}

}


