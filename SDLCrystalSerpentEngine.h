// ==============================================================
// Crystal Serpent Engine v0.1
// Made with SDL, EnTT, UTF8-CPP, TDM-GCC 10.3.0 and love <3
//                   by Living Oscillograph
//                             aka
//                          ТехнОкраТ
// ==============================================================

#ifndef SDLCRYSTALSERPENTENGINE_H
#define SDLCRYSTALSERPENTENGINE_H

// SDLCSE
#include "SDLCSE/common.h"
#include "SDLCSE/ColorPalette.h"
#include "SDLCSE/cTexture.h"
#include "SDLCSE/cSurface.h"
#include "SDLCSE/cLogSystem.h"
#include "SDLCSE/cFontManager.h"
#include "SDLCSE/cResourceManager.h"
#include "SDLCSE/cGraphics.h"
#include "SDLCSE/cSounds.h"
#include "SDLCSE/cGameObject.h"
#include "SDLCSE/cGameObjectServitor.h"
#include "SDLCSE/cHighScoresFile.h"
#include "SDLCSE/cDebugSystem.h"
#include "SDLCSE/cPhysics2D_core.h"
#include "SDLCSE/cInputSystem.h"

// addons to CSE main class
#include "SDLCSE/cEntityComponentSystem.h"
#include "SDLCSE/cStateMachine.h"

namespace SDLCSE {
	/* ------------------
	This class essentially constructs a cosy framework from SDL functions
	------------------ */
	class cCrystalSerpentEngine : public cEntityComponentSystem {
	public:
		// --- SDL vars ---
		SDL_Event* e; // event queue
		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the window renderer 
		SDL_Surface* screenSurface; // the window surface
		SDL_Rect Rect;		  // temp region
		SDL_FRect FRect;
		SDL_Surface* surface; // temp surface
		SDL_Texture* texture; // temp texture
		
		// --- SDLCSE vars ---
		// static cCrystalSerpentEngine* instance; // this instance
		cLogSystem* LogSystem;
		cFontManager* FontManager;
		cGraphics* Graphics;
		cResourceManager* ResourceManager;
		cSounds* Sounds;
		cPhysics2D* Physics2D;
		cGameObjectServitor* GameObjectServitor;
		cStateMachine* StateMachine;
		cInputSystem* InputSystem;
		cDebugSystem* DebugSystem;
		
		bool quit;
		bool debug;
		int winX, winY, winWidth, winHeight;
		int i, j, k, l; // indexes for cycles
		
		// ECS implemented through the work with registry
		// entt::registry registry; // inerited from cEntityComponentSystem
		
		std::vector<Objects::cGameObject*> objects; // various
		
		//std::vector<Objects::cDecorationObject*> decorations; // images
		//std::vector<Objects::cDoodadObject*> doodads; // potentially destructable images-based objects
		//std::vector<Objects::cPropsObject*> props; // interactable objects
		//std::vector<Objects::cUnitObject*> units; // playable and non-playable characters
		//std::vector<Objects::cTextObject*> texts; // messages to display
		// std::vector<std::vector<cGameObject*>> objectsLayer; // layers organize appearance on screen
		
		// these were private memmbers once
		int FPSCounter, time1, time2, timeBetweenFrames, timePassedOneSecondMax, timePassed;
		std::string windowTitle; // that which was sent to the engine initializer
		std::stringstream tmpTitle;
		std::stringstream logMessage;
		std::string strTemp;
		
		// --- SDLCSE customizable functions ---
		virtual void IntroState(){};
		virtual void MenuState(){};
		virtual void GameState(){};
		virtual void ScoresState(){};
		virtual void CreditsState(){};
		
		// --- SDLCSE game world parameters ---
		float scaleX, scaleY, scaleZ; // scaling coefficients to calculate what's inside one pixel
		
		// --- SDLCSE utilitary stuff ---
		// std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf32conv;
		
