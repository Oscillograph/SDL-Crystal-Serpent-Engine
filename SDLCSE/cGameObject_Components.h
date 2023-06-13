#ifndef CGAMEOBJECT_COMPONENTS_H
#define CGAMEOBJECT_COMPONENTS_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	struct animationFrames{
		SDL_Point begin; // upper left corner of the first frame
		SDL_Point end; // upper left corner of the last frame
		int width; // width of a frame
		int height; // height of a frame
		int framesTotal; // equals (end.x - begin.x)/width;
		float framesPerSecond; // basically, equals 1/secondsPerFrame
		float secondsPerFrame; // basically, equals 1/framesPerSecond
		int timeBetweenFrames;
		bool loop;
		
		animationFrames(){
			begin = {0, 0};
			end = {0, 0};
			width = 0;
			height = 0;
			framesTotal = 0;
			framesPerSecond = 1.0f;
			secondsPerFrame = 1.0f;
			timeBetweenFrames = 1000;
			loop = false;
		}
		
		animationFrames(SDL_Point beginPoint, SDL_Point endPoint, int w, int h, float fps, bool loopIt){
			begin = beginPoint;
			end = endPoint;
			width = w;
			height = h;
			framesTotal = (int)round(std::abs((endPoint.x - beginPoint.x) / w));
			/*if (endPoint.x > beginPoint.x){
				framesTotal = (int)round((endPoint.x - beginPoint.x) / w);
			} else {
				framesTotal = (int)round((beginPoint.x - endPoint.x) / w);
			}*/
			
			if (framesTotal == 0){
				framesTotal = 1;
			}
			framesPerSecond = fps;
			secondsPerFrame = 1/fps;
			timeBetweenFrames = (int)round(1000/fps);
			loop = loopIt;
		}
	};
	
	namespace Components{
		struct base {
			std::string name;
			int state;
		};
		struct text {
			std::u32string content;
		};
		
		struct hitBox {
			std::vector<SDL_FPoint> points;
			int type;
			float radius; // looking here, if here's only one point
		};
		
		struct group {
			int order; // order for display
			entt::entity parent; // link to the group-holder entity
			entt::entity previous; // for a list of entities;
			entt::entity next; // for a list of entities;
			std::vector<entt::entity> children; // if this entity is a group-holder
		};
		
		struct sprite {
			cTexture* sprite;
			SDL_FRect place;
			SDL_Rect window;
			SDL_FPoint geometryCorrection;
			bool display;
		};
		
		struct animation{
			std::map<int, animationFrames*> frames; // [ANIM_CODE] = {};
			int currentAnimation;
			int currentFrame;
			int timeBefore; // last time this component was checked
			int framesTotal;
			bool paused;
			
			animation(){
				paused = true;
				currentAnimation = Objects::GAME_OBJECT_STATE_IDLE;
				currentFrame = 0;
				timeBefore = SDL_GetTicks64();
				framesTotal = 0;
				
				for (int i = Objects::GAME_OBJECT_STATE_BEGIN+1; i < Objects::GAME_OBJECT_STATE_TOTAL; i++){
					frames[i] = NULL;
				}
			}
			
			void add(int state, animationFrames* animFrames){
				frames[state] = animFrames;
			}
			
			void set(int state){
				currentAnimation = state;
				framesTotal = frames[currentAnimation]->framesTotal;
				currentFrame = 0;
			}
			
			void stop(){
				paused = true;
				currentFrame = 0;
			}
			
			void start(){
				paused = false;
				currentFrame = 0;
			}
			
			void pause(){
				paused = true;
			}
			
			void unpause(){
				paused = false;
			}
			
			void reset(){
				currentFrame = 0;
			}
			
			void change(int state, animationFrames* animFrames){
				if (frames[state] != NULL){
					delete frames[state];
				}
				add(state, animFrames);
			}
		};
		
		struct controlKeyBoard {
			// int stands for command from enum
			std::unordered_map<int, SDL_Keycode> KBControls;
			
			controlKeyBoard(){
				for (int i = Commands::KBCommand_BEGIN+1; i < Commands::KBCommand_TOTAL; i++){
					KBControls[i] = SDLK_UNKNOWN;
				}
			}
			
			void set(int state, SDL_Keycode code){
				KBControls[state] = code;
			}
			
			SDL_Keycode get(int state){
				return KBControls[state];
			}
		};
		
		struct controlMouse {
			// int stands for command from enum
			std::map<int, SDL_Keycode> MouseControls;
		};
		
		struct physics2D{
			// general physics
			int time0; // when did the movement start, in ticks
			float time; // local timer
			float mass; // mass (thanks, Cap'n!)
			SDL_FPoint centerOfMass; // center of mass (thanks, Cap'n a lot!)
			
			// dynamics
			SDL_FPoint velocity;
			SDL_FPoint acceleration;
			
			// characteristics 
			float elasticity; // коэффициент эластичности (как сжимается)
			bool canBounce; // can bounce from an object
			bool canPush; // can hit another object to make it bounce
			bool canMove;
			float bounce; // коэффициент упругости (как разжимается)
			float friction; // коэффициент трения
			
			// rotation section
			bool canRotate; // because objects could be irrotateable
			float rotationAngle; // in radians
			float rotationSin; // sine of the rotation angle
			float rotationCos; // cosine of the rotation angle
		};
		
		struct stateStack {
			int stack[StateStackMax];
			int stack_size;
			int current;
		};
	}
}

#endif
