#ifndef CSURFACE_H
#define CSURFACE_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	class cSurface {
	public:
		int width;
		int height;
		SDL_Surface* source;
		
		cSurface(std::string path) { // construct from a file
			source = load(path);
			width = source->w;
			height = source->h;
		}
		
		cSurface(int w, 	 	 int h, 	 	 Uint32 flags=0, int depth=32,
				 Uint32 Rmask=0, Uint32 Gmask=0, Uint32 Bmask=0, Uint32 Amask=0){ // construct blank
			source = SDL_CreateRGBSurface(flags, w, h, depth, Rmask, Gmask, Bmask, Amask);
			// source = SDL_CreateRGBSurfaceWithFormat(0, w, h, depth, SDL_PIXELFORMAT_RGBA8888);
			width = source->w;
			height = source->h;
		}
		
		~cSurface() {
			free();
		}
		
		SDL_Surface* load(std::string path){
			SDL_Surface* loadedSurface = IMG_Load(path.c_str()); //Load image at specified path
			
			if(loadedSurface == NULL){
				printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			} 
			return loadedSurface;
		}
		
		SDL_Texture* convertToTexture(SDL_Surface* srcSurface, SDL_Renderer* renderer){
			SDL_Texture* newTexture = NULL; //The final texture
			newTexture = SDL_CreateTextureFromSurface(renderer, srcSurface);
			free();
			return newTexture;
		}
		
		int getWidth(){
			if (source != NULL){
				return source->w;
			}
		}
		
		int getHeight(){
			if (source != NULL){
				return source->h;
			}
		}
		
		uint32_t* getPixels32(){ // shamelessly took from Lazy Foo's tutorials
			uint32_t* pixels = NULL; 
			if (source != NULL){ 
				pixels = static_cast<uint32_t*>(source->pixels);
			}
			
			return pixels;
		}
		
		int getPitch32(){ // shamelessly took from Lazy Foo's tutorials
			int pitch = 0; 
			if (source != NULL){ 
				pitch = source->pitch/4;
			}
			
			return pitch;
		}
		
		void free() {
			if(source != NULL){
				SDL_FreeSurface(source);
				source = NULL;
			}
		}
	};
}

#endif
