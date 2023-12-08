#pragma once

#include "map.hpp"

Map::Map(int _size)
{
	m_mapSize = _size;
}

void Map::newMap(float _map[G_mapAlloc][G_mapAlloc], int _high, float _roughness, float _change)
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
			_map[i][z] = empty;
		}
	}

	//set initial corner values to extrapolate from at each corner
	_map[0][0] = randRange(-_roughness, _roughness);
	_map[0][m_mapSize] = randRange(-_roughness, _roughness);
	_map[m_mapSize][0] = randRange(-_roughness, _roughness);
	_map[m_mapSize][m_mapSize] = randRange(-_roughness, _roughness);


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
				x1y1 = _map[(int)round(i)][(int)round(z)];
				x2y1 = _map[(int)round(i + chunk)][(int)round(z)];
				x1y2 = _map[(int)round(i)][(int)round(z + chunk)];
				x2y2 = _map[(int)round(i + chunk)][(int)round(z + chunk)];

				avg = (x1y1 + x2y1 + x1y2 + x2y2) / 4;

				//set the value "0" with the average + a random value and make sure we are not overwriting other values
				if (_map[(int)round(i + half)][(int)round(z + half)] == empty) _map[(int)round(i + half)][(int)round(z + half)] = avg + randRange(-_roughness, _roughness);
			}
		}



		//----DIAMOND----

		for (float z = 0; z < m_mapSize + 1; z += half)
		{
			for (float i = fmodf((z + half), chunk); i < m_mapSize + 1; i += chunk)
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
					x1y = _map[(int)round(i - half)][(int)round(z)];
					div += 1;
				}

				if (i + half < m_mapSize + 1)
				{
					x2y = _map[(int)round(i + half)][(int)round(z)];
					div += 1;
				}

				if (z + half < m_mapSize + 1)
				{
					xy1 = _map[(int)round(i)][(int)round(z + half)];
					div += 1;
				}

				if (z - half >= 0)
				{
					xy2 = _map[(int)round(i)][(int)round(z - half)];
					div += 1;
				}

				//calculate the average and use div to only average the amount of values grabbed
				avg = (x1y + x2y + xy1 + xy2) / 4;

				//set the value "0" (from the diagram above) with the average + a random value and make sure we are not overwriting other values
				if (_map[(int)round(i)][(int)round(z)] == empty) _map[(int)round(i)][(int)round(z)] = avg + randRange(-_roughness, _roughness);
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

bool Map::drawMap(sf::RenderTarget& _target, float _map[G_mapAlloc][G_mapAlloc], sf::Vector2i _tileSize, sf::Vector2i _position, sf::Vector2i _gridSize, bool _solidColor, const std::string& _tileset)
{

	//define some colors cause the default ones are ugly
	sf::Vector3i c_black(13, 14, 26);
	sf::Vector3i c_dkblue(33, 47, 106);
	sf::Vector3i c_blue(47, 80, 118);
	sf::Vector3i c_ltblue(65, 95, 120);
	sf::Vector3i c_tan(116, 113, 89);
	sf::Vector3i c_green(49, 83, 76);
	sf::Vector3i c_dkgreen(34, 53, 59);
	sf::Vector3i c_dkpurple(43, 37, 49);
	sf::Vector3i c_purple(77, 61, 85);
	sf::Vector3i c_snow(182, 182, 182);

	// load the tileset texture
	if (!_solidColor)
	{
		if (!m_tileset.loadFromFile(_tileset))
			return false;
	}

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
				tileNumber = _map[iX][iY];
			}
			else tileNumber = -10000;

			int posX = (i * _tileSize.x / 2);
			int posY = z * _tileSize.y - ((i % 2) * 8);

			// get a pointer to the triangles' vertices of the current tile
			sf::Vertex* triangles = &m_vertices[(i + z * _gridSize.x) * 6];

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

					if (tileNumber < -10) { color = (sf::Color(c_dkblue.x, c_dkblue.y, c_dkblue.z)); }
					else if (tileNumber < -5) { color = (sf::Color(c_blue.x, c_blue.y, c_blue.z)); }
					else if (tileNumber < 0) { color = (sf::Color(c_ltblue.x, c_ltblue.y, c_ltblue.z)); }
					else if (tileNumber < 5) { color = (sf::Color(c_tan.x, c_tan.y, c_tan.z)); }
					else if (tileNumber < 10) { color = (sf::Color(c_green.x, c_green.y, c_green.z)); }
					else if (tileNumber < 15) { color = (sf::Color(c_dkgreen.x, c_dkgreen.y, c_dkgreen.z)); }
					else if (tileNumber < 20) { color = (sf::Color(c_purple.x, c_purple.y, c_purple.z)); }
					else { color = (sf::Color(c_dkpurple.x, c_dkpurple.y, c_dkpurple.z)); }
				}
				else color = sf::Color(c_black.x, c_black.y, c_black.z);

				for (int k = 0; k < 6; k += 1)
				{
					triangles[k].color = sf::Color(color);
				}

			}


			else
			{
				int index;
				// get the current tile number
				if (tileNumber < -15) { index = 0; }
				else if (tileNumber < -10) { index = 1; }
				else if (tileNumber < -5) { index = 2; }
				else if (tileNumber < 0) { index = 3; }
				else if (tileNumber < 5) { index = 4; }
				else if (tileNumber < 10) { index = 5; }
				else if (tileNumber < 15) { index = 6; }
				else if (tileNumber < 20) { index = 7; }
				else { index = 8; }

				// find its position in the tileset texture
				//int tu = index % (m_tileset.getSize().x / tileSize);
				//int tv = index / (m_tileset.getSize().x / tileSize);
				//int tu = tileNumber % (m_tileset.getSize().x / tileSize);
				//int tv = tileNumber / (m_tileset.getSize().x / tileSize);

				// define the 6 matching texture coordinates
				//triangles[0].texCoords = sf::Vector2f(tu * tileSize, tv * tileSize);
				//triangles[1].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
				//triangles[2].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
				//triangles[3].texCoords = sf::Vector2f(tu * tileSize, (tv + 1) * tileSize);
				//triangles[4].texCoords = sf::Vector2f((tu + 1) * tileSize, tv * tileSize);
				//triangles[5].texCoords = sf::Vector2f((tu + 1) * tileSize, (tv + 1) * tileSize);

			}

		}
	}

	if (_solidColor)_target.draw(m_vertices);
	else _target.draw(m_vertices, &m_tileset); //Need to include the texture???
	return true;
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


