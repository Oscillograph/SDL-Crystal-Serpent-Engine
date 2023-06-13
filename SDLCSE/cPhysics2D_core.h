#ifndef CPHYSICS2D_H
#define CPHYSICS2D_H

// SDLCSE
#include "./common.h"

// This is an ASCII art representation of what the objectsGrid should describe
// x----x----x----x-----
// |    |    |    |    |
// x____x____x____x____|
// |    |    |    |    |
// |____|____|____|____|
// Objects should be connected to one or up to 4 cells in this grid depending on object coordinates.
// This should allow a simple technique to determining a set of objects to check if they collide.
// "x" are the FPoints stored in objectsGrid. Cells are "pxStep" in width and height. 

namespace SDLCSE{
	struct gridCellContainer{
		std::vector<Objects::cGameObject*> objects;
		std::vector<entt::entity> entities;
		float x;
		float y;
	};
	
	/* ------------------
	  This is a physics subsystem class
	  ------------------ */
	class cPhysics2D {
	public:
		int i, j, k, l; // indexes for cycles
		int iMin, iMax, jMin, jMax, kMin, kMax, lMin, lMax; // various intervals limits
		int time0;
		float time;
		bool timeToReset;
		
		cGameObjectServitor* GameObjectServitor;
		cLogSystem* LogSystem;
		cDebugSystem* DebugSystem;
		cGraphics* Graphics;
		
		// std::vector<std::vector<SDL_FPoint>> objectsGrid;
		std::vector<gridCellContainer*> objectsGrid;
		float pxStep;
		int gridCellCountTotal;
		int gridCellCountRow;
		
		cPhysics2D(){
		}
		~cPhysics2D(){
		}
		
		// physics engine has a timer
		void timeReset(){
			time0 = SDL_GetTicks64();
			time = time0 / 1000.f;
		}
		
		// every object capable of physical interaction has its own timer
		void timeReset(Objects::cGameObject* object){
			object->time0 = SDL_GetTicks64();
		}
		
		// every entity capable of physical interaction has its own timer
		void timeReset(entt::entity entity, entt::registry* reg){
			auto* physics = &(reg->get<Components::physics2D>(entity));
			physics->time0 = SDL_GetTicks64();
		}
		
		void getTime(){
			time = (SDL_GetTicks64() - time0) / 1000.f;
		}
		
		void getTime(Objects::cGameObject* object){
			object->time = (SDL_GetTicks64() - object->time0) / 1000.f;
		}
		
		void getTime(entt::entity entity, entt::registry* reg){
			auto* physics = &(reg->get<Components::physics2D>(entity));
			physics->time = (SDL_GetTicks64() - physics->time0) / 1000.f;
		}
		
		void gridCreate(SDL_FRect* place){
			// create a Cartesian grid in the rectangle "place" 
			pxStep = 50;
			gridCellContainer* tempCell;
			gridCellCountTotal = floor(place->w * place->h / pxStep);
			gridCellCountRow = floor(place->w / pxStep);
			for (i=0; i < gridCellCountTotal; i++){
				j = floor(i / gridCellCountRow);
				tempCell->x = place->x + (i - j*gridCellCountRow)*pxStep;
				tempCell->y = place->y + j*pxStep;
				objectsGrid.push_back(tempCell);
			}
		}
		
		bool gridLinkObject(Objects::cGameObject* object){
			bool inCell = false;
			iMin = floor(object->place.x / pxStep) + floor(object->place.y / pxStep)*gridCellCountRow;
			iMax = floor((object->place.x + object->place.w) / pxStep) 
								+ floor((object->place.y + object->place.h) / pxStep)*gridCellCountRow;
			for (i=iMin; i < iMax; i++){
				inCell = false;
				for (j=0; (j < objectsGrid[i]->objects.size()) && (!inCell); j++){
					if (&(objectsGrid[i]->objects[j]) == &object){
						inCell = true;
					}
				}
				if (!inCell){
					objectsGrid[i]->objects.push_back(object);
					inCell = true;
				}
			}
			return inCell;
		}
		
