// ==============================================================
// Crystal Serpent Engine v0.1
// Made with SDL, TDM-GCC 10.3.0 and love <3
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
#include "SDLCSE/cFontManager.h"
#include "SDLCSE/cGameObject.h"
#include "SDLCSE/cResourceManager.h"
#include "SDLCSE/cGraphics.h"
#include "SDLCSE/cHighScoresFile.h"
#include "SDLCSE/cPhysics2D.h"


namespace SDLCSE {	
	/* ------------------
	This is an audio management class
	------------------ */
	class cSounds {
	public:
		uint32_t* soundBuffer;
		uint32_t NiquistFrequency;
		uint32_t chunkSize;
		uint32_t* leftChannel;
		uint32_t* rightChannel;
		
		int i, j, k, l; // indexes for cycles
		
		cResourceManager* ResourceManager;
		
		cSounds() {}
		~cSounds() {}
		
		void loadSound() {}
		void playSound() {}
		void saveSound() {}
		void synthesizer() {}
	};
	
	/* ------------------
	This class essentially constructs a cosy framework from SDL functions
	------------------ */
	class cCrystalSerpentEngine {
	public:
		// --- SDL vars ---
		SDL_Event* e; // event queue
		SDL_Window* window; // the window
		SDL_Renderer* renderer; // the window renderer 
		SDL_Surface* screenSurface; // the window surface
		SDL_Rect Rect;		  // temp region
		SDL_Surface* surface; // temp surface
		SDL_Texture* texture; // temp texture
		
		// --- SDLCSE vars ---
		// static cCrystalSerpentEngine* instance; // this instance
		cFontManager* FontManager;
		cGraphics* Graphics;
		cResourceManager* ResourceManager;
		cSounds* Sounds;
		cPhysics2D* Physics2D;
		
		bool quit;
		int winX, winY, winWidth, winHeight;
		int i, j, k, l; // indexes for cycles
		std::vector<cGameObject*> objects;
		
		// --- SDLCSE customizable functions ---
		void (*IntroState)();
		void (*MenuState)();
		void (*GameState[6])(); // a pointer to a user-defined function resembling the gameplay
		void (*ScoresState)();
		void (*CreditsState)();
		
