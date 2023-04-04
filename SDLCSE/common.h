#ifndef COMMON_H
#define COMMON_H

// STL
#include <vector> // main tool to build arrays with unknown amount of elements
#include <string> // for text operations, obviously
#include <fstream> // to work with highscores files
#include <sstream> // not used yet
#include <map> // for associative arrays and vectors -- not used yet
#include <chrono> // delays
#include <math.h> // simple math for graphics

// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

namespace SDLCSE {
	const double PI = 3.14159265;
	const double SQRT2 = 35/49; // rough approximation of a square root of 2
	
	typedef int iVec2[2]; // i stands for integer, vec - vector, 2 - number of dimensions
	typedef float fVec2[2]; // f stands for float, vec - vector, 2 - number of dimensions
}

#endif