		void gridUnLinkObject(Objects::cGameObject* object){
			bool inCell = false;
			iMin = floor(object->place.x / pxStep) + floor(object->place.y / pxStep)*gridCellCountRow;
			// LogSystem->log(std::to_string(iMin));
			iMax = floor((object->place.x + object->place.w) / pxStep) 
								+ floor((object->place.y + object->place.h) / pxStep)*gridCellCountRow;
			// LogSystem->log(std::to_string(iMax));
			for (i=iMin; i < iMax; i++){
				inCell = false; 
				k = 0;
				// LogSystem->log(std::to_string(i));
				for (j=0; (j < objectsGrid[i]->objects.size()) && (!inCell); j++){
					//if (i == 0){ LogSystem->log("j = " + std::to_string(j));}
					if (&(objectsGrid[i]->objects[j]) == &object){
						inCell = true;
						k = j;
					}
				}
				// LogSystem->log("k = " + std::to_string(k));
				if (inCell){
					objectsGrid[i]->objects.erase(objectsGrid[i]->objects.begin() + k);
					inCell = false;
				}
			}
		}
		
		void gridLinkRoutine(Objects::cGameObject* object){
			gridUnLinkObject(object);
			gridLinkObject(object);
		}
		
		// move entity
		void move(entt::entity entity, entt::registry* reg){
			auto* physics2D = &(reg->get<Components::physics2D>(entity));
			
			//if (Math::length(physics2D->velocity) > 1.0f){
			if (physics2D->canMove){
				physics2D->velocity.x += physics2D->acceleration.x*physics2D->time;
				physics2D->velocity.y += physics2D->acceleration.y*physics2D->time;
				
				float moveX = physics2D->velocity.x * physics2D->time;
				float moveY = physics2D->velocity.y * physics2D->time;
				
				physics2D->centerOfMass.x += moveX;
				physics2D->centerOfMass.y += moveY;
				
				auto* hitBox = &(reg->get<Components::hitBox>(entity));
				for (i = 0; i < hitBox->points.size(); i++){
					hitBox->points[i].x += moveX;
					hitBox->points[i].y += moveY;
				}
				GameObjectServitor->correctGeometry(entity, reg);
			}
		}
		
		// unmove entity
		void unMove(entt::entity entity, entt::registry* reg){
			auto* physics2D = &(reg->get<Components::physics2D>(entity));
			auto* hitBox = &(reg->get<Components::hitBox>(entity));
			
			//physics2D->velocity.x -= physics2D->acceleration.x*physics2D->time;
			//physics2D->velocity.y -= physics2D->acceleration.y*physics2D->time;
			float moveX = physics2D->velocity.x * physics2D->time;
			float moveY = physics2D->velocity.y * physics2D->time;
			
			physics2D->centerOfMass.x -= moveX;
			physics2D->centerOfMass.y -= moveY;
			for (i = 0; i < hitBox->points.size(); i++){
				hitBox->points[i].x -= moveX;
				hitBox->points[i].y -= moveY;
			}
			GameObjectServitor->correctGeometry(entity, reg);
		}
		
		void move(Objects::cGameObject* object){
			object->centerOfMass.x += object->velocity[0]*object->time;
			object->centerOfMass.y += object->velocity[1]*object->time;
			GameObjectServitor->correctGeometry(object);
		}
		
		void unMove(Objects::cGameObject* object){
			object->centerOfMass.x -= object->velocity[0]*object->time;
			object->centerOfMass.y -= object->velocity[1]*object->time;
			GameObjectServitor->correctGeometry(object);
		}
		