		// --- SDLCSE methods ---
		cCrystalSerpentEngine(std::string winTitle, int x, int y, int width, int height, int layersNum){ // constructor
			winX = x;
			winY = y;
			winWidth = width;
			winHeight = height;
			
			window = NULL; // the window
			renderer = NULL; // the window renderer 
			// font = NULL;
			screenSurface = NULL;
			// Rect = NULL;
			surface = NULL;
			texture = NULL;
			
			time1 = 0;
			time2 = 0;
			delta = 0;
			gamma = 0;
			FPSCounter = 0;
			
			int imgFlags = IMG_INIT_PNG + IMG_INIT_JPG;
			int winFlags = SDL_WINDOW_RESIZABLE + SDL_WINDOW_SHOWN + SDL_WINDOW_OPENGL;
			int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
			// int renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
			quit = false;
			
			// begin SDL initialization
			if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
				printf("Error initializing SDL: %s\n", SDL_GetError());
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
						FontManager = new cFontManager;
						ResourceManager = new cResourceManager;
						Sounds = new cSounds;
						Physics2D = new cPhysics2D;
						
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
								Graphics->screenSurface = screenSurface;
								Graphics->window = window;
								Graphics->FontManager = FontManager;
								Graphics->ResourceManager = ResourceManager;
								FontManager->renderer = renderer;
								ResourceManager->screenSurface = screenSurface;
								ResourceManager->renderer = renderer;
								ResourceManager->window = window;
								FPSCountStart();
								
								// initialize randomizer
								srand(SDL_GetTicks64());
							}
						}
					}
				}
			}
		};
			
		~cCrystalSerpentEngine(){ // destructor
			// destroy textures
			if(texture != NULL){
				SDL_DestroyTexture(texture);
				texture = NULL;
			}
			
			// destroy surfaces
			if (surface != NULL){
				SDL_FreeSurface(surface);
				surface = NULL;
			}
			if (screenSurface != NULL){
				SDL_FreeSurface(screenSurface);
				screenSurface = NULL;
			}
						
			// destroy renderer and the window
			if (renderer != NULL){
				SDL_DestroyRenderer(renderer);
				renderer = NULL;
			}
			if (window != NULL){
				SDL_DestroyWindow(window);
				window = NULL;
			}

			delete FontManager;
			delete Graphics;
			delete ResourceManager;
			delete Sounds;
				
			// quit systems
			IMG_Quit();
			Mix_Quit();
			SDL_Quit();
		}
			
		void processUserInput(){ // common user input processing
			while(SDL_PollEvent(e)){
				if (e->type == SDL_QUIT){
					quit = true;
				} 
				if (e->type == SDL_KEYDOWN){
					switch(e->key.keysym.sym){
						case SDLK_SPACE:
							quit = true;
							break;
						default:
							break;
					}
				}
			}
		}
		
		/*void buildFrame(){
			clearFrame();
			// do something to build on top of this
			
			// draw inner rectangle
			Graphics->RenderBorder(Graphics->textureLayers[0], 50, 0, winWidth-100, winHeight-50, 2);
			
			// ---------------------
			Graphics->demoDrawPixels();
			// ---------------------
			
			Graphics->updateScreen();
			
			FPSCount();
		}*/
		
		void clearFrame(){
			Graphics->clearScreen();
		}
		
		void FPSCountStart(){
			tmpTitle.str("");
			time1 = SDL_GetTicks64(); // FPS count begin
		}
		
		void FPSCount(){
			time2 = SDL_GetTicks64();
			delta = time2 - time1;
			FPSCounter++;
			gamma += delta;
			if (gamma > 1000){
				tmpTitle << "Раз, два, три! Test window!! FPS: " << FPSCounter << "; delta: " << delta;
				// tmpTitle << "; " << (Graphics->renderer) << "_" << renderer;
				FPSCounter = 0;
				gamma = 0;
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
		void Start(){
			quit = false;
			Physics2D->timeReset();
			Physics2D->timeToReset = false;
			for (i = 0; i < objects.size(); i++){
				Physics2D->timeReset(objects[i]);
			}
			FPSCountStart();
			
			while (!quit){
				// delay(20);
				// ------------------------
				// 0. Input management system
				processUserInput();
				
				// ------------------------
				// Somewhere here a game logic container should be.
				// 1. Events system
				// 2. Reaction system
				// 3. Game Object Management System (GOMS)
				
				// 4. World progress system
				// 		- physics and other world rules subsystems management
				// 
				// - Use function pointers, maps and command pattern, Luke!
				for (i = 0; i < objects.size(); i++){
					Physics2D->getTime(objects[i]);
					Physics2D->move(objects[i]);
					if (Physics2D->outOfBounds({0,0,winWidth,winHeight}, objects[i])){
						Physics2D->unMove(objects[i]);
						objects[i]->velocity[0] = (rand() % 20) - 10;
						objects[i]->velocity[1] = (rand() % 20) - 10;
						Physics2D->timeReset(objects[i]);
					}
				}
				
				// ------------------------
				// 5. Graphic system
				// Graphics->LayersClear();
				Graphics->clearScreen();
				
				for (i = 0; i < objects.size(); i++){
					Graphics->placeTexture(objects[i]->sprite->texture, &(objects[i]->place));
				}
				
				Graphics->updateScreen();
				
				// ------------------------
				// 6. Sound system
				
				// ------------------------
				// 7. Show debug informatuion as text over the screen
				
				// ------------------------
				// 8. Resource management system
				
				// ------------------------
				// 9. File I/O system
				
				// ------------------------
				// 10. Log system
				
				FPSCount();
			}
		}
	private:
		int FPSCounter, time1, time2, delta, gamma;
		std::stringstream tmpTitle;
		std::string strTemp;
	};
}

#endif
