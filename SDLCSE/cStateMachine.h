#ifndef CSTATEMACHINE_H
#define CSTATEMACHINE_H

// SDLCSE
#include "./common.h"

namespace SDLCSE {
	namespace StateConditions {
		struct condition1{
			int a, b;
		};
	}
	
	struct CSE_States {
		std::unordered_map<int, int> states;
		
		add(int state){
			states.emplace(states.size(), state);
		}
		
		addEmpty(){
			states.emplace(states.size(), Objects::GAME_OBJECT_STATE_NONE);
		}
		
		
	};
	
	class cState {
	public:
		virtual void onEnter() = 0;
		virtual void onExit() = 0;
		virtual void change() = 0;
	};
	
	class cLocalStateMachine : public cState {
	public:
		abstractStateMachine();
		~abstractStateMachine();
		
	private:
		int currentState;
	};
	
	// auto* stack = &(cEntityComponentSystem::getStateStackComponent(entity));
	
	struct CSE_StateStack {
		int stack[StateStackMax];
		int stack_size;
		int current;
		
		CSE_StateStack(){
			for (int i = 0; i < StateStackMax; i++){
				stack[i] = Objects::GAME_OBJECT_STATE_IDLE; // default state
			}
			current = 0;
			stack_size = 0;
		}
		
		int size(){
			return stack_size; // yeah, we've got an int variable just to track size of the stack
		}
		
		bool push(int state){
			if (stack_size < StateStackMax){
				stack[current] = state; // add new element
				current = stack_size;
				stack_size++;
				return true; // push successful
			}
			return false; // nowhere to push
		}
		
		void force(int state){
			if (!push(state)){
				for (int i = 0; i < stack_size-1; i++){
					stack[i] = stack[i+1];
				}
				stack[stack_size-1] = state;
				current = stack_size - 1;
			}
		}
		
		int read(){
			return stack[current];
		}
		
		bool pop(int &state){
			if (stack_size > 0){
				state = stack[current]; // return the popped element
				stack_size--;
				current = stack_size;
				return true; // pop successful
			} else {
				current = 0;
				state = stack[current]; // return default, because the engine needs positive states
				return false; // nothing to pop
			}
		}
	};
	
	class cStateMachine {
	public:
		CSE_StateStack statesComponent; // well, now we can think about making this machine as a component for an entity
		
		cStateMachine (entt::registry* reg){
			engine_registry = reg;
		}
		~cStateMachine () {};
		
		CSE_Entity addState(){
			return states_registry->create();
		};
		
		void setState(){}
		
		void processState(){}
		
		decltype(auto) addCondition(CSE_Entity state){
			return &(states_registry->emplace<StateConditions::condition1>(state));
		};
		
	private:
		entt::registry* states_registry;
		entt::registry* engine_registry;
	};
}

#endif