		// check if collision happened
		int isCollision(entt::entity entityA, entt::entity entityB, entt::registry* reg){
			auto* hitBoxA = &(reg->get<Components::hitBox>(entityA));
			auto* hitBoxB = &(reg->get<Components::hitBox>(entityB));
			if ((hitBoxA->type == Objects::HIT_BOX_CIRCLE) &&
				(hitBoxB->type == Objects::HIT_BOX_CIRCLE)){
				// CIRCLE and CIRCLE
				if (hitBoxA->radius + hitBoxB->radius - Math::distance(hitBoxA->points[0], hitBoxB->points[0]) > 0){
					return 1;
				}
			}
			if (((hitBoxA->type == Objects::HIT_BOX_CIRCLE) && (hitBoxB->type == Objects::HIT_BOX_LINE)) ||
				((hitBoxA->type == Objects::HIT_BOX_LINE) && (hitBoxB->type == Objects::HIT_BOX_CIRCLE))){
				// CIRCLE and LINE
				// 		or
				// LINE and CIRCLE
				// pour some syntax sugar on me
				auto* hitBoxCircle = hitBoxA;
				auto* hitBoxLine = hitBoxB;
				
				if (hitBoxA->type != Objects::HIT_BOX_CIRCLE){
					hitBoxCircle = hitBoxB;
					hitBoxLine = hitBoxA;
				}
				
				SDL_FPoint pointA = hitBoxLine->points[0];
				SDL_FPoint pointB = hitBoxLine->points[1];
				SDL_FPoint pointC = hitBoxCircle->points[0];
				SDL_FPoint pointM = {0.0f, 0.0f};
				
				SDL_FPoint ABvector = pointB - pointA;
				SDL_FPoint BCvector = pointB - pointC;
				SDL_FPoint ACvector = pointC - pointA;
				float ABlength = Math::length(ABvector);
				float BClength = Math::length(BCvector);
				float AClength = Math::length(ACvector);
				
				SDL_FPoint uAB = Math::findNorm(ABvector); // norm of AB
				SDL_FPoint AMvector = Math::dotProduct(ACvector, uAB)*uAB; // projection of AC on AB
				float AMlength = Math::length(AMvector);
				
				pointM = pointA + AMvector; // here's where the normal should be!
				
				if (AMlength / ABlength > 1){
					pointM = pointB;
				} else {
					if (Math::length(ABvector + AMvector) < ABlength){
						pointM = pointA;
					}
				}
				
				SDL_FPoint CMvector = pointM - pointC; // vector from circle C to line
				float CMlength = Math::length(CMvector); // find CM length
				
				if (CMlength < hitBoxCircle->radius){
					return 2;
				}
			}
			if ((hitBoxA->type == Objects::HIT_BOX_LINE) &&
				(hitBoxB->type == Objects::HIT_BOX_LINE)){
				// collide LINE and LINE
				// TODO: check for collision between line and line
				// return depends: 3 or 0
			}
			return 0;
		}
		
