#ifndef CFONTMANAGER_H
#define CFONTMANAGER_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	/* -------------------
	  This is Font Manager.
	  It works with raster format only.
	  ------------------- */
	class cFontManager {
	public:
		SDL_Renderer* renderer;
		std::u32string alphabetEn;
		std::u32string alphabetRu;
		std::vector<std::u32string> alphabet; 
		SDL_Rect destRect8;
		SDL_Rect fontClip8;
		SDL_Rect fontClip16;
		SDL_Rect fontClip32;
		SDL_Rect fontClip64;
		cTexture* fontTexture;
		
		cFontManager(){
			alphabetEn = U"`1234567890-=qwertyuiop[]\\asdfghjkl;'zxcvbnm,./~!@#$%^&*()_+QWERTYUIOP{}|ASDFGHJKL:\"ZXCVBNM<>? \n"; //95
			alphabetRu = U"ё1234567890-=йцукенгшщзхъ\\фывапролджэячсмитьбю.Ё!\"№;%:?*()_+ЙЦУКЕНГШЩЗХЪ/ФЫВАПРОЛДЖЭЯЧСМИТЬБЮ, \n"; //95
			alphabet.push_back(alphabetEn);
			alphabet.push_back(alphabetRu);
			
			//
			destRect8 = {0, 0, 8, 8};
			
			// set the clip size and coordinates
			fontClip8.x = 0; 
			fontClip8.y = 0;
			fontClip8.w = 8; 
			fontClip8.h = 8;
			
			fontClip16.x = 0; 
			fontClip16.y = 0;
			fontClip16.w = 16; 
			fontClip16.h = 16;
			
			fontClip32.x = 0; 
			fontClip32.y = 0;
			fontClip32.w = 32; 
			fontClip32.h = 32;
			
			fontClip64.x = 0; 
			fontClip64.y = 0;
			fontClip64.w = 64; 
			fontClip64.h = 64;
			
			fontTexture = NULL;
		};
		
		~cFontManager(){
		};
		
		void load(std::string path){
			if (fontTexture != NULL){
				fontTexture->load(path);
			} else {
				fontTexture = new cTexture(path, renderer);
			}
		};
		
		void getClip8(int id=0, int row=0){
			if ((fontTexture != NULL)){
				fontClip8.x = id*fontClip8.w;
				fontClip8.y = row*fontClip8.h;
			}
		}
		
		void getClip16(int id=0, int row=0){
			if ((fontTexture != NULL)){
				fontClip16.x = id*fontClip16.w;
				fontClip16.y = row*fontClip16.h;
			}
		}
		
		void getClip32(int id=0, int row=0){
			if ((fontTexture != NULL)){
				fontClip32.x = id*fontClip32.w;
				fontClip32.y = row*fontClip32.h;
			}
		}
		
		void getClip64(int id=0, int row=0){
			if ((fontTexture != NULL)){
				fontClip64.x = id*fontClip64.w;
				fontClip64.y = row*fontClip64.h;
			}
		}
		
		void free(){
			fontTexture->free();
		}
		
	private:
		static cFontManager* instance;
	};
}

#endif
