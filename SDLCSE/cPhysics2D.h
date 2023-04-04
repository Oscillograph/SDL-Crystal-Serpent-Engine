#ifndef CPHYSICS2D_H
#define CPHYSICS2D_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	/* ------------------
	  This is a physics subsystem class
	  ------------------ */
	class cPhysics2D {
	public:
		int i, j, k, l; // indexes for cycles
		int time0;
		float time;
		bool timeToReset;
		
		cPhysics2D(){}
		~cPhysics2D(){}
		
		void timeReset(){
			time0 = SDL_GetTicks64();
			time = time0 / 1000.f;
		}
		
		void timeReset(cGameObject* object){
			object->time0 = SDL_GetTicks64();
		}
		
		void getTime(){
			time = (SDL_GetTicks64() - time0) / 1000.f;
		}
		
		void getTime(cGameObject* object){
			time = (SDL_GetTicks64() - object->time0) / 1000.f;
		}
		
		void move(cGameObject* object){
			object->centerOfMass = {round(object->centerOfMass.x + object->velocity[0]*time),
									round(object->centerOfMass.y + object->velocity[1]*time)};
			object->correctGeometry();
		}
		
		void unMove(cGameObject* object){
			object->centerOfMass = {round(object->centerOfMass.x - object->velocity[0]*time),
									round(object->centerOfMass.y - object->velocity[1]*time)};
			object->correctGeometry();
		}
		
		bool outOfBounds(SDL_Rect rect, cGameObject* object){
			if (object->place.x < rect.x){ // far to the left
				return true;
			}
			
			if (object->place.x > (rect.x + rect.w)){ // far to the right
				return true;
			}
			
			if (object->place.y < rect.y){ // far to the top
				return true;
			}
			
			if (object->place.y > (rect.y + rect.h)){ // far to the bottom
				return true;
			}
			
			return false; // commenting this line makes the screen GO WILD
		}
	};
}

#endif