		// COLLISION REACTION ROUTINES
		void collideCircleCircle(entt::entity entityA, entt::entity entityB, entt::registry* reg){
			// 0. Get componens to work with
			auto* hitBoxA = &(reg->get<Components::hitBox>(entityA));
			auto* hitBoxB = &(reg->get<Components::hitBox>(entityB));
			auto* physicsA = &(reg->get<Components::physics2D>(entityA));
			auto* physicsB = &(reg->get<Components::physics2D>(entityB));
			
			// 1. De-overlap phase - based on hitBoxes
			// {dx, dy} - vector that connects centers of the circles, it's axis N in the notebook
			float dx = hitBoxB->points[0].x - hitBoxA->points[0].x;
			float dy = hitBoxB->points[0].y - hitBoxA->points[0].y;
			SDL_FPoint un = Math::findNorm({dx, dy}); // unit vector of N axis (look up in the notebook, 16.05.2023)
			
			// overlapDistance for each circle (that's why it's only half of the difference)
			float overlapDistance = 0.5f*(hitBoxA->radius + hitBoxB->radius - Math::distance(hitBoxA->points[0], hitBoxB->points[0]));
			
			if (physicsA->canBounce && physicsB->canBounce){
				hitBoxB->points[0].x += overlapDistance*un.x;
				hitBoxB->points[0].y += overlapDistance*un.y;
				hitBoxA->points[0].x -= overlapDistance*un.x;
				hitBoxA->points[0].y -= overlapDistance*un.y;
			} else {
				if (physicsA->canBounce){
					hitBoxA->points[0].x -= 2*overlapDistance*un.x;
					hitBoxA->points[0].y -= 2*overlapDistance*un.y;
				}
				if (physicsB->canBounce){
					hitBoxB->points[0].x += 2*overlapDistance*un.x;
					hitBoxB->points[0].y += 2*overlapDistance*un.y;
				}
			}
			
			// for circles, hitBox->points[0].xy is equivalent to physics2D->centerOfMass.xy
			physicsA->centerOfMass = hitBoxA->points[0];
			physicsB->centerOfMass = hitBoxB->points[0];
			
			// 2. Bounce phase - based on physics2D
			SDL_FPoint Va0 = physicsA->velocity; // backup initial velocity of entity A
			SDL_FPoint Vb0 = physicsB->velocity; // backup initial velocity of entity B
			
			SDL_FPoint ut = {-un.y, un.x}; // unit vector of T axis ortogonal to N axis
			
			SDL_FPoint Va0nt = {Math::dotProduct(Va0, ut), Math::dotProduct(Va0, un)};
			SDL_FPoint Vb0nt = {Math::dotProduct(Vb0, ut), Math::dotProduct(Vb0, un)};
			
			// DebugSystem->Debug("Va0: {", Va0.x, ", ", Va0.y, "}; Va0nt: {", Va0nt.x, ", ", Va0nt.y, "}\n");
			// LogSystem->logStream("un: {", un.x, ", ", un.y, "}; ut: {", ut.x, ", ", ut.y, "}");
			// LogSystem->logStream("Va0: {", Va0.x, ", ", Va0.y, "}; Va0nt: {", Va0nt.x, ", ", Va0nt.y, "}\n", "Vb0: {", Vb0.x, ", ", Vb0.y, "}; Va0nt: {", Vb0nt.x, ", ", Vb0nt.y, "}");
			
			SDL_FPoint Va1nt = {Va0nt.x, Va0nt.y};
			SDL_FPoint Vb1nt = {Vb0nt.x, Vb0nt.y};
			
			if (physicsA->canBounce && physicsB->canBounce){
				Va1nt.y = (Va0nt.y*(physicsA->mass - physicsB->mass) + 2*physicsB->mass*Vb0nt.y) / (physicsA->mass + physicsB->mass);
				Vb1nt.y = (Vb0nt.y*(physicsB->mass - physicsA->mass) + 2*physicsA->mass*Va0nt.y) / (physicsB->mass + physicsA->mass);
			} else {
				if (physicsA->canBounce){
					Va1nt.y = 2*(Va0nt.y*(physicsA->mass - physicsB->mass) + 2*physicsB->mass*Vb0nt.y) / (physicsA->mass + physicsB->mass);
					Va1nt.y = 2*(Va0nt.y*(physicsA->mass - physicsB->mass) + 2*physicsB->mass*Vb0nt.y) / (physicsA->mass + physicsB->mass);
				}
				if (physicsB->canBounce){
					Vb1nt.y = 2*(Vb0nt.y*(physicsB->mass - physicsA->mass) + 2*physicsA->mass*Va0nt.y) / (physicsB->mass + physicsA->mass);
					Vb1nt.y = 2*(Vb0nt.y*(physicsB->mass - physicsA->mass) + 2*physicsA->mass*Va0nt.y) / (physicsB->mass + physicsA->mass);
				}
			}
			
			SDL_FPoint Va1n = Va1nt.y*un;
			SDL_FPoint Vb1n = Vb1nt.y*un;
			
			SDL_FPoint Va1t = Va1nt.x*ut;
			SDL_FPoint Vb1t = Vb1nt.x*ut;
			
			//LogSystem->logStream("Va1n: {", Va1n.x, ", ", Va1n.y, "}; Va1t: {", Va1t.x, ", ", Va1t.y, "}\n", "Vb1n: {", Vb1n.x, ", ", Vb1n.y, "}; Va1t: {", Vb1t.x, ", ", Vb1t.y, "}");
			
			SDL_FPoint Va1 = Va1n + Va1t;
			SDL_FPoint Vb1 = Vb1n + Vb1t;
			
			//LogSystem->logStream("Va1: {", Va1.x, ", ", Va1.y, "}; Va1nt: {", Va1nt.x, ", ", Va1nt.y, "}\n", "Vb1: {", Vb1.x, ", ", Vb1.y, "}; Va1nt: {", Vb1nt.x, ", ", Vb1nt.y, "}\n");
			
			physicsA->velocity = Va1;
			physicsB->velocity = Vb1;
		}
		
