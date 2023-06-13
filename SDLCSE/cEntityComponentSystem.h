#ifndef CENTITYCOMPONENTSYSTEM_H
#define CENTITYCOMPONENTSYSTEM_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	typedef entt::entity CSE_Entity;
	
	class cEntityComponentSystem {
	public:
		entt::registry registry;
		
		cEntityComponentSystem(){};
		
		~cEntityComponentSystem(){};
		
		// ===========================================================
		// ENTITIES
		//
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
		
		// ===========================================================
		// COMPONENTS
		//
		// base
		bool hasBaseComponent(CSE_Entity entity){
			return registry.all_of<Components::base>(entity);
		}
		decltype(auto) addBaseComponent(CSE_Entity entity){
			return registry.emplace<Components::base>(entity);
		}
		decltype(auto) getBaseComponent(CSE_Entity entity){
			return registry.get<Components::base>(entity);
		}
		void removeBaseComponent(CSE_Entity entity){
			registry.remove<Components::base>(entity);
		}
		
		// text
		bool hasTextComponent(CSE_Entity entity){
			return registry.all_of<Components::text>(entity);
		}
		decltype(auto) addTextComponent(CSE_Entity entity){
			return registry.emplace<Components::text>(entity);
		}
		decltype(auto) getTextComponent(CSE_Entity entity){
			return registry.get<Components::text>(entity);
		}
		void removeTextComponent(CSE_Entity entity){
			registry.remove<Components::text>(entity);
		}
		
		// group
		bool hasGroupComponent(CSE_Entity entity){
			return registry.all_of<Components::group>(entity);
		}
		decltype(auto) addGroupComponent(CSE_Entity entity){
			return registry.emplace<Components::group>(entity);
		}
		decltype(auto) getGroupComponent(CSE_Entity entity){
			return registry.get<Components::group>(entity);
		}
		void removeGroupComponent(CSE_Entity entity){
			registry.remove<Components::group>(entity);
		}
		
		// hitBox
		bool hasHitBoxComponent(CSE_Entity entity){
			return registry.all_of<Components::hitBox>(entity);
		}
		decltype(auto) addHitBoxComponent(CSE_Entity entity){
			return registry.emplace<Components::hitBox>(entity);
		}
		decltype(auto) getHitBoxComponent(CSE_Entity entity){
			return registry.get<Components::hitBox>(entity);
		}
		void removeHitBoxComponent(CSE_Entity entity){
			registry.remove<Components::hitBox>(entity);
		}
		
		// sprite
		bool hasSpriteComponent(CSE_Entity entity){
			return registry.all_of<Components::sprite>(entity);
		}
		decltype(auto) addSpriteComponent(CSE_Entity entity){
			return registry.emplace<Components::sprite>(entity);
		}
		decltype(auto) getSpriteComponent(CSE_Entity entity){
			return registry.get<Components::sprite>(entity);
		}
		void removeSpriteComponent(CSE_Entity entity){
			registry.remove<Components::sprite>(entity);
		}
		
		// animation
		bool hasAnimationComponent(CSE_Entity entity){
			return registry.all_of<Components::animation>(entity);
		}
		decltype(auto) addAnimationComponent(CSE_Entity entity){
			return registry.emplace<Components::animation>(entity);
		}
		decltype(auto) getAnimationComponent(CSE_Entity entity){
			return registry.get<Components::animation>(entity);
		}
		void removeAnimationComponent(CSE_Entity entity){
			registry.remove<Components::animation>(entity);
		}
		
		// physics2D
		bool hasPhysics2DComponent(CSE_Entity entity){
			return registry.all_of<Components::physics2D>(entity);
		}
		decltype(auto) addPhysics2DComponent(CSE_Entity entity){
			return registry.emplace<Components::physics2D>(entity);
		}
		decltype(auto) getPhysics2DComponent(CSE_Entity entity){
			return registry.get<Components::physics2D>(entity);
		}
		void removePhysics2DComponent(CSE_Entity entity){
			registry.remove<Components::physics2D>(entity);
		}
		
		// controlKeyBoard
		bool hasControlKeyBoardComponent(CSE_Entity entity){
			return registry.all_of<Components::controlKeyBoard>(entity);
		}
		decltype(auto) addControlKeyBoardComponent(CSE_Entity entity){
			return registry.emplace<Components::controlKeyBoard>(entity);
		}
		decltype(auto) getControlKeyBoardComponent(CSE_Entity entity){
			return registry.get<Components::controlKeyBoard>(entity);
		}
		void removeControlKeyBoardComponent(CSE_Entity entity){
			registry.remove<Components::controlKeyBoard>(entity);
		}
		
		// controlMouse
		bool hasControlMouseComponent(CSE_Entity entity){
			return registry.all_of<Components::controlMouse>(entity);
		}
		decltype(auto) addControlMouseComponent(CSE_Entity entity){
			return registry.emplace<Components::controlMouse>(entity);
		}
		decltype(auto) getControlMouseComponent(CSE_Entity entity){
			return registry.get<Components::controlMouse>(entity);
		}
		void removeControlMouseComponent(CSE_Entity entity){
			registry.remove<Components::controlMouse>(entity);
		}
		
		// stateStack
		bool hasStateStackComponent(CSE_Entity entity){
			return registry.all_of<Components::stateStack>(entity);
		}
		decltype(auto) addStateStackComponent(CSE_Entity entity){
			return registry.emplace<Components::stateStack>(entity);
		}
		decltype(auto) getStateStackComponent(CSE_Entity entity){
			return registry.get<Components::stateStack>(entity);
		}
		void removeStateStackComponent(CSE_Entity entity){
			registry.remove<Components::stateStack>(entity);
		}
		
		// ===========================================================
		// SYSTEMS
		//
		
	};
}

#endif
