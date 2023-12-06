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


//sets up window with a pixel scale and returns the dimmensions of the window
//in a 2 dimmensional vector
sf::Vector2i windowSetup(sf::RenderWindow&, sf::View&, int, bool, int);

