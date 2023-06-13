#ifndef CGAMEOBJECT_ENTITIES_H
#define CGAMEOBJECT_ENTITIES_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	/*
	typedef entt::entity CSE_Entity;
	
	namespace Entities {
		class cEntityComponentSystem {
		public:
			entt::registry registry;
			
			cEntityComponentSystem(){};
			
			~cEntityComponentSystem(){};
			
			// ENTITIES
			CSE_Entity addEntity(std::string name){
				CSE_Entity entity = registry.create();
				auto &base = registry.emplace<Components::base>(entity);
				base.name = name;
				return entity;
			}
			
			void removeEntity(CSE_Entity id){
				registry.destroy(id);
			}
			
			void wipeEntities(){
				registry.clear();
			}
			
			/*
			// COMPONENTS
			template <typename C>
			decltype(auto) addComponent(C, CSE_Entity E){
				return registry.emplace<C>(E);
			}
			
			template <typename C>
			decltype(auto) getComponent(C, CSE_Entity E){
				return registry.get<C>(E);
			}
			
			template <typename C>
			void removeComponent(C, CSE_Entity E){
				registry.remove<C>(E);
			}
			
			// SYSTEMS
			
		};
	}*/
}

#endif
