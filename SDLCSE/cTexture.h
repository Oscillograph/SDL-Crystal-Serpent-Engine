#ifndef CTEXTURE_H
#define CTEXTURE_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	class cTexture {
	public:
		SDL_Surface* surface;
		SDL_Texture* texture;
		
		cTexture(std::string path, SDL_Renderer* r){
			texture = NULL; //The final texture
			surface = NULL;
			renderer = r;
			
			load(path);
		}
		
		cTexture(SDL_Renderer* renderer, int w, int h, uint32_t flags=0, int depth=32,
			uint32_t Rmask=0, uint32_t Gmask=0, uint32_t Bmask=0, uint32_t Amask=0){
			surface = SDL_CreateRGBSurface(flags, w, h, depth, Rmask, Gmask, Bmask, Amask);
			texture = SDL_CreateTexture(renderer,
										SDL_PIXELFORMAT_RGBA8888,
										SDL_TEXTUREACCESS_STREAMING,
										// SDL_TEXTUREACCESS_TARGET, 
										w,
										h);
		}
		
		~cTexture(){
			free();
		}
		
		void load(std::string path){
			freeSurface();
			surface = IMG_Load(path.c_str()); //Load image at specified path  
			
			if(surface == NULL){
				printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
			} else {
				freeTexture();
				
				//Create texture from surface pixels
				texture = SDL_CreateTextureFromSurface(renderer, surface);
				
				if(texture == NULL){
					printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
				} 
			} 
		}
		
		SDL_Texture* convertSurface(SDL_Surface* srcSurface){
			free();
			surface = srcSurface;
			texture = SDL_CreateTextureFromSurface(renderer, srcSurface);
			return texture;
		}
		
		void update(){
			if (surface != NULL){
				if (texture != NULL){
					SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
				}
			}
		}
		
		int getWidth(){
			if (surface != NULL){
				return surface->w;
			}
		}
		
		int getHeight(){
			if (surface != NULL){
				return surface->h;
			}
		}
		
		uint32_t* getPixels32(){ // shamelessly took from Lazy Foo's tutorials 
			if (surface != NULL){ 
				return static_cast<uint32_t*>(surface->pixels);
			}
		}
		
		int getPitch32(){ // shamelessly took from Lazy Foo's tutorials
			if (surface != NULL){ 
				return surface->pitch/4;
			}
		}
		
		void freeTexture(){
			if (texture != NULL){
				SDL_DestroyTexture(texture);
				texture = NULL;
			}
		}
		
		void freeSurface(){
			if (surface != NULL){
				SDL_FreeSurface(surface);
				surface = NULL;
			}
		}
		
		void free(){
			freeTexture();
			freeSurface();
		}
		
		void clear(SDL_Renderer* renderer){
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
		}
		
	private:
		SDL_Renderer* renderer;
	};
}

#endif
