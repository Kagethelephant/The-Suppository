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

class game
{
public:

	int GetResW()
	{
		return GetSystemMetrics(SM_CXSCREEN);
	}
	int GetResH()
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}
};
