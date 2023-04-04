#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	/* ------------------
	  This is Resource Management
	  ------------------ */
	class cResourceManager {
	public:
		SDL_Surface* screenSurface; // the window surface
		SDL_Renderer* renderer;
		SDL_Window* window;
		std::vector<SDL_Surface*> surfaces;
		// std::vector<SDL_Texture*> textures;
		std::vector<cTexture*> textures;
		
		cResourceManager(){};
		~cResourceManager(){
			for(int i=0; i<surfaces.size(); i++){
				SDL_FreeSurface(surfaces[i]);
				surfaces[i] = NULL;
			}
			for(int i=0; i<textures.size(); i++){
				// SDL_DestroyTexture(textures[i].source);
				// textures[i] = NULL;
				textures[i]->free();
			}
		};
		
		SDL_Surface* loadSurface(std::string path){
			SDL_Surface* loadedSurface = NULL; // loaded surface
			SDL_Surface* optimizedSurface = NULL; // the final optimized surface
			
			loadedSurface = IMG_Load(path.c_str());
			if (loadedSurface == NULL){
				printf("Unable to load an image %s! SDL Image Error: %s\n", path.c_str(), SDL_GetError());
			} else {
				//Convert surface to screen format 
				optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0); 
				if(optimizedSurface == NULL){
					printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
				}
				SDL_FreeSurface(loadedSurface); //Get rid of old loaded surface 
			}
			return optimizedSurface;
		}
		
		SDL_Texture* loadTexture(std::string path){ 
			SDL_Texture* newTexture = NULL; //The final texture
			SDL_Surface* loadedSurface = IMG_Load(path.c_str()); //Load image at specified path  
			
			if(loadedSurface == NULL){
				printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			} else {
				//Create texture from surface pixels 
				newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
				if(newTexture == NULL){
					printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
				} 
				//Get rid of old loaded surface 
				SDL_FreeSurface(loadedSurface);
			} 
			return newTexture;
		}
	};
}
#endif