		// --- SDLCSE methods ---
		// cCrystalSerpentEngine(std::string winTitle, int x, int y, int width, int height, int layersNum){ // constructor
		void Init(std::string winTitle, int x, int y, int width, int height, int layersNum){ // constructor
			LogSystem = new cLogSystem; 
			LogSystem->log("Log System initialized.");
			LogSystem->log("Init started.");
			winX = x;
			winY = y;
			winWidth = width;
			winHeight = height;
			windowTitle = winTitle;
			
			window = nullptr; // the window
			renderer = nullptr; // the window renderer 
			// font = nullptr;
			screenSurface = nullptr;
			// Rect = nullptr;
			surface = nullptr;
			texture = nullptr;
			
			time1 = 0;
			time2 = 0;
			timeBetweenFrames = 0;
			timePassedOneSecondMax = 0;
			timePassed = 0;
			FPSCounter = 0;

			quit = false;
			debug = false;
			
			int imgFlags = IMG_INIT_PNG + IMG_INIT_JPG;
			// int winFlags = SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL;
			int winFlags = SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL;
			int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
			// int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
			
			// begin SDL initialization
			if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
				// LogSystem->log("Error initializing SDL:" + SDL_GetError() + "\n");
				LogSystem->log("Error initializing SDL");
				// printf("Error initializing SDL: %s\n", SDL_GetError());
			} else {
				// Create the main window
				window = SDL_CreateWindow( winTitle.c_str(), x, y, width, height, winFlags );
				// Make sure creating the window succeeded
				if ( window == NULL) {
					printf("Error creating window: %s\n", SDL_GetError());
				} else {
					//Create renderer for window 
					renderer = SDL_CreateRenderer(cCrystalSerpentEngine::window, -1, renderFlags); 
					
					if(renderer == NULL){ 
						printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
					} else {
						//Initialize renderer color
						Graphics = new cGraphics(renderer, winX, winY, winWidth, winHeight, layersNum);
						LogSystem->log("Graphics System initialized.");
						FontManager = new cFontManager();
						LogSystem->log("Font System initialized.");
						ResourceManager = new cResourceManager();
						LogSystem->log("Resource Management System initialized.");
						Sounds = new cSounds();
						LogSystem->log("Sounds System initialized.");
						Physics2D = new cPhysics2D();
						LogSystem->log("2D Physics System initialized.");
						GameObjectServitor = new cGameObjectServitor();
						LogSystem->log("Game Objects Servitor initialized.");
						InputSystem = new cInputSystem();
						LogSystem->log("Input System initialized.");
						DebugSystem = new cDebugSystem(Graphics, 0.0f, 0.0f, 300.0f, 50.0f);
						LogSystem->log("Debug System initialized.");
						StateMachine = new cStateMachine(&registry);
						LogSystem->log("State Machine engine addon initialized.");
						
						// SDL_GL_CreateContext(window);
						
						// Initialize PNG loading
						if (!(IMG_Init(imgFlags) & imgFlags)){
							printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
						} else {
							//Initialize SDL_mixer 
							if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) { 
								printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() ); 
							}
							// Get the surface from the window
							screenSurface = SDL_GetWindowSurface( window );
							// Make sure getting the surface succeeded
							if ( screenSurface == NULL ) {
								printf("Error getting surface: %s\n", SDL_GetError());
							} else {
								// Surprising, but it's not enough to declare e.
								// Without this line the program will likely crash with c0000005.
								e = new SDL_Event;
								
								// pass pointers to important objects to CSE subsystems
								Graphics->screenSurface = screenSurface;
								Graphics->window = window;
								Graphics->FontManager = FontManager;
								Graphics->ResourceManager = ResourceManager;
								
								FontManager->renderer = renderer;
								
								ResourceManager->screenSurface = screenSurface;
								ResourceManager->renderer = renderer;
								ResourceManager->window = window;
								
								Physics2D->GameObjectServitor = GameObjectServitor;
								Physics2D->LogSystem = LogSystem;
								Physics2D->DebugSystem = DebugSystem;
								Physics2D->Graphics = Graphics;
								
								DebugSystem->debugText->place.w = (int)round(5*winWidth/6);
								DebugSystem->debugText->place.h = winHeight;
								DebugSystem->GameObjectServitor = GameObjectServitor;
								DebugSystem->debugText->display = debug;
								DebugSystem->debugText->Name = "Отладчик";
								objects.push_back(DebugSystem->debugText);
								
								// InputSystem->e = e;
								FPSCountStart();
								LogSystem->log("Internal systems pointers configured.");
								
								// initialize randomizer
								srand(SDL_GetTicks64());
							}
						}
					}
				}
			}
		};
		
		~cCrystalSerpentEngine(){ // destructor
			Close();
		}
		
		void Close(){ 
			LogSystem->log("Game Engine destructor called.");
			// destroy textures
			if(texture != nullptr){
				SDL_DestroyTexture(texture);
				texture = nullptr;
			}
			
			// destroy surfaces
			if (surface != nullptr){
				SDL_FreeSurface(surface);
				surface = nullptr;
			}
			if (screenSurface != nullptr){
				SDL_FreeSurface(screenSurface);
				screenSurface = nullptr;
			}
						
			// destroy renderer and the window
			if (renderer != nullptr){
				SDL_DestroyRenderer(renderer);
				renderer = nullptr;
			}
			if (window != nullptr){
				SDL_DestroyWindow(window);
				window = nullptr;
			}

			delete FontManager;
			delete Graphics;
			delete ResourceManager;
			delete Sounds;
			delete Physics2D;
			delete GameObjectServitor;
			delete InputSystem;
			delete DebugSystem;
			LogSystem->log("Log System powers down. Bye!");
			delete LogSystem;
			delete e;
			
			// quit systems
			IMG_Quit();
			Mix_Quit();
			SDL_Quit();
		}
		
		Objects::cPropsObject* newPropsObject(std::string name,
									 const SDL_FRect place, 
									 cTexture* sprite, 
									 const SDL_FPoint centerOfMass, 
									 const SDL_FPoint velocity, 
									 const SDL_FPoint acceleration,
									 bool destructible=false, 
									 bool interactive=false,
									 int layer=0, 
									 float mass=1.0f, 
									 int hitbox=Objects::HIT_BOX_CIRCLE){
			Objects::cPropsObject* newProps = new Objects::cPropsObject();
			newProps->Name = name;
			newProps->type = Objects::GAME_OBJECT_TYPE_PROPS;
			newProps->layer = layer;
			newProps->hitbox = hitbox;
			newProps->display = true;
			newProps->mass = mass;
			newProps->canBounce = true;
			newProps->canPush = true;
			newProps->place.x = place.x;
			newProps->place.y = place.y;
			newProps->place.w = place.w;
			newProps->place.h = place.h;
			newProps->sprite = sprite;
			newProps->centerOfMass.x = centerOfMass.x;
			newProps->centerOfMass.y = centerOfMass.y;
			newProps->geometryCorrection.x = newProps->centerOfMass.x - newProps->place.x;
			newProps->geometryCorrection.y = newProps->centerOfMass.y - newProps->place.y;
			newProps->velocity[0] = velocity.x;
			newProps->velocity[1] = velocity.y;
			newProps->acceleration[0] = acceleration.x;
			newProps->acceleration[1] = acceleration.y;
			newProps->destructible = destructible;
			newProps->interactive = interactive;
			Physics2D->timeReset(newProps);
			
			objects.push_back(newProps);
			// logMessage.str(std::string());
			// logMessage << name << ": {" << place.x << ", " << place.y << ", " << (place.x+place.w) << ", " << (place.y+place.h) << "} in {" << centerOfMass.x << ", " << centerOfMass.y << "} ";
			// logMessage << " created and stored successfully.";
			// LogSystem->log(logMessage.str());
			return newProps;
		}
		
		virtual CSE_Entity newProps(std::string name){
			CSE_Entity newEntity = addEntity(name);
			registry.emplace<Components::hitBox>(newEntity);
			
			registry.emplace<Components::physics2D>(newEntity);
			Physics2D->timeReset(newEntity, &registry);
			
			registry.emplace<Components::sprite>(newEntity);
			return newEntity;
		}

		virtual void onKeyboardUpdate(int32_t keyCode, uint32_t keyStatus){};
		
		virtual void onMouseUpdate(MouseController* Mouse){};
		
		virtual void onWorldUpdate(){};
		
		virtual void DebugDefault(){
			DebugSystem->Debug(GameEngineName, "\nVersion: ", GameEngineVersion, "\n");
			DebugSystem->Debug("\n_____[ О т л а д к а ]_____\n");
			DebugSystem->Debug("Курсор: {", (InputSystem->Mouse.cursor.x), ", ", (InputSystem->Mouse.cursor.y), "}\n");
			DebugSystem->Debug("Всего объектов: ", objects.size(), "\n");
			for (int v=0; v<objects.size(); v++){
				DebugSystem->Debug("#", v, " - ", objects[v]->Name, "\n");
			}
			
			auto view = registry.view<Components::base>();
			DebugSystem->Debug("Всего сущностей: ", view.size(), "\n");
			int v = 0;
			for (auto entity : view){
				v++;
				auto* base = &(view.get<Components::base>(entity));
				if (registry.all_of<Components::physics2D>(entity)){
					auto* physics = &(registry.get<Components::physics2D>(entity));
					// DebugSystem->Debug("#", v, " - ", base->name, " - M: ", physics->mass, "; V:{", physics->velocity.x, ", ", physics->velocity.y, "}", "\n");
					DebugSystem->Debug("#", v, " - ", base->name, 
									   " - M: ", physics->mass, 
									   "; XY:{", physics->centerOfMass.x, ", ", physics->centerOfMass.y, "}",
									   "; V:{", physics->velocity.x, ", ", physics->velocity.y, "}", "\n");
				} else {
					DebugSystem->Debug("#", v, " - ", base->name, "\n");
				}
			}
		};
		
		virtual void processUserInput(){ // common user input processing
			if (timeBetweenFrames == 0) LogSystem->log("Event listening start...");
			while(SDL_PollEvent(e)){
				if (e->type == SDL_QUIT){
					quit = true;
					break;
				}
				if (e->type == SDL_KEYDOWN){
					if (e->key.keysym.sym == SDLK_F2){
						debug = !debug;
						DebugSystem->debugText->display = debug;
					}
					onKeyboardUpdate(e->key.keysym.sym, SDL_KEYDOWN);
					break;
				}
				if (e->type == SDL_KEYUP){
					onKeyboardUpdate(e->key.keysym.sym, SDL_KEYUP);
					break;
				}
				if (e->type == SDL_MOUSEMOTION){
					InputSystem->mouseListener(e, MOUSE_EVENT_MOVE);
					onMouseUpdate(&(InputSystem->Mouse));
					break;
				}
				if (e->type == SDL_MOUSEBUTTONDOWN){
					InputSystem->mouseListener(e, MOUSE_EVENT_BUTTON_DOWN);
					if (e->button.button == SDL_BUTTON_LEFT)
						InputSystem->Mouse.leftMouseButtonHoldDown = true;
					if (e->button.button == SDL_BUTTON_RIGHT)
						InputSystem->Mouse.rightMouseButtonHoldDown = true;
					onMouseUpdate(&(InputSystem->Mouse));
					break;
				}
				if (e->type == SDL_MOUSEBUTTONUP){
					InputSystem->mouseListener(e, MOUSE_EVENT_BUTTON_UP);
					if (e->button.button == SDL_BUTTON_LEFT)
						InputSystem->Mouse.leftMouseButtonHoldDown = false;
					if (e->button.button == SDL_BUTTON_RIGHT)
						InputSystem->Mouse.rightMouseButtonHoldDown = false;
					onMouseUpdate(&(InputSystem->Mouse));
					break;
				}
			}
			if (timeBetweenFrames == 0) LogSystem->log("Event listening end...");
		}
		
		virtual void clearFrame(){
			Graphics->clearScreen();
		}
		
		virtual void FPSCountStart(){
			tmpTitle.str("");
			time1 = SDL_GetTicks64(); // FPS count begin
		}
		
		virtual void FPSCount(){
			time2 = SDL_GetTicks64();
			timeBetweenFrames = time2 - time1;
			FPSCounter++;
			timePassed += timeBetweenFrames;
			timePassedOneSecondMax += timeBetweenFrames;
			
			// 1000 of timePassedOneSecondMax equals 1 second of real time 
			if (timePassedOneSecondMax > 1000){
				tmpTitle <<  windowTitle << " FPS: " << FPSCounter << "; миллисекунд между кадрами: " << timeBetweenFrames;
				// tmpTitle << "; " << (Graphics->renderer) << "_" << renderer;
				FPSCounter = 0;
				timePassedOneSecondMax = 0;
				strTemp = tmpTitle.str();
				SDL_SetWindowTitle(window, strTemp.c_str());
				tmpTitle.str("");
			}
			time2 = SDL_GetTicks64();
			time1 = time2;
		}
		
		void delay(int ms){ // delay wrapper (in ms)
			SDL_Delay(ms);
		}
		
		// what the framework does when it has been successfully set up
		virtual void Start(){
			LogSystem->log("Game Cycle preparation started.");
			std::stringstream sbuf;
			std::u32string s32buf;
			quit = false;
			Physics2D->timeReset();
			Physics2D->timeToReset = false;
			for (i = 0; i < objects.size(); i++){
				Physics2D->timeReset(objects[i]);
			}
			
			auto view = registry.view<Components::physics2D>();
			for (auto entity : view){
				Physics2D->timeReset(entity, &registry);
			}
			
			FPSCountStart();
			LogSystem->log("Game Cycle prepared.");
			while (!quit){
				// delay(20);
				// ------------------------
				// 0. Start collecting debug information
				if (timeBetweenFrames == 0){LogSystem->log("Debug info collection started...");}
				if (debug){
					DebugSystem->Reset();
					DebugDefault();
				}
				// ------------------------
				// 1. Input management system
				if (timeBetweenFrames == 0){LogSystem->log("Game cycle started.");}
				/*InputSystem->mouseListener();
				if (timeBetweenFrames == 0){
					LogSystem->log("Mouse controller data updated...");
				}*/
				processUserInput();
				if (timeBetweenFrames == 0){LogSystem->log("User Input processed...");}
				// ------------------------
				// Somewhere here a game logic container should be.
				// ------------------------
				// 2. Events system
				// ------------------------
				// 3. App Reaction system
				// ------------------------
				// 4. Game Object Management System (GOMS)
				if (timeBetweenFrames == 0){LogSystem->log("Game Object Management should've been processed here...");}
				
				// ------------------------
				// 5. World progress system
				// 		- physics and other world rules subsystems management
				// control time flow and then update world
				onWorldUpdate();
				if (timeBetweenFrames == 0){LogSystem->log("World Update processed...");}
				// ------------------------
				// 6. Resource management system
				if (timeBetweenFrames == 0){LogSystem->log("Resources management should've been processed here...");}
				// ------------------------
				// 7. Sound system
				if (timeBetweenFrames == 0){LogSystem->log("Sounds should've been processed here...");}
				// ------------------------
				// 8. Show debug information as text over the screen
				if (timeBetweenFrames == 0){LogSystem->log("Debug interface processed...");}
				if (debug){
					DebugSystem->Display();
				}
				// ------------------------
				// 9. Graphic system
				// Graphics->LayersClear();
				Graphics->clearScreen();
				
				auto view = registry.view<Components::sprite>();
				for (auto entity : view){
					auto* sprite = &(registry.get<Components::sprite>(entity));
					if (sprite->display){
						if (registry.all_of<Components::animation>(entity)){
							auto* animation = &(registry.get<Components::animation>(entity));
							auto* frameset = animation->frames[animation->currentAnimation];
							
							// calculate id of the current frame
							if (!(animation->paused)){
								int timeTemp = SDL_GetTicks64();
								if ((timeTemp - animation->timeBefore) >= frameset->timeBetweenFrames){
									animation->timeBefore = timeTemp;
									animation->currentFrame++;
									if (animation->currentFrame == animation->framesTotal){
										if (frameset->loop){
											animation->currentFrame = 0;
										} else {
											animation->currentFrame--;
											animation->paused = true;
										}
									}
								}
							} 
							
							// form the frame window
							SDL_Rect frame = {
								frameset->begin.x + (frameset->width * animation->currentFrame),
								frameset->begin.y,
								std::abs(frameset->width),
								std::abs(frameset->height)
							};
							
							// draw sprite
							Graphics->placeTextureF(sprite->sprite->texture, &(sprite->place), &frame);
						} else {
							Graphics->placeTextureF(sprite->sprite->texture, &(sprite->place), &(sprite->window));
						}
					}
				}
				
				for (i = 0; i < objects.size(); i++){
					if (objects[i]->display){
						Graphics->placeTextureF(objects[i]->sprite->texture, &(objects[i]->place));
					}
				}
				
				Graphics->updateScreen();
				if (timeBetweenFrames == 0){LogSystem->log("Graphics processed...");}
				// ------------------------
				// 10. File I/O system
				if (timeBetweenFrames == 0){LogSystem->log("Special File I/O operations should've been processed here...");}
				// ------------------------
				// 11. Log system
				if (timeBetweenFrames == 0){LogSystem->log("Special Logging should've been processed here...");}
				// ------------------------
				// 12. FPS Count
				FPSCount();
				if (timeBetweenFrames == 0){LogSystem->log("FPS Count processed...");}
			}
			LogSystem->log("Game Cycle closed.");
		}
		
		// FINITE STATE MACHINE
		// TODO: finite state machine - MAKE IT!
		virtual int getState(CSE_Entity entity){
			auto* base = &(registry.get<Components::base>(entity));
			return base->state;
		}
		
		virtual void setState(CSE_Entity entity, int state){
			auto* base = &(registry.get<Components::base>(entity));
			base->state = state;
		}
		
		virtual void updateState(CSE_Entity entity, int state){};
		
		// cycle through entities and figure out what to do with them 
		virtual void stateMachine(CSE_Entity entity){
			auto view = registry.view<Components::base>();
			
			for (auto entity : view){
				int state = getState(entity);
				switch (state){
					case Objects::GAME_OBJECT_STATE_REMOVE:
						removeEntity(entity);
						break;
					default:
						updateState(entity, state);
						break;
				}
			}
		}
		
		
	};
}

#endif