		void collideCircleLine(entt::entity entityA, entt::entity entityB, entt::registry* reg){
			// 0. Get componens to work with
			auto* hitBoxCircle = &(reg->get<Components::hitBox>(entityA));
			auto* physicsCircle = &(reg->get<Components::physics2D>(entityA));
			auto* hitBoxLine = &(reg->get<Components::hitBox>(entityB));
			auto* physicsLine = &(reg->get<Components::physics2D>(entityB));
			
			SDL_FPoint pointA = hitBoxLine->points[0];
			SDL_FPoint pointB = hitBoxLine->points[1];
			SDL_FPoint pointC = hitBoxCircle->points[0];
			
			// 1. De-overlap phase
			SDL_FPoint ABvector = pointB - pointA;
			SDL_FPoint BCvector = pointB - pointC;
			SDL_FPoint ACvector = pointC - pointA;
			float ABlength = Math::length(ABvector);
			float BClength = Math::length(BCvector);
			float AClength = Math::length(ACvector);
			
			// let's find where the collision vector should be plotted
			SDL_FPoint uAB = Math::findNorm(ABvector); // norm of AB
			SDL_FPoint AMvector = Math::dotProduct(ACvector, uAB)*uAB; // projection of AC on AB
			float AMlength = Math::length(AMvector);
			
			SDL_FPoint pointM = pointA + AMvector; // here's where the normal should be!
			
			if (AMlength / ABlength > 1){
				pointM = pointB;
			} else {
				if (Math::length(ABvector + AMvector) < ABlength){
					pointM = pointA;
				}
			} 
			
			SDL_FPoint CMvector = pointM - pointC; // plot the normal vector CM
			
			float CMlength = Math::length(CMvector); // find CM length
			SDL_FPoint uCM = Math::findNorm(CMvector);
			
			SDL_FPoint deOverlap = 0.5*(hitBoxCircle->radius - CMlength)*uCM;
			
			if (physicsCircle->canBounce && physicsLine->canBounce){
				// move hitboxes
				hitBoxCircle->points[0].x -= deOverlap.x;
				hitBoxCircle->points[0].y -= deOverlap.y;
				// move center of mass
				physicsCircle->centerOfMass = hitBoxCircle->points[0];

				hitBoxLine->points[0].x += deOverlap.x;
				hitBoxLine->points[0].y += deOverlap.y;
				hitBoxLine->points[1].x += deOverlap.x;
				hitBoxLine->points[1].y += deOverlap.y;
			} else {
				deOverlap = 2*deOverlap;
				if (physicsCircle->canBounce){
					// move hitbox
					hitBoxCircle->points[0].x -= deOverlap.x;
					hitBoxCircle->points[0].y -= deOverlap.y;
					// move center of mass
					physicsCircle->centerOfMass = hitBoxCircle->points[0];
				}
				if (physicsLine->canBounce){
					// move hitbox
					hitBoxLine->points[0].x += deOverlap.x;
					hitBoxLine->points[0].y += deOverlap.y;
					hitBoxLine->points[1].x += deOverlap.x;
					hitBoxLine->points[1].y += deOverlap.y;
				}
			}
			
			// 2. Bounce phase
			if (physicsCircle->canBounce && physicsLine->canBounce){
				// TODO: bounce line AND circle away from each other
			} else {
				if (physicsCircle->canBounce && physicsLine->canPush){
					SDL_FPoint v1 = physicsCircle->velocity; 
					SDL_FPoint pointK = Math::project(v1, Math::findNorm(pointB - pointA));
					// SDL_FPoint KKvector = pointK - v1;
					SDL_FPoint v2 = v1 + 2*(pointK - v1);
					physicsCircle->velocity = v2;
				}
				// TODO: bounce line from a circle
				if (physicsLine->canBounce && physicsCircle->canPush){}
			}
		}
		
