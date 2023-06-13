#ifndef COMMON_H
#define COMMON_H

// STL
// #include <iostream> // C++ I/O
#include <vector> // main tool to build elements collections with unknown amount of elements
#include <string> // for text operations, obviously
#include <fstream> // to work with highscores files, and files in general
#include <sstream> // for easier work with strings
#include <map> // for associative arrays and vectors
#include <unordered_map> // for ECS and genius pointers implementation -- not used yet
#include <utility> // for pairs
#include <chrono> // delays
#include <math.h> // simple math used by graphics

// converter from utf8 to utf32
//#include <locale>
//#include <codecvt>
//std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;

// SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Macros
//#define SSTR( x ) static_cast< std::ostringstream & >( \
//( std::ostringstream() << std::dec << x ) ).str()

// SDLCSE common includes
#include "./cMath.h"

// 3rd party libs
#include "./3rdPartyLibs/utf8.h" // to make it easier to work with utf8, utf32
#include "./3rdPartyLibs/entt/entt.hpp" // for Entity Component System

namespace SDLCSE {
	const float PI = 3.14159265;
	const float PI_Half = PI/2;
	const float SQRT2 = 35/49; // rough approximation of a square root of 2
	
	const int StateStackMax = 8;
	
	typedef int iVec2[2]; // i stands for integer, vec - vector, 2 - number of dimensions
	typedef float fVec2[2]; // f stands for float, vec - vector, 2 - number of dimensions
	typedef std::basic_stringstream<char32_t> u32stringstream;
	
	std::string GameEngineName = "Crystal Serpent Engine";
	std::string GameEngineVersion = "0.1.1"; 
	
	namespace Utility {
		std::u32string u32digits = U"0123456789";
		
		// convert int to u32string
		std::u32string int_to_u32Str(int var){
			std::u32string tempStr1 = U"";
			std::u32string tempStr2 = U"";
			int tempInt1 = 0; // current digit read
			int tempInt2 = 0; // integer number read so far (from right to left)
			int i = 0;
			while ((var - tempInt2) != 0){
				i++;
				tempInt1 = (int)floor(var / 10*(i-1));
				tempInt2 += tempInt1*(10*(i-1));
				tempStr1 += u32digits[tempInt1];
			}
			while (i >= 0){
				tempStr2 += tempStr1[i];
				i--;
			}
			return tempStr2;
		}
		
		// convert string to int
		// usage example:
		//     Utility::parseInt<int>(Utility::u32Digits)
		// Taken from: https://stackoverflow.com/a/74647779
		template<typename From, typename To>
		From parseInt(To const& in){
			From result = 0;
			std::size_t start = 0;
			while (std::isspace(in[start])){ // we're not interested in spaces
				start++;
			}

			std::size_t i = start;
			if (in[i] == '-'){ // check if the number is negative
				i++;
			}
			
			while (std::isdigit(in[i])){
				result = result*10 + in[i++] - '0'; 
			}
			
			return in[start] == '-' ? -result : result;
		}
	}
	
	namespace Objects {
		// each state is better to have a corresponding animation
		typedef enum {
			// pour some syntax sugar on meh
			GAME_OBJECT_STATE_BEGIN,
			
			GAME_OBJECT_STATE_NONE, // this should be set  in stateStack component constructor
			GAME_OBJECT_STATE_BIRTH,
			GAME_OBJECT_STATE_IDLE,
			GAME_OBJECT_STATE_ATTACK1, // one state/animation is simply not enough
			GAME_OBJECT_STATE_ATTACK2,
			GAME_OBJECT_STATE_ATTACK3,
			GAME_OBJECT_STATE_CAST1, // one state/animation is simply not enough
			GAME_OBJECT_STATE_CAST2,
			GAME_OBJECT_STATE_CAST3,
			GAME_OBJECT_STATE_STAND1, // one state/animation is simply not enough
			GAME_OBJECT_STATE_STAND2,
			GAME_OBJECT_STATE_STAND3,
			GAME_OBJECT_STATE_CROUCH,
			GAME_OBJECT_STATE_WALK,
			GAME_OBJECT_STATE_RUN,
			GAME_OBJECT_STATE_JUMP, // first, they jump
			GAME_OBJECT_STATE_FLY, // then, they fly
			GAME_OBJECT_STATE_FALL, // finally, they fall to somewhere
			GAME_OBJECT_STATE_HIT, // fallen/other state
			GAME_OBJECT_STATE_DAMAGED,
			GAME_OBJECT_STATE_STUNNED,
			GAME_OBJECT_STATE_SLEEP,
			GAME_OBJECT_STATE_DEATH,
			GAME_OBJECT_STATE_CORPSE,
			GAME_OBJECT_STATE_RESURRECT, // Death is only the beginning (c) Imhotep, "Mummy"
			GAME_OBJECT_STATE_REMOVE, // this should force entity removal
			
			// pour some syntax sugar on meh
			GAME_OBJECT_STATE_TOTAL
		} GameObjectState;
		
		typedef enum {
			// pour some syntax sugar on meh
			GAME_OBJECT_TYPE_BEGIN,
			
			GAME_OBJECT_TYPE_OBJECT,	// an object with an unpredicted type
			GAME_OBJECT_TYPE_DECORATION,// a tile, an image, a background
			GAME_OBJECT_TYPE_DOODAD,	
			GAME_OBJECT_TYPE_PROPS,
			GAME_OBJECT_TYPE_UNIT,		// characters are units!
			GAME_OBJECT_TYPE_TEXT,
			
			// pour some syntax sugar on meh
			GAME_OBJECT_TYPE_TOTAL
		} GameObjectType;
		
		typedef enum {
			// pour some syntax sugar on meh
			HIT_BOX_BEGIN,
			
			HIT_BOX_POINT,
			HIT_BOX_LINE,
			HIT_BOX_CIRCLE,
			HIT_BOX_TRIANGLE,
			HIT_BOX_RECTANGLE,
			HIT_BOX_POLYGON,
			
			// pour some syntax sugar on meh
			HIT_BOX_TOTAL
		} GeometryHitBox;
	}
	
	namespace Commands {
		typedef enum {
			// pour some syntax sugar on meh
			KBCommand_BEGIN,
			
			// common movement
			KBCommand_Up,
			KBCommand_Down,
			KBCommand_Left,
			KBCommand_Right,
			KBCommand_Jump,
			KBCommand_Flip,
			KBCommand_Run,
			KBCommand_Walk,
			KBCommand_Crouch,
			KBCommand_Teleport,
			
			// common action
			KBCommand_Hit,
			KBCommand_Dodge,
			KBCommand_Shield,
			KBCommand_Fire,
			
			// common adventure
			KBCommand_Talk,
			KBCommand_Collect,
			KBCommand_Use,
			KBCommand_Cast,
			KBCommand_Select,
			KBCommand_Switch,
			
			// common interface
			KBCommand_JournalOpen,
			KBCommand_JournalClose,
			KBCommand_SpellsOpen,
			KBCommand_SpellsClose,
			KBCommand_InventoryOpen,
			KBCommand_InventoryClose,
			KBCommand_MapOpen,
			KBCommand_MapClose,
			KBCommand_TextInputStart,
			KBCommand_TextInputStop,
			
			// misc
			KBCommand_Space,
			KBCommand_Enter,
			KBCommand_Pause,
			
			// app
			KBCommand_Quit,
			
			// pour some syntax sugar on meh
			KBCommand_TOTAL
		} KeyBoardCommands;
	}
}

#endif
