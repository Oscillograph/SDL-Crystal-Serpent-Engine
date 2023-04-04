#ifndef CGRAPHICS_DIRECTSURFACEDRAW_H
#define CGRAPHICS_DIRECTSURFACEDRAW_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	/*
	  As stated by the name, this class provides methods to draw via pixel access on surfaces
	 */
	class cGraphics_DirectSurfaceDraw {
	public:
		std::vector<cSurface*> surfaceLayers;
		int xn, yn;
		uint32_t* pxBuffer;
		uint32_t colorBuffer;
		
		// shamelessly taken from 
		// https://github.com/flydecahedron/The-Black-Art-of-Multiplatform-Game-Programming/blob/master/Graphics.cpp
		void DrawPixel(int layer, int x, int y, Colors::RGBA color, int alpha=0xFF){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}
				
				if ((x < surfaceLayers[layer]->source->w) && 
					(y < surfaceLayers[layer]->source->h) && 
					(x >= 0) && 
					(y >= 0)){
					Uint32* buffer;
					Uint32 c; // color data in the pixel format
					c = SDL_MapRGBA(surfaceLayers[layer]->source->format, color.r, color.g, color.b, alpha);
					buffer = (Uint32*)surfaceLayers[layer]->source->pixels + y*surfaceLayers[layer]->source->pitch/4 + x;
					*buffer = c;
				}
				
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawPixel(int layer, int x, int y, uint32_t color){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}
				
				if ((x < surfaceLayers[layer]->source->w) && 
					(y < surfaceLayers[layer]->source->h) && 
					(x >= 0) && 
					(y >= 0)){
					pxBuffer = (Uint32*)surfaceLayers[layer]->source->pixels + y*surfaceLayers[layer]->source->pitch/4 + x;
					*pxBuffer = color;
				}
				
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawPixel(SDL_Surface* surface, int x, int y, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
				 	SDL_LockSurface(surface);
				}
				
				if ((x < surface->w) && 
					(y < surface->h) && 
					(x >= 0) && 
					(y >= 0)){
					colorBuffer = SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
					pxBuffer = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
					// *buffer = SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
					*pxBuffer = colorBuffer;
					// surface->pixels = (void *)buffer;
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawPixel(SDL_Surface* surface, int x, int y, uint32_t color){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				if ((x < surface->w) && 
					(y < surface->h) && 
					(x >= 0) && 
					(y >= 0)){
					pxBuffer = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
					*pxBuffer = color;
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawPixelSized(SDL_Surface* surface, int x, int y, int pxSize, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				for (int k = 0; k < pxSize; k++){
					xn = x + k;
					for (int l = 0; l < pxSize; l++){
						 yn = y + l;
						 if ((xn < surface->w) && (yn < surface->h)){
							 colorBuffer = SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
							 pxBuffer = (uint32_t*)surface->pixels + yn*surface->pitch/4 + xn;
							 *pxBuffer = colorBuffer;
						 }
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawPixelSized(SDL_Surface* surface, int x, int y, int pxSize, uint32_t color){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				for (int k = 0; k < pxSize; k++){
					xn = x + k;
					for (int l = 0; l < pxSize; l++){
						yn = y + l;
						if ((xn < surface->w) && (yn < surface->h)){
							pxBuffer = (uint32_t*)surface->pixels + yn*surface->pitch/4 + xn;
							*pxBuffer = color;
						}
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawLine(int layer, int x1, int y1, int x2, int y2, Colors::RGBA color, int alpha=0xFF){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}
				
				// As we draw a line, we need to construct its equation
				// to determine coordinates between start and end points.
				// 					y = kx + c
				int k = y1/x1; 
				int c = y1 - k*x1; 
 
				for (xn = x1; xn < x2; xn++){
					yn = k*xn + c;
					DrawPixel(layer, xn, yn, color, alpha);
				}
				
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawLine(SDL_Surface* surface, int x1, int y1, int x2, int y2, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				// As we draw a line, we need to construct its equation
				// to determine coordinates between start and end points.
				// 					y = kx + c
				int k = y1/x1; 
				int c = y1 - k*x1; 
				
				for (xn = x1; xn < x2; xn++){
					yn = k*xn + c;
					DrawPixel(surface, xn, yn, color, alpha);
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawRect(int layer, int x, int y, int w, int h, Colors::RGBA color, int alpha=0xFF){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}
				
				if ((x + w) > surfaceLayers[layer]->source->w){
					int pxWidth = (int)(round(surfaceLayers[layer]->source->w - x));
				} 
				if ((y + h) > surfaceLayers[layer]->source->h){
					int pxHeight = (int)(round(surfaceLayers[layer]->source->h - y));
				}
				
				DrawLine(layer, x, y, x+w, y, color, alpha); // draw top line
				DrawLine(layer, x+w, y, x+w, y+h, color, alpha); // draw right line
				DrawLine(layer, x+w, y+h, x, y+h, color, alpha); // draw bottom line
				DrawLine(layer, x, y+h, x, y, color, alpha); // draw left line
				
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawRect(SDL_Surface* surface, int x, int y, int w, int h, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				if ((x + w) > surface->w){
					int pxWidth = (int)(round(surface->w - x));
				} 
				if ((y + h) > surface->h){
					int pxHeight = (int)(round(surface->h - y));
				}
				
				DrawLine(surface, x, y, x+w, y, color, alpha); // draw top line
				DrawLine(surface, x+w, y, x+w, y+h, color, alpha); // draw right line
				DrawLine(surface, x+w, y+h, x, y+h, color, alpha); // draw bottom line
				DrawLine(surface, x, y+h, x, y, color, alpha); // draw left line
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawCircle(SDL_Surface* surface, int x, int y, int r, int pxSize, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				double circlePerimeter = PI * r * 2; // find the perimeter of a circle
				int pxNumber = (int)round(circlePerimeter / pxSize); // count pixels in the perimeter
				
				double degreeStep = 2*PI / pxNumber; // find how much degrees in each pixel
				for (int j = 0; j < pxNumber; j++) { // draw the pixels
					xn = (int)(x + round(r*cos(j*degreeStep)));
					yn = (int)(y + round(r*sin(j*degreeStep)));
					xn = xn - (xn % pxSize);
					yn = yn - (yn % pxSize);
					
					if ((xn < surface->w) && (yn < surface->h) && (xn >= 0) && (yn >= 0)){
						DrawPixelSized(surface, xn, yn, pxSize, color, alpha);
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawFilledCircle2(SDL_Surface* surface, int x, int y, int r, int pxSize, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				x = x - (x % pxSize);
				y = y - (y % pxSize);
				int r2 = r*r;
				colorBuffer = SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
				
				for (int k = -r; k <= r; k = k + pxSize){
					for (int l = -r; l <= r; l = l + pxSize){
						if (k*k + l*l <= r2){
							xn = x + k;
							yn = y + l;
							xn = xn - (xn % pxSize);
							yn = yn - (yn % pxSize);
							
							if ((xn < surface->w) && (yn < surface->h) && (xn >= 0) && (yn >= 0)){
								DrawPixelSized(surface, xn, yn, pxSize, colorBuffer);
							}
						}
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawFilledCircle(SDL_Surface* surface, int x, int y, int r, int pxSize, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
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
							
							if ((xn < surface->w) && (yn < surface->h) && (xn >= 0) && (yn >= 0)){
								DrawPixelSized(surface, xn, yn, pxSize, color, alpha);
							}
						}
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawFilledRect(int layer, int x, int y, int w, int h, Colors::RGBA color, int alpha=0xFF){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}

				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = 0; 
				int pxHeight = 0;
				if ((x + w) > surfaceLayers[layer]->source->w){
					pxWidth = (int)(round(surfaceLayers[layer]->source->w - x));
				} 
				if ((y + h) > surfaceLayers[layer]->source->h){
					pxHeight = (int)(round(surfaceLayers[layer]->source->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						DrawPixel(layer, x+j, y+i, color, alpha);
					}
				}

				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawFilledRect(int layer, int x, int y, int w, int h, uint32_t color){
			if (surfaceLayers[layer]->source != NULL){
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_LockSurface(surfaceLayers[layer]->source);
				}
				
				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = 0; 
				int pxHeight = 0;
				if ((x + w) > surfaceLayers[layer]->source->w){
					pxWidth = (int)(round(surfaceLayers[layer]->source->w - x));
				} 
				if ((y + h) > surfaceLayers[layer]->source->h){
					pxHeight = (int)(round(surfaceLayers[layer]->source->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						DrawPixel(layer, x+j, y+i, color);
					}
				}
				
				if (SDL_MUSTLOCK(surfaceLayers[layer]->source)){
					SDL_UnlockSurface(surfaceLayers[layer]->source);
				}
			}
		}
		
		void DrawFilledRect(SDL_Surface* surface, int x, int y, int w, int h, uint32_t color){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = 0; 
				int pxHeight = 0;
				if ((x + w) > surface->w){
					pxWidth = (int)(round(surface->w - x));
				} 
				if ((y + h) > surface->h){
					pxHeight = (int)(round(surface->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						DrawPixel(surface, x+j, y+i, color);
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void DrawFilledRect(SDL_Surface* surface, int x, int y, int w, int h, Colors::RGBA color, int alpha=0xFF){
			if (surface != NULL){
				if (SDL_MUSTLOCK(surface)){
					SDL_LockSurface(surface);
				}
				
				// Since DrawPixel checks conditions of x and y,
				// we only need to make sure that w and h are in bounds.
				int pxWidth = w; 
				int pxHeight = h;
				if ((x + w) > surface->w){
					pxWidth = (int)(round(surface->w - x));
				} 
				if ((y + h) > surface->h){
					pxHeight = (int)(round(surface->h - y));
				} 
				
				for (int i = 0; i < pxHeight; i++){
					for (int j = 0; j < pxWidth; j++){
						DrawPixel(surface, x+j, y+i, color, alpha);
					}
				}
				
				if (SDL_MUSTLOCK(surface)){
					SDL_UnlockSurface(surface);
				}
			}
		}
		
		void CreateSurfaceLayers(int number, int w, int h, int bpp=32){
			cSurface* newLayer = NULL;
			int j, k, l = 0;
			
			for(int j = 0; j < number; j++){
				if (j > 0) {
					#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					newLayer = new cSurface(w, h, 0, bpp, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
					#else
					newLayer = new cSurface(w, h, 0, bpp, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
					#endif
				} else {
					newLayer = new cSurface(w, h, 0, bpp, 0, 0, 0, 0);
				}

				surfaceLayers.push_back(newLayer);
			}
		}
		
		void clearSurface(SDL_Surface* surface, Colors::RGBA color, int alpha=0xFF){
			SDL_Rect tmpRect = {0, 0, surface->w, surface->h};
			// DrawFilledRect(surface, 0, 0, surface->w, surface->h, color, alpha);
			// DrawFilledRect(surface, 0, 0, surface->w, surface->h, SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha));
			SDL_FillRect(surface, &tmpRect, SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha));
		}
	};
}
#endif
