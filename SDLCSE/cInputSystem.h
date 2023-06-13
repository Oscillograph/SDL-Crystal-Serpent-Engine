#ifndef CINPUTSYSTEM_H
#define CINPUTSYSTEM_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	typedef enum {
		MOUSE_EVENT_BUTTON_UP,
		MOUSE_EVENT_BUTTON_DOWN,
		MOUSE_EVENT_LEFT_BUTTON_UP,
		MOUSE_EVENT_RIGHT_BUTTON_UP,
		MOUSE_EVENT_LEFT_BUTTON_DOWN,
		MOUSE_EVENT_RIGHT_BUTTON_DOWN,
		MOUSE_EVENT_MOVE,
		MOUSE_EVENT_TOTAL,
	} MouseEvents;
	
	struct MouseController{
		// cursor position over the application window
		SDL_Point cursor;
		// state of buttons at the moment of this structure update  
		bool leftButtonDown;
		bool leftButtonUp;
		bool rightButtonDown;
		bool rightButtonUp;
		// to control which event made this structure update
		bool eventUp;
		bool eventDown;
		bool eventMove;
		// to control which button fired an event
		bool leftButton;
		bool rightButton;
		// to control the state of buttons between the structure updates
		bool leftMouseButtonHoldDown;
		bool rightMouseButtonHoldDown;
	};
 
	
	class cInputSystem{
	public:
		MouseController Mouse; // mouse controller
		// SDL_Point touchCursorPosition; // touch screen
		// SDL_Event* e; // event queue
		
		// ---------------------------------------------------------------------------
		// Idea behind "return" is that it's tied in CSE with boolean "quit" variable
		// ---------------------------------------------------------------------------
		// Problem: This system needs to send meaningful messages to the engine.
		// ---------------------------------------------------------------------------
		
		cInputSystem(){
			Mouse.leftButtonDown = false;
			Mouse.rightButtonDown = false;
			Mouse.leftButtonUp = false;
			Mouse.rightButtonUp = false;
			
			Mouse.eventUp = false;
			Mouse.eventDown = false;
			Mouse.eventMove = false;
			
			Mouse.leftButton = false;
			Mouse.rightButton = false;
			
			Mouse.leftMouseButtonHoldDown = false;
			Mouse.rightMouseButtonHoldDown = false;
		}
		~cInputSystem(){}
		
		/*bool keyboardListener(){
			switch(e->key.keysym.sym){
			case SDLK_SPACE:
				return true;
				break;
			default:
				return false;
				break;
			}
			return false;
		};
		
		bool mouseListener(){
			Mouse.cursor.x = e->motion.x;
			Mouse.cursor.y = e->motion.y;
			return false;
		};*/
		
		void mouseListener(SDL_Event* e, int mouseEvent){
			Mouse.cursor.x = e->motion.x;
			Mouse.cursor.y = e->motion.y;
			switch(mouseEvent){
			case MOUSE_EVENT_MOVE:
				Mouse.eventMove = true;
				Mouse.eventUp = false;
				Mouse.eventDown = false;
				break;
			case MOUSE_EVENT_BUTTON_UP: //MOUSE_EVENT_LEFT_BUTTON_UP || MOUSE_EVENT_RIGHT_BUTTON_UP):
				Mouse.eventUp = true;
				Mouse.eventDown = false;
				Mouse.eventMove = false;
				
				Mouse.leftButton = (e->button.button == SDL_BUTTON_LEFT);
				Mouse.rightButton = (e->button.button == SDL_BUTTON_RIGHT);
				
				Mouse.leftButtonDown = !(e->button.button == SDL_BUTTON_LEFT);
				Mouse.leftButtonUp = (e->button.button == SDL_BUTTON_LEFT);
				Mouse.rightButtonDown = !(e->button.button == SDL_BUTTON_RIGHT);
				Mouse.rightButtonUp = (e->button.button == SDL_BUTTON_RIGHT);
				break;
			case MOUSE_EVENT_BUTTON_DOWN:
				Mouse.eventDown = true;
				Mouse.eventUp = false;
				Mouse.eventMove = false;
				
				Mouse.leftButton = (e->button.button == SDL_BUTTON_LEFT);
				Mouse.rightButton = (e->button.button == SDL_BUTTON_RIGHT);
				
				Mouse.leftButtonDown = (e->button.button == SDL_BUTTON_LEFT);
				Mouse.leftButtonUp = !(e->button.button == SDL_BUTTON_LEFT);
				Mouse.rightButtonDown = (e->button.button == SDL_BUTTON_RIGHT);
				Mouse.rightButtonUp = !(e->button.button == SDL_BUTTON_RIGHT);
				break;
			}
		};
		
		/*bool touchListener(){
			return false;
		}
		
		bool SDLEventListener(){
			while(SDL_PollEvent(e)){
				// main app events
				if (e->type == SDL_QUIT){
					return true;
				}
				
				// keyboard events
				if (e->type == (SDL_KEYDOWN || SDL_KEYUP)){
					return keyboardListener();
				}
				
				// mouse events
				if (e->type == (SDL_MOUSEMOTION || SDL_MOUSEBUTTONDOWN || SDL_MOUSEBUTTONUP)){
					return mouseListener();
				}
				
				// touch screen events
				if (e->type == (SDL_FINGERDOWN || SDL_FINGERUP || SDL_FINGERMOTION)){
					return touchListener();
				}
			}
			return false;
		};*/
	};
}

#endif
