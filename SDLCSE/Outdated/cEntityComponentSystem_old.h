#ifndef CENTITYCOMPONENTSYSTEM_H
#define CENTITYCOMPONENTSYSTEM_H

// SDLCSE
#include "./common.h"
#include "./cGameObject_Components.h"

namespace SDLCSE {
	// --------------------------------------------------------
	// Global vars
	const int entities_max = 10000;

	// --------------------------------------------------------
	
	class cEntityComponentSystem {
	public:
		int entities[entities_max];
		int entities_now;
		int entities_id_max;
		Components_Registry registry;
		
		cEntityComponentSystem(){
			int objects_now = -1;
			int objects_id_max = -1;
			for (int v = 0; v < entities_max; v++){
				entities[v] = -1;
			}
		};
		
		~cEntityComponentSystem(){};
		
		// ENTITIES
		int addEntity(){
			entities_now++;
			entities_id_max++;
			entities[entities_now] = entities_id_max;
			return entities_id_max;
		}
		
		void removeEntity(int id){
			entities_now--;
			while (id < (entities_max - 1)) {
				entities[id] = entities[id + 1];
				id++;
			}
			entities[id] = -1;
		}
		
		// COMPONENTS
		
		// SYSTEMS
	};
}

#endif
