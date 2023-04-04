#ifndef CGRAPHICS_H
#define CGRAPHICS_H

// SDLCSE
#include "./common.h"

#include "./cGraphics_DirectSurfaceDraw.h"

namespace SDLCSE {
	/* ------------------
	  This is Graphics
	  ------------------ */
	class cGraphics : public cGraphics_DirectSurfaceDraw {
	public:
		Colors::RGBA Palette[8]; // palette for 8 colors in RGBA format
		SDL_Color rgb; // some SDL functions don't get SDLCSE::Color::RGBA type for input
		SDL_Renderer* renderer; // the window renderer
		SDL_Surface* screenSurface; // the window surface
		SDL_Window* window; // the window pointer
		
		// pointers to other engine components
		cFontManager* FontManager;
		cTexture* fontTarget; // where the fonts should be drawn on
		cSurface* theSymbol;
		cResourceManager* ResourceManager;
		
		int winX, winY, winWidth, winHeight;
		int i, j, k, l; // indexes for cycles; usually: k for x, l for y
		SDL_Rect windowRect;
		SDL_Rect tempRect; // just a rect for various functions
		
		// vectors for raster font matrices
		std::vector<Uint32> symbols8bit; // a single font line
		std::vector<std::vector<Uint32>> symbols8bitTable; // a whole table from the font file
		
		// memory buffers for texture rendering
		std::vector<cTexture*> textureLayers;
		
		cGraphics(SDL_Renderer* r, int x, int y, int w, int h, int layersNum){ // x,y,w,h are in pixels
			rgb = {0, 0, 0, 0};
			renderer = r;
			winX = x;
			winY = y;
			winWidth = w;
			winHeight = h;
			
			windowRect = {x, y, w, h};
			
			// initialize memory buffers
			// create a texture for the whole screen
			
			// initialize render color
			// setPalette8(0);
			SDL_SetRenderDrawColor(	renderer, 
				Colors::RGBA_Black0.r,
				Colors::RGBA_Black0.g, 
				Colors::RGBA_Black0.b, 
				Colors::RGBA_Black0.a);
			
			// Create three layers: 
			// 0 - Background
			// 1 - Scene
			// 2 - Foreground

			// The last additional layer is for blank memory to be copied during frame clear stage.
			LayersCreate(layersNum + 1, winWidth, winHeight);
			if (layersNum > 0){
				fontTarget = textureLayers[layersNum-1];
			} else {
				fontTarget = textureLayers[0];
			}
			
		};
		
		~cGraphics(){
			screenSurface = NULL;
			renderer = NULL;
			
			for (i = textureLayers.size(); i > 0 ; i--){
				delete textureLayers[i-1];
				textureLayers.pop_back();
			}
		};
		
		bool setPalette8(int variant){
			bool success = true;
			int colorTone;
			switch (variant) {
				case 0: colorTone = Colors::RGBA_Tone0; break;
				case 1: colorTone = Colors::RGBA_Tone1; break;
				case 2: colorTone = Colors::RGBA_Tone2;	break;
				case 3: colorTone = Colors::RGBA_Tone3;	break;
				case 4: colorTone = Colors::RGBA_Tone4;	break;
				case 5: colorTone = Colors::RGBA_Tone5;	break;
				case 6: colorTone = Colors::RGBA_Tone6;	break;
				case 7: colorTone = Colors::RGBA_Tone7;	break;
				case 8: colorTone = Colors::RGBA_Tone8;	break;
			}
			
			Palette[0].r = 0x00; Palette[0].g = 0x00; Palette[0].b = 0x00; Palette[0].a = 0xFF; // Black
			Palette[1].r = colorTone; Palette[1].g = 0x00; Palette[1].b = 0x00; Palette[1].a = 0xFF; // Red
			Palette[2].r = 0x00; Palette[2].g = colorTone; Palette[2].b = 0x00; Palette[2].a = 0xFF; // Green
			Palette[3].r = 0x00; Palette[3].g = 0x00; Palette[3].b = colorTone; Palette[3].a = 0xFF; // Blue
			Palette[4].r = colorTone; Palette[4].g = colorTone; Palette[4].b = 0x00; Palette[4].a = 0xFF; // Yellow
			Palette[5].r = colorTone; Palette[5].g = 0x00; Palette[5].b = colorTone; Palette[5].a = 0xFF; // Purple
			Palette[6].r = 0x00; Palette[6].g = colorTone; Palette[6].b = colorTone; Palette[6].a = 0xFF; // Cyan
			Palette[7].r = colorTone; Palette[7].g = colorTone; Palette[7].b = colorTone; Palette[7].a = 0xFF; // White
			
			return success;
		}
		