		// TODO: collision line vs line
		void collideLineLine(entt::entity entityA, entt::entity entityB, entt::registry* reg){
			// 0. Get componens to work with
			auto* hitBoxA = &(reg->get<Components::hitBox>(entityA));
			auto* hitBoxB = &(reg->get<Components::hitBox>(entityB));
			auto* physicsA = &(reg->get<Components::physics2D>(entityA));
			auto* physicsB = &(reg->get<Components::physics2D>(entityB));
			
			// 1. De-overlap phase
			if (physicsA->canBounce && physicsB->canBounce){
			} else {
				if (physicsA->canBounce){}
				if (physicsB->canBounce){}
			}
			
			// 2. Bounce phase
			if (physicsA->canBounce && physicsB->canBounce){
			} else {
				if (physicsA->canBounce){}
				if (physicsB->canBounce){}
			}
		}
		
		// process collisions with respect to ECS
		// TODO: process groups of hitboxes
		void processCollisionsEvenDumber(entt::registry* reg){
			// 1. Determine objects that can possibly collide.
			auto view = reg->view<Components::hitBox>();
			int v = 0; //  just two indexes  to make sure that  
			int b = 0; // selected pairs of entities are unique
			for (auto entityA : view){
				// does it know about physics?
				if (reg->all_of<Components::physics2D>(entityA)){
					b = 0;
					for (auto entityB : view){
						if(b > v){
							// does that too know about physics?
							if (reg->all_of<Components::physics2D>(entityB)){
								// 2. Check if collision happened & // 3. React to collision.
								switch (isCollision(entityA, entityB, reg)){
									case 1: collideCircleCircle(entityA, entityB, reg);
										break;
									case 2: {
											auto* hitBoxA = &(reg->get<Components::hitBox>(entityA));
											auto* hitBoxB = &(reg->get<Components::hitBox>(entityB));
											if ((hitBoxA->type == Objects::HIT_BOX_CIRCLE) &&
												(hitBoxB->type == Objects::HIT_BOX_LINE)){
												collideCircleLine(entityA, entityB, reg);
											} else {
												if ((hitBoxA->type == Objects::HIT_BOX_LINE) &&
													(hitBoxB->type == Objects::HIT_BOX_CIRCLE)){
													collideCircleLine(entityB, entityA, reg);
												}
											}
										}
										break;
									case 3: collideLineLine(entityA, entityB, reg);
										break;
									default: {} // have a cup of tea as nothing happened
										break;
								}
							}
						}
						b++;
					}
				}
				v++;
			}
		}
		
		int outOfBounds(SDL_Rect rect, Objects::cGameObject* object){
			if (object->place.y < rect.y){ // far to the top
				return 3;
			} else {
				if ((object->place.x + object->place.w) > (rect.x + rect.w)){ // far to the right
					return 2;
				} else {
					if ((object->place.y + object->place.h) > (rect.y + rect.h)){ // far to the bottom
					// if ((object->place.y) > (rect.y + rect.h)){ // far to the bottom
						return 4;
					} else {
						if (object->place.x < rect.x){ // far to the left
							return 1;
						}
					}
				}
			}
			return 0; // commenting this line makes the screen GO WILD
		}
	};
}

#endif
