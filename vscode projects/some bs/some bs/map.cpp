#pragma once

#include "map.hpp"



//-----CONSTRUCTOR------

Map::Map(const int _size)
{
	m_mapSize = _size;



	for (int i = 0; i <= m_mapSize; i++)
	{
		m_mapSort[i] = new short int* [m_mapSize+1];

		for (int z = 0; z <= m_mapSize; z++)
		{
			m_mapSort[i][z] = new short int[2];
		}
	}

	for (int i = 0; i <= m_mapSize; i++)
	{
		m_map[i] = new short int [m_mapSize+1];
	}

	for (int i = 0; i <= m_mapSize; i++)
	{
		m_mapTile[i] = new short int[m_mapSize + 1];
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
	}
	
	delete[] m_mapSort;
	delete[] m_mapTile;
	delete[] m_map;

	std::cout << "***** MAP DESTROYED *****" << std::endl;

}






//------DIAMOND SQUARE------

void Map::newMap(int _high, float _roughness, float _change)
{

	//---INITIALIZE VARIABLES----
	int div;

	//points to extrapolate from for square step
	int avg, x1y1, x2y1, x1y2, x2y2, x1y, x2y, xy1, xy2;



	//create a chunk the size of the grid that will be halfed every itteration
	float chunk = m_mapSize;
	//to find the center of the chunk we are working with
	float half = m_mapSize / 2;
	//to initialize the grid so we know if we missed one
	int empty = -1000;



	//---INITIALIZE MAP----

	//initialize the grid (1 cell grater then the size in the x and y ge
	for (int z = 0; z <= m_mapSize; ++z)
	{
		for (int i = 0; i <= m_mapSize; ++i)
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
				x1y1 = m_map[(int)floor(i)][(int)floor(z)];
				x2y1 = m_map[(int)floor(i + chunk)][(int)floor(z)];
				x1y2 = m_map[(int)floor(i)][(int)floor(z + chunk)];
				x2y2 = m_map[(int)floor(i + chunk)][(int)floor(z + chunk)];


				avg = round((x1y1 + x2y1 + x1y2 + x2y2) / 4);
				avg += randRange(-_roughness, _roughness);

				//set the value "0" with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)floor(i + half)][(int)floor(z + half)] == empty)
				{
					m_map[(int)floor(i + half)][(int)floor(z + half)] = avg;

					if (avg < -10)     { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 0; }
					else if (avg < -5) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 1; }
					else if (avg < 0)  { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 2; }
					else if (avg < 5)  { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 3; }
					else if (avg < 10) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 4; }
					else if (avg < 15) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 5; }
					else if (avg < 20) { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 6; }
					else               { m_mapTile[(int)floor(i + half)][(int)floor(z + half)] = 7; }
				}
			}
		}



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
				avg = round((x1y + x2y + xy1 + xy2) / 4);
				avg += randRange(-_roughness, _roughness);



				//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
				if (m_map[(int)floor(i)][(int)floor(z)] == empty)
				{
					m_map[(int)floor(i)][(int)floor(z)] = avg;

					if (avg < -10)     { m_mapTile[(int)floor(i)][(int)floor(z)] = 0; }
					else if (avg < -5) { m_mapTile[(int)floor(i)][(int)floor(z)] = 1; }
					else if (avg < 0)  { m_mapTile[(int)floor(i)][(int)floor(z)] = 2; }
					else if (avg < 5)  { m_mapTile[(int)floor(i)][(int)floor(z)] = 3; }
					else if (avg < 10) { m_mapTile[(int)floor(i)][(int)floor(z)] = 4; }
					else if (avg < 15) { m_mapTile[(int)floor(i)][(int)floor(z)] = 5; }
					else if (avg < 20) { m_mapTile[(int)floor(i)][(int)floor(z)] = 6; }
					else               { m_mapTile[(int)floor(i)][(int)floor(z)] = 7; }
				}
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

bool Map::drawMap(sf::RenderTarget& _target, sf::Vector2i _tileSize, sf::Vector2i _position, sf::Vector2i _gridSize, const std::string& _tileset)
{
	if (!m_tileset.loadFromFile(_tileset))
		return -1;

	_gridSize.x += 2;
	_gridSize.y += 18;
	_position.y -= 2;

	int tileNumber;
	int index;

	int iX, iY;
	int posX, posY;
	int tu, tv;


	sf::Color color;


	// populate the vertex array, with two triangles per tile
	for (int k = 0; k < 8; k++)
	{
		// resize the vertex array to fit the level size
		m_vertices[k].setPrimitiveType(sf::Triangles);
		m_vertices[k].resize((_gridSize.x) * (_gridSize.y) * 6);

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
				sf::Vertex* triangles = &m_vertices[k][(i + z * _gridSize.x) * 6];

				posX = (((i)*_tileSize.x) + ((z % 2) * _tileSize.x) / 2) - _tileSize.x;
				posY = (((z)*_tileSize.y) / 2) - _tileSize.y;


				triangles[0].position = sf::Vector2f(posX, posY-(k*16));
				triangles[1].position = sf::Vector2f(posX + 32, posY - (k * 16));
				triangles[2].position = sf::Vector2f(posX, posY + 32 - (k * 16));
				triangles[3].position = sf::Vector2f(posX, posY + 32 - (k * 16));
				triangles[4].position = sf::Vector2f(posX + 32, posY - (k * 16));
				triangles[5].position = sf::Vector2f(posX + 32, posY + 32 - (k * 16));

				index = 0;
				color = (sf::Color::Transparent);

				if (tileNumber != -1000)
				{					
					if (tileNumber >= k)
					{
						index = 0;

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
					else if (k == 3)
					{
						index = 21;
						color = sf::Color(G_blue_x, G_blue_y, G_blue_z, 150);
					}
					else
					{
						color = (sf::Color::Transparent);
					}

				}
				else if(k == 0)
				{
					index = 22;
					color = sf::Color(G_black_x, G_black_y, G_black_z);
				}

				for (int k = 0; k < 6; k += 1)
				{
					triangles[k].color = color;
				}

				tu = 32 * (index % (m_tileset.getSize().x / 32));
				tv = 32 * (index / (m_tileset.getSize().x / 32));

				// define the 6 matching texture coordinates
				triangles[0].texCoords = sf::Vector2f(tu + 0, tv + 0);
				triangles[1].texCoords = sf::Vector2f(tu + 32, tv + 0);
				triangles[2].texCoords = sf::Vector2f(tu + 0, tv + 32);
				triangles[3].texCoords = sf::Vector2f(tu + 0, tv + 32);
				triangles[4].texCoords = sf::Vector2f(tu + 32, tv + 0);
				triangles[5].texCoords = sf::Vector2f(tu + 32, tv + 32);

			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		_target.draw(m_vertices[i], &m_tileset); //Need to include the texture???
	}

	return true;
}







//-----SORT ARRAY LARGE TO SMALL ------
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


