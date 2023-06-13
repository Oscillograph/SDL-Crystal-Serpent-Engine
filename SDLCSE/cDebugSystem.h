#ifndef CDEBUGSYSTEM_H
#define CDEBUGSYSTEM_H

#include "./common.h"
// #include <stdarg.h>

namespace SDLCSE {
	class cDebugSystem {
	public:
		std::stringstream u8stream;
		std::string u8string;
		std::u32string u32string;
		u32stringstream u32stream;
		Objects::cTextObject* debugText;
		
		cGameObjectServitor* GameObjectServitor;
		cGraphics* Graphics;
		
		cDebugSystem(cGraphics* GraphicsSystem, float x, float y, float w, float h){
			Graphics = GraphicsSystem;

			// Create the debugging message field
			debugText = new Objects::cTextObject();
			debugText->type = Objects::GAME_OBJECT_TYPE_TEXT;
			debugText->place = {x, y, w, h};
			debugText->content = u32stream.str();
			debugText->sprite = Graphics->RenderTextToTexture(&(debugText->place), 2, Colors::RGBA_Yellow8, debugText->content, 1);
			debugText->centerOfMass.x = (debugText->place.x + debugText->place.w)/2; 
			debugText->centerOfMass.y = (debugText->place.y + debugText->place.h)/2;
			debugText->geometryCorrection.x = debugText->centerOfMass.x - debugText->place.x;
			debugText->geometryCorrection.y = debugText->centerOfMass.y - debugText->place.y;
			debugText->velocity[0] = 0; // px per second
			debugText->velocity[1] = 0; // px per second
			debugText->display = false;
			
			Reset();
		};
		~cDebugSystem(){};
		
		void Reset(){
			u8string = "";
			u32string = U"";
			u8stream.str(std::string());
			u32stream.str(std::u32string());
			//u32stream << U"Debug Text";
		}
		
		// a simple template to add printable things to utf8 string
		// shamelessly taken from: https://stackoverflow.com/a/52970404
		template <typename T>
		void u8add(T t) {
			u8stream << t;
		}
		
		template<typename... Args>
		void Debug(Args... args){
			u8stream.str(std::string()); // clear utf8 buffer
			u32string = U""; // clear utf32 string
			
			// add args to utf8 string stream
			int dummyLambda[] = { 0, ((void) u8add(std::forward<Args>(args)),0)... };
			u8string = u8stream.str();
			
			// convert it to utf32
			utf8::utf8to32(u8string.begin(), u8string.end(), std::back_inserter(u32string));
			
			u32stream << u32string;
		}
		
		void Display(){
			GameObjectServitor->updateContent(Graphics, debugText, u32stream.str(), 
										&(debugText->place), SDLCSE::Colors::RGBA_Yellow8, 2, 1);
		}
	};
}

#endif