		void LayersCreate(int number, int w, int h, int bpp=32){
			cTexture* newLayer = NULL;
			int j, k, l = 0;
			
			for(int j = 0; j < number; j++){
				newLayer = new cTexture(renderer, w, h, 0, bpp, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
				SDL_SetTextureBlendMode(newLayer->texture, SDL_BLENDMODE_BLEND);
				textureLayers.push_back(newLayer);
				newLayer = NULL;
			}
		}
		
		void LayersClear(){
			for (i=0; i < textureLayers.size()-1; i++){
				// LayerClear(i);
				SDL_memcpy(textureLayers[i]->surface->pixels, 
						   textureLayers[textureLayers.size()-1]->surface->pixels, 
						   textureLayers[i]->surface->h * textureLayers[i]->surface->pitch);
				textureLayers[i]->update();
			}
			
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
		}
		
		void LayerClear(int layerId){
			SDL_memcpy(textureLayers[layerId]->surface->pixels, 
				textureLayers[textureLayers.size()-1]->surface->pixels, 
				textureLayers[layerId]->surface->h * textureLayers[layerId]->surface->pitch);
			textureLayers[layerId]->update();
			// textureLayers[layerId]->clear(renderer);
		}
		
		void RenderPixel(cTexture* target, int x, int y, int pxSize, uint32_t color){
			if (target->surface != NULL){
				for (int k = 0; k < pxSize; k++){
					xn = x + k;
					for (int l = 0; l < pxSize; l++){
						yn = y + l;
						if ((xn < target->surface->w) && (yn < target->surface->h)){
							pxBuffer = (uint32_t*)target->surface->pixels + yn*target->surface->pitch/4 + xn;
							*pxBuffer = color;
						}
					}
				}
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderPixelAuto(cTexture* target, int x, int y, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				for (int k = 0; k < pxSize; k++){
					xn = x + k;
					for (int l = 0; l < pxSize; l++){
						yn = y + l;
						if ((xn < target->surface->w) && (yn < target->surface->h)){
							pxBuffer = (uint32_t*)target->surface->pixels + yn*target->surface->pitch/4 + xn;
							*pxBuffer = color;
						}
					}
				}
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		void RenderLine(cTexture* target, int x1, int y1, int x2, int y2, int pxSize, uint32_t color){
			if (target->surface != NULL){
				// As we draw a line, we need to construct its equation
				// to determine coordinates between start and end points.
				// 					y = kx + c
				int k = y1/x1; 
				int c = y1 - k*x1; 
				
				for (xn = x1; xn < x2; xn++){
					yn = k*xn + c;
					RenderPixel(target, xn, yn, pxSize, color);
				}
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderLineAuto(cTexture* target, int x1, int y1, int x2, int y2, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				// As we draw a line, we need to construct its equation
				// to determine coordinates between start and end points.
				// 					y = kx + c
				int k = y1/x1; 
				int c = y1 - k*x1; 
				
				for (xn = x1; xn < x2; xn++){
					yn = k*xn + c;
					RenderPixel(target, xn, yn, pxSize, color);
				}
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		void RenderRect(cTexture* target, int x, int y, int w, int h, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if ((x + w) > target->surface->w){
					int pxWidth = (int)(round(target->surface->w - x));
				} 
				if ((y + h) > target->surface->h){
					int pxHeight = (int)(round(target->surface->h - y));
				}
				
				RenderLine(target, x, y, x+w, y, pxSize, color); // draw top line
				RenderLine(target, x+w, y, x+w, y+h, pxSize, color); // draw right line
				RenderLine(target, x+w, y+h, x, y+h, pxSize, color); // draw bottom line
				RenderLine(target, x, y+h, x, y, pxSize, color); // draw left line
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderRectAuto(cTexture* target, int x, int y, int w, int h, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				if ((x + w) > target->surface->w){
					int pxWidth = (int)(round(target->surface->w - x));
				} 
				if ((y + h) > target->surface->h){
					int pxHeight = (int)(round(target->surface->h - y));
				}
				
				RenderLine(target, x, y, x+w, y, pxSize, color); // draw top line
				RenderLine(target, x+w, y, x+w, y+h, pxSize, color); // draw right line
				RenderLine(target, x+w, y+h, x, y+h, pxSize, color); // draw bottom line
				RenderLine(target, x, y+h, x, y, pxSize, color); // draw left line
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		void RenderFilledRect(cTexture* target, int x, int y, int w, int h, int pxSize, uint32_t color){
			if (target->surface != NULL){
				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = w; 
				int pxHeight = h;
				if ((x + w) > target->surface->w){
					pxWidth = (int)(round(target->surface->w - x));
				} 
				if ((y + h) > target->surface->h){
					pxHeight = (int)(round(target->surface->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						RenderPixel(target, x+j, y+i, pxSize, color);
					}
				}
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderFilledRectAuto(cTexture* target, int x, int y, int w, int h, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = w; 
				int pxHeight = h;
				if ((x + w) > target->surface->w){
					pxWidth = (int)(round(target->surface->w - x));
				} 
				if ((y + h) > target->surface->h){
					pxHeight = (int)(round(target->surface->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						RenderPixel(target, x+j, y+i, pxSize, color);
					}
				}
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		void RenderCircle(cTexture* target, int x, int y, int r, int pxSize, uint32_t color){
			if (target->surface != NULL){
				double circlePerimeter = PI * r * 2; // find the perimeter of a circle
				int pxNumber = (int)round(circlePerimeter / pxSize); // count pixels in the perimeter
				
				double degreeStep = 2*PI / pxNumber; // find how much degrees in each pixel
				for (int j = 0; j < pxNumber; j++) { // draw the pixels
					xn = (int)(x + round(r*cos(j*degreeStep)));
					yn = (int)(y + round(r*sin(j*degreeStep)));
					xn = xn - (xn % pxSize);
					yn = yn - (yn % pxSize);
					
					if ((xn < target->surface->w) && (yn < target->surface->h) && (xn >= 0) && (yn >= 0)){
						RenderPixel(target, xn, yn, pxSize, color);
					}
				}
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderCircleAuto(cTexture* target, int x, int y, int r, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				double circlePerimeter = PI * r * 2; // find the perimeter of a circle
				int pxNumber = (int)round(circlePerimeter / pxSize); // count pixels in the perimeter
				
				double degreeStep = 2*PI / pxNumber; // find how much degrees in each pixel
				for (int j = 0; j < pxNumber; j++) { // draw the pixels
					xn = (int)(x + round(r*cos(j*degreeStep)));
					yn = (int)(y + round(r*sin(j*degreeStep)));
					xn = xn - (xn % pxSize);
					yn = yn - (yn % pxSize);
					
					if ((xn < target->surface->w) && (yn < target->surface->h) && (xn >= 0) && (yn >= 0)){
						RenderPixel(target, xn, yn, pxSize, color);
					}
				}
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		void RenderFilledCircle(cTexture* target, int x, int y, int r, int pxSize, uint32_t color){
			if (target->surface != NULL){
				x = x - (x % pxSize);
				y = y - (y % pxSize);
				int r2 = r*r;
				
				for (int k = -r; k <= r; k = k + pxSize){
					for (int l = -r; l <= r; l = l + pxSize){
						if (k*k + l*l <= r2){
							xn = x + k;
							yn = y + l;
							xn = xn - (xn % pxSize);
							yn = yn - (yn % pxSize);
							
							if ((xn < target->surface->w) && (yn < target->surface->h) && (xn >= 0) && (yn >= 0)){
								RenderPixel(target, xn, yn, pxSize, color);
							}
						}
					}
				}
			}
		}
		
		// Auto means this function locks/unlocks the surface and forces texture to update contents
		void RenderFilledCircleAuto(cTexture* target, int x, int y, int r, int pxSize, uint32_t color){
			if (target->surface != NULL){
				if (SDL_MUSTLOCK(target->surface)){
					SDL_LockSurface(target->surface);
				}
				
				x = x - (x % pxSize);
				y = y - (y % pxSize);
				int r2 = r*r;
				
				for (int k = -r; k <= r; k = k + pxSize){
					for (int l = -r; l <= r; l = l + pxSize){
						if (k*k + l*l <= r2){
							xn = x + k;
							yn = y + l;
							xn = xn - (xn % pxSize);
							yn = yn - (yn % pxSize);
							
							if ((xn < target->surface->w) && (yn < target->surface->h) && (xn >= 0) && (yn >= 0)){
								RenderPixel(target, xn, yn, pxSize, color);
							}
						}
					}
				}
				
				if (SDL_MUSTLOCK(target->surface)){
					SDL_UnlockSurface(target->surface);
				}
				
				target->update();
			}
		}
		
		// a very old function kept for backwards compatibility
		void RenderPixelFont(int x, int y, int pxSize, uint32_t color){
			RenderPixel(fontTarget, x, y, pxSize, color);
		}
		
		void RenderPixelFontToTexture(cTexture* textTexture, int x, int y, int pxSize, uint32_t color){
			RenderPixel(textTexture, x, y, pxSize, color);
		}
				
		Uint32 getPixel(SDL_Surface *surface, int x, int y){
			/*
			  Got this function from here:
			  https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
			 */
			/* - - - Usage example: - - -
			  SDL_Color rgb;
			  Uint32 data = getPixel(gSurface, 200, 200);
			  SDL_GetRGB(data, gSurface->format, &rgb.r, &rgb.g, &rgb.b);
			 - - - - - - - - - - - - - - - */
			int bpp = surface->format->BytesPerPixel;
			/* Here p is the address to the pixel we want to retrieve */
			Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
			
			switch (bpp){
			case 1:
				return *p;
				break;
			case 2:
				return *(Uint16 *)p;
				break;
			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN){
					return p[0] << 16 | p[1] << 8 | p[2];
				} else {
					return p[0] | p[1] << 8 | p[2] << 16;
				}
				break;
			case 4:
				return *(Uint32 *)p;
				break;
			default:
				return 0;       /* shouldn't happen, but avoids warnings */
			}
		}
		
		void getSymbols8bit(){
			for(i = 0; i < FontManager->alphabet.size(); i++){ // for each alphabet
				for(j = 0; j < FontManager->alphabet[i].size()-1; j++){ // and each font symbol
					symbols8bitTable.push_back(symbols8bit); // add a new vector of 64 int values;
					for(l = 0; l < 8; l++){ // and each y coordinate
						for(k = 0; k < 8; k++){ // for each x coordinate
							// get the pixel color
							SDL_GetRGB(getPixel(FontManager->fontTexture->surface, j*8 + k, i*8 + l), 
												FontManager->fontTexture->surface->format,
												&rgb.r, 
												&rgb.g, 
												&rgb.b);
							// if it's black, add 1, else add 0
							if ((rgb.r + rgb.g + rgb.b) == 0){
								symbols8bitTable[i].push_back(1);
							} else {
								symbols8bitTable[i].push_back(0);
							}
						}
					}
				}
			}
			// from this moment we have symbol8bitTable[i][m], where m = 0..64*alphabet[i].size()
			// this way we get an equivalent of a set of raster surfaces
		}
		
		void RenderText(SDL_Rect* place,		// where to draw  
					  int pxSize, 			// size of the pen
					  Colors::RGBA color, 	// color
					  std::u32string text, 	// what to draw
					  int interval = 2){	// interval between lines
			// cols and rows with no respect to spaces trimming made via correctX variable
			int cols = place->w / (8*pxSize);
			int rows = place->h / ((8 + interval)*pxSize);
			int currentRow = 0;
			long long unsigned int symbolId = 0; // the type corresponds to std::u32string::npos
			int correctX = 0;
			int correctY = 0;
			int curX;
			int curY;
			int alphabetId = 0;
			bool foundSymbol = false;
			uint32_t fontColor = SDL_MapRGBA(textureLayers[textureLayers.size()-1]->surface->format,
											 color.r, color.g, color.b, color.a);
			
			// for each symbol in text
			j = 0; // index of a symbol inside a line being typed right now
			if (SDL_MUSTLOCK(fontTarget->surface)){
				SDL_LockSurface(fontTarget->surface);
			}
			for(i = 0; i < text.size(); i++){
				// load font symbol
				foundSymbol = false;
				while ((!foundSymbol) && (alphabetId < FontManager->alphabet.size())){
					symbolId = FontManager->alphabet[alphabetId].find(text[i]);
					// force a new line if '\n' found
					if (symbolId == 95){
						foundSymbol = true;
						currentRow++;
						j = 0; // reset the index inside a line
						correctX = 0;
						symbolId = 0;
						alphabetId = 0;
					} else {
						if (symbolId != std::u32string::npos){
							// Correct coordinates of the "cursor"
							if (j > cols){
								currentRow++;
								j = 0;
								correctX = 0;
							}
							correctY = currentRow*(8 + interval)*pxSize;
							if ((symbols8bitTable[alphabetId][symbolId*64 + 0] +
								symbols8bitTable[alphabetId][symbolId*64 + 8]+
								symbols8bitTable[alphabetId][symbolId*64 + 16] + 
								symbols8bitTable[alphabetId][symbolId*64 + 24] +
								symbols8bitTable[alphabetId][symbolId*64 + 32] +
								symbols8bitTable[alphabetId][symbolId*64 + 40] +
								symbols8bitTable[alphabetId][symbolId*64 + 48] +
								symbols8bitTable[alphabetId][symbolId*64 + 56]) == 0){
								if ((symbols8bitTable[alphabetId][symbolId*64 + 0 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 8 + 1]+
									symbols8bitTable[alphabetId][symbolId*64 + 16 + 1] + 
									symbols8bitTable[alphabetId][symbolId*64 + 24 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 32 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 40 + 1] + 
									symbols8bitTable[alphabetId][symbolId*64 + 48 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 56 + 1]) == 0){
									correctX--;
								}
								// correctX--;
							}
							// position the cursor
							curX = place->x + j*8*pxSize + correctX*pxSize;
							// curX = place->x + j*8*pxSize;
							curY = place->y + correctY;
							
							// draw the symbol in its place
							for(l = 0; l < 8; l++){
								for(k = 0; k < 8; k++){
									if (symbols8bitTable[alphabetId][symbolId*64 + k + l*8] == 1){
										RenderPixelFont(curX + k*pxSize, curY + l*pxSize, pxSize, fontColor);
									}
								}
							}
							alphabetId = 0; // reset alphabet
							symbolId = 0; // reset symbol index in that alphabet
							j++; // we move through the line!
							foundSymbol = true;
						} else {
							alphabetId++; // look in an another alphabet
							symbolId = 0; // reset symbol index in that alphabet
						}
					}
				}
			}
			if (SDL_MUSTLOCK(fontTarget->surface)){
				SDL_UnlockSurface(fontTarget->surface);
			}
			fontTarget->update();
		}
		
		cTexture* RenderTextToTexture(SDL_Rect* place,		// where to draw  
			int pxSize, 			// size of the pen
			Colors::RGBA color, 	// color
			std::u32string text, 	// what to draw
			int interval = 2){	// interval between lines
			
			// cols and rows with no respect to spaces trimming made via correctX variable
			int cols = place->w / (8*pxSize);
			int rows = place->h / ((8 + interval)*pxSize);
			int currentRow = 0;
			long long unsigned int symbolId = 0; // the type corresponds to std::u32string::npos
			int correctX = 0;
			int correctY = 0;
			int curX;
			int curY;
			int alphabetId = 0;
			bool foundSymbol = false;
			cTexture* textTexture = new cTexture(renderer, place->w, place->h, 0, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
			SDL_SetTextureBlendMode(textTexture->texture, SDL_BLENDMODE_BLEND);
			uint32_t fontColor = SDL_MapRGBA(textTexture->surface->format,
				color.r, color.g, color.b, color.a);
			
			// for each symbol in text
			j = 0; // index of a symbol inside a line being typed right now
			if (SDL_MUSTLOCK(textTexture->surface)){
				SDL_LockSurface(textTexture->surface);
			}
			for(i = 0; i < text.size(); i++){
				// load font symbol
				foundSymbol = false;
				while ((!foundSymbol) && (alphabetId < FontManager->alphabet.size())){
					symbolId = FontManager->alphabet[alphabetId].find(text[i]);
					// force a new line if '\n' found
					if (symbolId == 95){
						foundSymbol = true;
						currentRow++;
						j = 0; // reset the index inside a line
						correctX = 0;
						symbolId = 0;
						alphabetId = 0;
					} else {
						if (symbolId != std::u32string::npos){
							// Correct coordinates of the "cursor"
							if (j > cols){
								currentRow++;
								j = 0;
								correctX = 0;
							}
							// add an interval between lines
							correctY = currentRow*(8 + interval)*pxSize;
							// cut empty space between characters
							if ((symbols8bitTable[alphabetId][symbolId*64 + 0] +
								symbols8bitTable[alphabetId][symbolId*64 + 8]+
								symbols8bitTable[alphabetId][symbolId*64 + 16] + 
								symbols8bitTable[alphabetId][symbolId*64 + 24] +
								symbols8bitTable[alphabetId][symbolId*64 + 32] +
								symbols8bitTable[alphabetId][symbolId*64 + 40] +
								symbols8bitTable[alphabetId][symbolId*64 + 48] +
								symbols8bitTable[alphabetId][symbolId*64 + 56]) == 0){
								if ((symbols8bitTable[alphabetId][symbolId*64 + 0 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 8 + 1]+
									symbols8bitTable[alphabetId][symbolId*64 + 16 + 1] + 
									symbols8bitTable[alphabetId][symbolId*64 + 24 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 32 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 40 + 1] + 
									symbols8bitTable[alphabetId][symbolId*64 + 48 + 1] +
									symbols8bitTable[alphabetId][symbolId*64 + 56 + 1]) == 0){
									correctX--;
								}
								// correctX--;
							}
							// position the cursor
							curX = j*8*pxSize + correctX*pxSize;
							curY = correctY;
							
							// draw the symbol in its place
							for(l = 0; l < 8; l++){
								for(k = 0; k < 8; k++){
									if (symbols8bitTable[alphabetId][symbolId*64 + k + l*8] == 1){
										RenderPixelFontToTexture(textTexture, curX + k*pxSize, curY + l*pxSize, pxSize, fontColor);
									}
								}
							}
							alphabetId = 0; // reset alphabet
							symbolId = 0; // reset symbol index in that alphabet
							j++; // we move through the line!
							foundSymbol = true;
						} else {
							alphabetId++; // look in an another alphabet
							symbolId = 0; // reset symbol index in that alphabet
						}
					}
				}
			}
			if (SDL_MUSTLOCK(textTexture->surface)){
				SDL_UnlockSurface(textTexture->surface);
			}
			textTexture->update();
			return textTexture;
		}
		
		void RenderBorder(cTexture* target, int x, int y, int width, int height, int pxSize){
			Colors::RGBA color = Colors::RGBA_Cyan6;
			
			RenderRect(target, x, y, width, height, pxSize, 
					   SDL_MapRGBA(target->surface->format, color.r, color.g, color.b, color.a));
		}
		
		void placeSurface(SDL_Surface* surface, SDL_Rect* stretchRect){
			SDL_UpperBlitScaled(surface, NULL, screenSurface, stretchRect);
		}
		
		void placeTexture(SDL_Texture* texture, SDL_Rect* stretchRect){
			SDL_RenderCopy(renderer, texture, NULL, stretchRect);
		}
		
		void demoDrawPixels(){
			int maxPixels = 2000;
			int pxSize = 4;
			int m = 0;
			// int mx = (winWidth - 100);
			int mx = textureLayers[0]->surface->w;
			// int my = (winHeight - 50);
			int my = textureLayers[0]->surface->h;
			
			uint32_t color;
			std::vector<int> pxX, pxY, pxColor;
			for(int i=0;i<maxPixels;i++){
				m = rand() % (mx / pxSize);
				pxX.push_back(m*pxSize);
				
				m = rand() % (my / pxSize);
				pxY.push_back(0 + m*pxSize);
				
				pxColor.push_back(0 + (rand() % 8));
			}
			setPalette8(8);
			
			if (SDL_MUSTLOCK(textureLayers[0]->surface)){
				SDL_LockSurface(textureLayers[0]->surface);
			}
			for(int i=0; i<maxPixels; i++){
				color = SDL_MapRGBA(textureLayers[0]->surface->format, 
					Palette[pxColor[i]].r,
					Palette[pxColor[i]].g,
					Palette[pxColor[i]].b,
					0xFF);
				
				RenderPixel(textureLayers[0], pxX[i], pxY[i], pxSize, color);
			}
			if (SDL_MUSTLOCK(textureLayers[0]->surface)){
				SDL_UnlockSurface(textureLayers[0]->surface);
			}
			textureLayers[0]->update();
			
			pxColor.clear();
			pxX.clear();
			pxY.clear();
		}
		
		void demoDrawCircles(int circlesCount=20, int size=4){
			int maxCircles = circlesCount;
			int pxSize = size;
			int m = 0;
			int mx = (winWidth - 100);
			int my = (winHeight - 50);
			int mr = 200;
			uint32_t color;
			SDL_Rect circleRect = {50, 0, mx, my};
			std::vector<int> pxX, pxY, pxR, pxColor;
			
			for(i = 0; i < maxCircles; i++){
				m = rand() % (mx / pxSize);
				pxX.push_back(50 + m*pxSize);
				
				m = rand() % (my / pxSize);
				pxY.push_back(0 + m*pxSize);
				
				m = rand() % (mr);
				pxR.push_back(0 + m);
				
				pxColor.push_back(0 + (rand() % 8));
			}
			setPalette8(8);
			if (SDL_MUSTLOCK(textureLayers[0]->surface)){
				SDL_LockSurface(textureLayers[0]->surface);
			}
			for(i = 0; i < maxCircles; i++){
				color = SDL_MapRGBA(textureLayers[0]->surface->format, 
									Palette[pxColor[i]].r,
									Palette[pxColor[i]].g,
									Palette[pxColor[i]].b,
									0xFF);
				RenderFilledCircle(textureLayers[0], pxX[i], pxY[i], pxR[i], pxSize, color);
			}
			if (SDL_MUSTLOCK(textureLayers[0]->surface)){
				SDL_UnlockSurface(textureLayers[0]->surface);
			}
			textureLayers[0]->update();
			pxColor.clear();
			pxX.clear();
			pxY.clear();
			pxR.clear();
		}
		
		void updateScreen(){
			SDL_RenderPresent(renderer);
		}
		
		void updateScreenSurface(){
			clearSurface(screenSurface, Colors::RGBA_Black8, 0xFF);
			if (surfaceLayers.size() > 1){
				for (i = 1; i < surfaceLayers.size(); i++){
					SDL_BlitSurface(surfaceLayers[i]->source, NULL, surfaceLayers[0]->source, NULL);
				}
			}
			SDL_BlitSurface(surfaceLayers[0]->source, NULL, screenSurface, NULL);
			
			SDL_UpdateWindowSurface(window);
		}
		
		void clearScreen(){
			setPalette8(8);
			SDL_SetRenderDrawColor( renderer, 
				Palette[0].r,
				Palette[0].g,
				Palette[0].b,
				Palette[0].a);
			SDL_RenderClear(renderer);
		}
	private:
		static cGraphics* instance;
	};
}
#endif
