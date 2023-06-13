#ifndef CGAMEOBJECT_H
#define CGAMEOBJECT_H

// SDLCSE
#include "./common.h"
#include "./cGameObject_Entities.h"
#include "./cGameObject_Components.h"
#include "./cGameObject_Systems.h"

namespace SDLCSE {
	namespace Objects{
		class cText{
		public:
			std::u32string content;
		};
		
		struct sHitBox{
			std::vector<SDL_FPoint> points;
			float radius; // looking here, if here's only one point
		};
		
		class cAppearanceData{
		public:
			// appearance data
			SDL_FPoint geometryCorrection;
			SDL_FRect place;
			cTexture* sprite;
			bool display;
		};
		
		class c2DPhysics{
		public:
			// 2D physics data 
			int time0; // when did the movement start, in ticks
			float time; // local timer
			float mass;
			float velocity[2]; // 0 - x, 1 - y
			float acceleration[2]; // 0 - x, 1 - y
			float elasticity; // коэффициент эластичности (как сжимается)
			bool canBounce;
			bool canPush; // can hit another object to make it bounce
			float bounce; // коэффициент упругости (как разжимается)
			float friction; // коэффициент трения
			sHitBox hitbox;
			SDL_FPoint centerOfMass;
		};
		
		/*---------------------
		  This is a common game object
		  -------------------- */
		class cGameObject : public c2DPhysics, public cAppearanceData, public cText {
		public:
			std::string Name;
			int type; // image, effect, decoration, character, 
			int layer; // background, scene, foreground
			int state; // state of the object
			int hitbox; // shape that describes the hit box
		};
		
		class cTextObject : public cGameObject{ // for text boxes
		public:
		};
		
		class cDecorationObject : public cGameObject{ // for various graphic decorations
		public:
			cDecorationObject(cAppearanceData &Look){
				Appearance = Look;
			}
		private:
			cAppearanceData Appearance;
		};
		
		cDecorationObject* getDecoration(cAppearanceData &Appearance){
			return new cDecorationObject(Appearance);
		}
		
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
		
		class cUnitObject : public cGameObject{ // for npc, heroes, etc.
		public:
			bool aiControlled;
			bool playerControlled;
		};
		
		
		// Characters stats and classes
		class cCharacterBasicStats {
			int health;
			int energy;
			int turnsMax;
			int turnsLeft;
		};
		
		class cCharacterSPECIALStats {
			// S.P.E.C.I.A.L.
			int strength;
			int perception;
			int endurance;
			int charisma;
			int intellect;
			int agility;
			int luck;
		};
		
		class cCharacter : public cGameObject, public cCharacterBasicStats, public cCharacterSPECIALStats {
		public:
			
		};
		
		namespace Primitives{
			class cPhysics{
				float time; // internal timer
				float vx, vy; // speed vector
				float ax, ay; // acceleration vector
				float mass;
			};
			
			class cPoint{
				float x, y; // coordinates
				int id;
			};
			
			class cLine{
				float x1, x2; // coordinates of the starting point
				float y1, y2; // coordinates of the ending point
				int id;
			};
			
			class cCircle{
			public:
				float x, y; // coordinates of the center
				float r; // radius
				float angle; // rotation in radians
				int id;
			};
			
			class cRectangle{
			public:
				float x, y; // coordinates of the upper left corner
				float w, h; // width and height
				float angle; // rotation in radians
				int id;
			};
			
			class cTriangle{
			public:
				float x1, x2, x3; // coordinates of points
				float y1, y2, y3; // coordinates of points
				float angle; // rotation in radians
				int id;
			};
		}
		
		class cHitBox{
		public:
			int type;
			std::vector<SDL_FPoint> points; // points for polygon or a point for circle
			float radius; // if there's only one point
			float angle; // rotation in radians
			int id;
		};
		
		class cHitBoxServitor{
		public:
			cHitBoxServitor(){};
			~cHitBoxServitor(){};
			// find normals to all edges of a polygon
			// (!) we seek only one normal per an edge
			std::vector<SDL_FPoint> findNormals(cHitBox* hitbox){
				std::vector<SDL_FPoint> normals; // prepare the normals
				int i = 0;
				if (hitbox->points.size() > 1){
					for (i=0; i < (hitbox->points.size()-1); i++){
						normals.push_back(Math::findNormal(hitbox->points[i], hitbox->points[i+1]));
					}
					normals.push_back(Math::findNormal(hitbox->points[i], hitbox->points[0]));
				} else 
					normals.clear();
				return normals; // keep in mind that normals.size() can be equal to zero
			}
		};
	}
}

#endif
