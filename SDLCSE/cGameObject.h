#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	enum cGameObjectState {
		GAME_OBJECT_STATE_BIRTH,
		GAME_OBJECT_STATE_IDLE,
		GAME_OBJECT_STATE_STAND1,
		GAME_OBJECT_STATE_STAND2,
		GAME_OBJECT_STATE_STAND3,
		GAME_OBJECT_STATE_CAST1,
		GAME_OBJECT_STATE_CAST2,
		GAME_OBJECT_STATE_CAST3,
		GAME_OBJECT_STATE_WALK,
		GAME_OBJECT_STATE_RUN,
		GAME_OBJECT_STATE_ATTACK1,
		GAME_OBJECT_STATE_ATTACK2,
		GAME_OBJECT_STATE_ATTACK3,
		GAME_OBJECT_STATE_DEATH,
		GAME_OBJECT_STATE_REMOVE,
		GAME_OBJECT_STATE_TOTAL
	};
	
	/*---------------------
	  This is a common game object
	  -------------------- */
	class cGameObject {
	public:
		std::string Name;
		// int type; // image, effect, decoration, character, 
		int layer; // background, scene, foreground
		int state; // state of the object
		
		// 2D physics data 
		int time0; // when did the movement start
		int mass;
		int velocity[2]; // 0 - x, 1 - y
		int acceleration[2]; // 0 - x, 1 - y
		SDL_Point centerOfMass;
		
		// appearance data
		SDL_Point geometryCorrection;
		SDL_Rect place;
		cTexture* sprite;
		
		// cGameObject() {}
		// ~cGameObject() {}
		virtual void updateState(){};
		virtual void setGraphics(cTexture* newGraphics){
			sprite = newGraphics;
		};
		virtual cTexture* getGraphics(){
			return sprite;
		};
		
		virtual void correctGeometry(){
			place = {centerOfMass.x - geometryCorrection.x,
					 centerOfMass.y - geometryCorrection.y};
		}
	};
	
	class cTextObject : public cGameObject{ // for text boxes
	public:
	};
	
	class cDecorationObject : public cGameObject{ // for various graphic decorations
	};
	
	class cDoodadObject : public cGameObject{ // for various objects on screen
	public:
		bool destructible;
		bool interactive;
	};
	
	class cPropsObject : public cGameObject{ // for various objects on screen
	public:
		bool destructible;
		bool interactive;
	};
	
	class cActiveObject : public cGameObject{ // for npc, heroes, etc.
	public:
		bool aiControlled;
		bool playerControlled;
	};
	
	class cMainCharacter : public cGameObject{
	public:
		int health;
		int energy;
		int turnsMax;
		int turnsLeft;
		
		// S.P.E.C.I.A.L.
		int strength;
		int perception;
		int endurance;
		int charisma;
		int intellect;
		int agility;
		int luck;
	};
}

#endif
