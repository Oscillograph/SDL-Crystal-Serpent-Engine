#ifndef CGAMEOBJECTSERVITOR_H
#define CGAMEOBJECTSERVITOR_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	/* ------------------
	  This is a game object servitor class
	  ------------------ */
	class cGameObjectServitor {
	public:
		void setGraphics(Objects::cGameObject* object, cTexture* newGraphics){
			object->sprite = newGraphics;
		};
		cTexture* getGraphics(Objects::cGameObject* object){
			return object->sprite;
		};
		
		void correctGeometry(Objects::cGameObject* object){
			object->place.x = object->centerOfMass.x - object->geometryCorrection.x;
			object->place.y	= object->centerOfMass.y - object->geometryCorrection.y;
		}
		
		void correctGeometry(entt::entity entity, entt::registry* reg){
			auto* sprite = &(reg->get<Components::sprite>(entity));
			auto* physics2D = &(reg->get<Components::physics2D>(entity));
			sprite->place.x = physics2D->centerOfMass.x + sprite->geometryCorrection.x;
			sprite->place.y	= physics2D->centerOfMass.y + sprite->geometryCorrection.y;
		}
		
		// TEXT OBJECTS
		void updateContent(SDLCSE::cGraphics* Graphics, Objects::cGameObject* tObject, std::u32string content,
			SDL_FRect* place, SDLCSE::Colors::RGBA color, int pxSize=1, int interval=1){
			tObject->content = content;
			delete tObject->sprite;
			tObject->sprite = Graphics->RenderTextToTexture(place, pxSize, color, content, interval);
		}
	};
}

#endif
