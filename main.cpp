// include SDL Crystal Serpent Engine
#include "SDLCrystalSerpentEngine.h"

namespace TESTGAME {
	const float VELOCITY_MAX = 500.0f;
	
	typedef enum {
		WALK_LEFT,
		WALK_RIGHT
	} walk;
}

struct sOldBall{
	bool selected;
	int objectId;
	float x, y; // for the upper left corner of place rect 
	float vx, vy; // for speed, obviously
	float cx, cy; // for center of mass
	bool canBounce;
	float mass;
};

class cGame : public SDLCSE::cCrystalSerpentEngine{
public:
	sOldBall oldBall; 
	
	int mouseXold, mouseYold;
	float mouseVx, mouseVy;
	
	float Vxmin, Vymin, 
	      Vxmax, Vymax; // min and max speeds in meters
		  
	SDLCSE::CSE_Entity Player1;
	SDLCSE::CSE_Entity Player2;
	SDLCSE::CSE_Entity Valleyball;
	
	cGame(){
		// initialize it
		scaleX = 1.0f; // 1 meter per pixel on Ox axis
		scaleY = 1.0f; // 1 meter per pixel on Oy axis
		
		Vxmin = 0.0f; Vxmax = 400.0f;
		Vymin = 0.0f; Vymax = 400.0f;
		
		oldBall = { false, 0,
					0.0f, 0.0f,
					0.0f, 0.0f,
					0.0f, 0.0f,
					false, 0.0f};
	}
	
	// how we will react to user input
	void onKeyboardUpdate(SDL_Keycode keyCode, uint32_t keyStatus) override {
		switch(keyCode){
		case SDLK_SPACE:
			quit = true;
			break;
		case SDLK_ESCAPE:
			registry.clear();
			break;
		default: {
			auto view = registry.view<SDLCSE::Components::controlKeyBoard>();
			if (view.size() > 0){
				for (auto player : view){
					auto* control = &(getControlKeyBoardComponent(player));
					if (keyCode == control->KBControls[SDLCSE::Commands::KBCommand_Jump]){
						moveCharacter(player, SDLCSE::Commands::KBCommand_Jump, keyStatus);
					}
					if (keyCode == control->KBControls[SDLCSE::Commands::KBCommand_Left]){
						moveCharacter(player, SDLCSE::Commands::KBCommand_Left, keyStatus);
					}
					if (keyCode == control->KBControls[SDLCSE::Commands::KBCommand_Right]){
						moveCharacter(player, SDLCSE::Commands::KBCommand_Right, keyStatus);
					}
				}
			}
			}
			break;
		}
	}
	
	void onMouseUpdate(SDLCSE::MouseController* Mouse) override {
		std::string logmessage = "";

		if (Mouse->eventDown){
			if (Mouse->leftButton){
				if (!oldBall.selected){
					for (i = objects.size()-1; i >= 0; i--){
						if (objects[i]->type == SDLCSE::Objects::GAME_OBJECT_TYPE_PROPS){
							if ((Mouse->cursor.x >= objects[i]->place.x) 
								&& (Mouse->cursor.x <= objects[i]->place.x + objects[i]->place.w)
								&& (Mouse->cursor.y >= objects[i]->place.y) 
								&& (Mouse->cursor.y <= objects[i]->place.y + objects[i]->place.h)){
								oldBall.selected = true;
								oldBall.objectId = i;
								oldBall.cx = objects[i]->centerOfMass.x;
								oldBall.cy = objects[i]->centerOfMass.y;
								oldBall.x = objects[i]->place.x;
								oldBall.y = objects[i]->place.y;
								oldBall.vx = objects[i]->velocity[0];
								oldBall.vy = objects[i]->velocity[1];
								oldBall.canBounce = objects[i]->canBounce;
								oldBall.mass = objects[i]->mass;
								
								objects[i]->velocity[0] = 0.0f;
								objects[i]->velocity[1] = 0.0f;
								objects[i]->canBounce = false;
								objects[i]->mass = 9000;
								i = -1;
							}
						}
					}
					if (!oldBall.selected){
						addSingleRandomCircle((float)(Mouse->cursor.x), (float)(Mouse->cursor.y));
					}
				}
			}
			if (Mouse->rightButton){}
		}
		
		if (Mouse->eventMove){
			if (oldBall.selected){
				objects[oldBall.objectId]->centerOfMass.x = (float)(Mouse->cursor.x);
				objects[oldBall.objectId]->centerOfMass.y = (float)(Mouse->cursor.y);
				// mouseXold = Mouse->cursor.x;
				// mouseYold = Mouse->cursor.y;
				GameObjectServitor->correctGeometry(objects[oldBall.objectId]);
			}
		}
		
		if (Mouse->eventUp){
			if (Mouse->leftButton){
				if (oldBall.selected){
					oldBall.selected = false;
					// objects[oldBall.objectId]->centerOfMass.x = oldBall.cx;
					// objects[oldBall.objectId]->centerOfMass.y = oldBall.cy;
					objects[oldBall.objectId]->velocity[0] = oldBall.vx;
					objects[oldBall.objectId]->velocity[1] = oldBall.vy;
					objects[oldBall.objectId]->canBounce = oldBall.canBounce;
					objects[oldBall.objectId]->mass = oldBall.mass;
					GameObjectServitor->correctGeometry(objects[oldBall.objectId]);
				}
			}
			if (Mouse->rightButton){}
		}
		
		// logging
		if (logmessage.size() > 0)
			LogSystem->log(logmessage);
	}
	
	void addSingleRandomCircle(const float x, const float y){
		SDL_FRect bSize;
		float bMass;
		
		//bMass = 1.0f + 0.1f * ((rand() % 10) - 5);
		bMass = 1.0f;
		bSize.w = 3 * 20 * bMass;
		bSize.h = 3 * 20 * bMass;
		bSize.x = x - 0.5f*bSize.w;
		bSize.y = y - 0.5f*bSize.h;
		
		SDLCSE::CSE_Entity entity = newProps("Ball Entity");
		auto* hitBox = &(getHitBoxComponent(entity));
		hitBox->type = SDLCSE::Objects::HIT_BOX_CIRCLE;
		hitBox->points.push_back({bSize.x + bSize.w/2, bSize.y + bSize.h/2});
		hitBox->radius = bSize.w/2;
		
		auto* physics2D = &(getPhysics2DComponent(entity));
		physics2D->centerOfMass = hitBox->points[0];
		physics2D->mass = 1.0f;
		physics2D->velocity = {0.0f, 0.0f};
		physics2D->acceleration = {-0.0f, 100.0f};
		physics2D->canMove = true;
		physics2D->canBounce = true;
		physics2D->canPush = true;
		
		auto* ballSprite = &(getSpriteComponent(entity));
		auto* ValleyballSprite = &(getSpriteComponent(Valleyball));
		ballSprite->sprite = ValleyballSprite->sprite;
		ballSprite->display = true;
		ballSprite->place = {
			bSize.x, 
			bSize.y, 
			bSize.w, 
			bSize.h
		};
		ballSprite->window = {0, 0, 20, 20};
		ballSprite->geometryCorrection = {ballSprite->place.x - physics2D->centerOfMass.x,
										  ballSprite->place.y - physics2D->centerOfMass.y};
		
		auto* Animation = &(addAnimationComponent(entity));
		Animation->add(SDLCSE::Objects::GAME_OBJECT_STATE_IDLE,
			new SDLCSE::animationFrames({0, 0}, {60, 0}, 20, 20, 4, true));
		Animation->set(SDLCSE::Objects::GAME_OBJECT_STATE_IDLE);
		Animation->start();
	}
	
	void onWorldUpdate() override{
		for (i = 0; i < objects.size(); i++){
			Physics2D->getTime(objects[i]);
			Physics2D->move(objects[i]);
			Physics2D->timeReset(objects[i]);
		}
		
		auto view = registry.view<SDLCSE::Components::physics2D>();
		for (auto entity : view){
			// limit speed of all entities
			auto* physicsComponent = &(getPhysics2DComponent(entity));
			float rel = SDLCSE::Math::length(physicsComponent->velocity) / TESTGAME::VELOCITY_MAX;
			if (rel > 1){
				physicsComponent->velocity.x = physicsComponent->velocity.x / rel;
				physicsComponent->velocity.y = physicsComponent->velocity.x / rel;
			}
			
			Physics2D->getTime(entity, &registry);
			Physics2D->move(entity, &registry);
			
			// make sure players don't fall into the abyss after jumps
			if (entity == Player1){
				auto* Player1Physics = &(getPhysics2DComponent(Player1));
				if ((Player1Physics->acceleration.y > 0.0f) && (Player1Physics->centerOfMass.y > (winHeight - 60))){
					Physics2D->unMove(Player1, &registry);
					Player1Physics->velocity.y = 0.0f;
					Player1Physics->acceleration.y = 0.0f;
					if (Player1Physics->velocity.x > 0.0f){
						setState(Player1, SDLCSE::Objects::GAME_OBJECT_STATE_RUN);
					}
					if (Player1Physics->velocity.x < 0.0f){
						setState(Player1, SDLCSE::Objects::GAME_OBJECT_STATE_WALK);
					}
					if (Player1Physics->velocity.x == 0.0f){
						setState(Player1, SDLCSE::Objects::GAME_OBJECT_STATE_IDLE);
					}
				}
			}
			
			Physics2D->timeReset(entity, &registry);
		}
		
		Physics2D->processCollisionsEvenDumber(&registry);
	}
	
	void moveCharacter(SDLCSE::CSE_Entity entity, int direction, uint32_t keyStatus){
		auto* base = &(getBaseComponent(entity));
		auto* physics = &(getPhysics2DComponent(entity));
		auto* animation = &(getAnimationComponent(entity));
		if (keyStatus == SDL_KEYDOWN){
			switch (direction){
				case SDLCSE::Commands::KBCommand_Jump:
					if (getState(entity) != SDLCSE::Objects::GAME_OBJECT_STATE_JUMP){
						setState(entity, SDLCSE::Objects::GAME_OBJECT_STATE_JUMP);
						animation->set(getState(entity));
						animation->start();
						physics->velocity.y = -100.0f;
						physics->acceleration.y = 100.0f;
					}
					break;
				case SDLCSE::Commands::KBCommand_Left:
					if (getState(entity) != SDLCSE::Objects::GAME_OBJECT_STATE_WALK){
						setState(entity, SDLCSE::Objects::GAME_OBJECT_STATE_WALK);
						animation->set(getState(entity));
						animation->start();
						physics->velocity.x = -100.0f;
					}
					break;
				case SDLCSE::Commands::KBCommand_Right:
					if (getState(entity) != SDLCSE::Objects::GAME_OBJECT_STATE_RUN){
						setState(entity, SDLCSE::Objects::GAME_OBJECT_STATE_RUN);
						animation->set(getState(entity));
						animation->start();
						physics->velocity.x = 100.0f;
					}
					break;
			}
		}
		if (keyStatus == SDL_KEYUP){
			switch (direction){
				case SDLCSE::Commands::KBCommand_Jump:
					if (getState(entity) == SDLCSE::Objects::GAME_OBJECT_STATE_JUMP){
						setState(entity, SDLCSE::Objects::GAME_OBJECT_STATE_IDLE);
						if (physics->velocity.y > 0.0f){
							physics->velocity.y = 0.0f;
						}
						physics->acceleration.y = 100.0f;
					}
					break;
				case SDLCSE::Commands::KBCommand_Left:
					physics->velocity.x = 0.0f;
					break;
				case SDLCSE::Commands::KBCommand_Right:
					physics->velocity.x = 0.0f;
					break;
						
			}
			setState(entity, SDLCSE::Objects::GAME_OBJECT_STATE_IDLE);
			animation->set(getState(entity));
			animation->start();
			//physics->velocity = {0.0f, 0.0f};
		}
	}
};

// example game built with SDLCSE
void addObjects(cGame* CSE){
	float winX = (float)(CSE->winX);
	float winY = (float)(CSE->winY);
	float winWidth = (float)(CSE->winWidth);
	float winHeight = (float)(CSE->winHeight);
	
	// add 8 circles of different colour
	CSE->Graphics->setPalette8(8);
	for (int i=0; i < 8; i++){
		CSE->ResourceManager->textures.push_back(new SDLCSE::cTexture(CSE->renderer, 32, 32, 0, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF));
		SDL_SetTextureBlendMode(CSE->ResourceManager->textures[i]->texture, SDL_BLENDMODE_BLEND);
		CSE->Graphics->DrawFilledCircle(CSE->ResourceManager->textures[i]->surface, 16, 16, 16, 1, CSE->Graphics->Palette[i]);
		CSE->ResourceManager->textures[i]->update();
	}
	
	SDL_FRect destRect0 = {0.0f, 0.0f, winWidth, winHeight};
	SDL_FRect destRect1 = {100.0f, 100.0f, 400.0f, 300.0f};
	SDL_FRect destRect2 = {400.0f, 100.0f, 750.0f, 300.0f};
	SDL_FRect ballPlace;
	SDL_FPoint point1, point2, point3;
	
	std::string fontSource = "./GameData/fonts/my8bit2.png";
	CSE->FontManager->load(fontSource);
	CSE->Graphics->getSymbols8bit();
	
	// Create objects
	SDLCSE::CSE_Entity entity;
	
	// 0. Border
	// Test Border
	entity = CSE->newProps("Border Test Entity");
	auto* hitBox = &(CSE->getHitBoxComponent(entity));
	hitBox->type = SDLCSE::Objects::HIT_BOX_LINE;
	hitBox->points.push_back({0.2f*winWidth, 0.4f*winHeight});
	hitBox->points.push_back({0.8f*winWidth, 0.6f*winHeight});
	
	auto* physics2D = &(CSE->getPhysics2DComponent(entity));
	physics2D->centerOfMass = {0.5f*(hitBox->points[1].x - hitBox->points[0].x), 
							   0.5f*(hitBox->points[1].y - hitBox->points[0].y)};
	physics2D->mass = 1.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {-0.0f, -0.0f};
	physics2D->canBounce = false;
	physics2D->canPush = true;
	physics2D->canMove = false;
	
	auto* sprite = &(CSE->getSpriteComponent(entity));
	sprite->display = true;
	sprite->place = {0.0f, 0.0f, winWidth, winHeight};
	sprite->window = {0, 0, CSE->winWidth, CSE->winHeight};
	
	CSE->Graphics->setPalette8(8);
	sprite->sprite = new SDLCSE::cTexture(CSE->renderer, winWidth, winHeight, 0, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	SDL_SetTextureBlendMode(sprite->sprite->texture, SDL_BLENDMODE_BLEND);
	CSE->Graphics->DrawLine(sprite->sprite->surface, hitBox->points[0].x, hitBox->points[0].y, hitBox->points[1].x, hitBox->points[1].y, CSE->Graphics->Palette[4]);
	sprite->sprite->update();
	sprite->geometryCorrection = {sprite->place.x - physics2D->centerOfMass.x,
								  sprite->place.y - physics2D->centerOfMass.y};
	//sprite->geometryCorrection = {0.0f, 0.0f};
	CSE->removeEntity(entity);
	
	// top
	entity = CSE->newProps("Border Top Entity");
	hitBox = &(CSE->getHitBoxComponent(entity));
	hitBox->type = SDLCSE::Objects::HIT_BOX_LINE;
	hitBox->points.push_back({0.0f, 0.0f});
	hitBox->points.push_back({winWidth, 0.0f});
	
	physics2D = &(CSE->getPhysics2DComponent(entity));
	physics2D->centerOfMass = {0.5f*(hitBox->points[1].x - hitBox->points[0].x), 
							   0.5f*(hitBox->points[1].y - hitBox->points[0].y)};
	physics2D->mass = 1.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {-0.0f, -0.0f};
	physics2D->canBounce = false;
	physics2D->canPush = true;
	
	CSE->removeSpriteComponent(entity);
	
	// right
	entity = CSE->newProps("Border Right Entity");
	hitBox = &(CSE->getHitBoxComponent(entity));
	hitBox->type = SDLCSE::Objects::HIT_BOX_LINE;
	hitBox->points.push_back({winWidth, 0.0f});
	hitBox->points.push_back({winWidth, winHeight});
	
	physics2D = &(CSE->getPhysics2DComponent(entity));
	physics2D->centerOfMass = {0.5f*(hitBox->points[1].x - hitBox->points[0].x), 
							   0.5f*(hitBox->points[1].y - hitBox->points[0].y)};
	physics2D->mass = 1.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {-0.0f, -0.0f};
	physics2D->canBounce = false;
	physics2D->canPush = true;
	physics2D->canMove = false;
	
	CSE->removeSpriteComponent(entity);
	
	// bottom
	entity = CSE->newProps("Border Bottom Entity");
	hitBox = &(CSE->getHitBoxComponent(entity));
	hitBox->type = SDLCSE::Objects::HIT_BOX_LINE;
	hitBox->points.push_back({winWidth, winHeight});
	hitBox->points.push_back({0.0f, winHeight});
	
	physics2D = &(CSE->getPhysics2DComponent(entity));
	physics2D->centerOfMass = {0.5f*(hitBox->points[1].x - hitBox->points[0].x), 
							   0.5f*(hitBox->points[1].y - hitBox->points[0].y)};
	physics2D->mass = 1.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {-0.0f, -0.0f};
	physics2D->canBounce = false;
	physics2D->canPush = true;
	physics2D->canMove = false;
	
	CSE->removeSpriteComponent(entity);
	
	// left
	entity = CSE->newProps("Border Left Entity");
	hitBox = &(CSE->getHitBoxComponent(entity));
	hitBox->type = SDLCSE::Objects::HIT_BOX_LINE;
	hitBox->points.push_back({0.0f, winHeight});
	hitBox->points.push_back({0.0f, 0.0f});
	
	physics2D = &(CSE->getPhysics2DComponent(entity));
	physics2D->centerOfMass = {0.5f*(hitBox->points[1].x - hitBox->points[0].x), 
							   0.5f*(hitBox->points[1].y - hitBox->points[0].y)};
	physics2D->mass = 1.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {-0.0f, -0.0f};
	physics2D->canBounce = false;
	physics2D->canPush = true;
	physics2D->canMove = false;
	
	CSE->removeSpriteComponent(entity);
	
	// 1. Sprite
	CSE->Valleyball = CSE->newProps("Volleyball Sprite Entity");
	CSE->removeHitBoxComponent(CSE->Valleyball);
	CSE->removePhysics2DComponent(CSE->Valleyball);
	
	auto* ValleyballSprite = &(CSE->getSpriteComponent(CSE->Valleyball));
	ValleyballSprite->sprite = new SDLCSE::cTexture("./GameData/img/Sprites.png", CSE->renderer, {0x00, 0x00, 0x00});
	ValleyballSprite->display = false;
	ValleyballSprite->place = {
		winWidth/4, 
		winHeight/3, 
		(float)(ValleyballSprite->sprite->getWidth()*3), 
		(float)(ValleyballSprite->sprite->getHeight()*3)
	};
	ValleyballSprite->window = {0, 0, CSE->winWidth, CSE->winHeight};
	
	// 2. Volleyballist
	CSE->Player1 = CSE->newProps("Player 1");
	
	auto* Player1Sprite = &(CSE->getSpriteComponent(CSE->Player1));
	Player1Sprite->sprite = ValleyballSprite->sprite;
	Player1Sprite->display = true;
	Player1Sprite->place = {
		winWidth/4, 
		winHeight - 30*3, 
		20.0f * 3, 
		30.0f * 3
	};
	Player1Sprite->window = {80, 0, 20, 30};
	
	hitBox = &(CSE->getHitBoxComponent(CSE->Player1));
	hitBox->type = SDLCSE::Objects::HIT_BOX_CIRCLE;
	hitBox->points.push_back({Player1Sprite->place.x + 10*3, Player1Sprite->place.y + 10*3});
	hitBox->radius = 30;
	
	physics2D = &(CSE->getPhysics2DComponent(CSE->Player1));
	physics2D->centerOfMass = hitBox->points[0];
	physics2D->mass = 10.0f;
	physics2D->velocity = {0.0f, 0.0f};
	physics2D->acceleration = {0.0f, 0.0f};
	physics2D->canMove = true;
	physics2D->canBounce = false;
	physics2D->canPush = true;
	
	Player1Sprite->geometryCorrection = {
		Player1Sprite->place.x - physics2D->centerOfMass.x,
		Player1Sprite->place.y - physics2D->centerOfMass.y
	};
	
	auto* Animation = &(CSE->addAnimationComponent(CSE->Player1));
	Animation->add(SDLCSE::Objects::GAME_OBJECT_STATE_IDLE,
		new SDLCSE::animationFrames({80, 0}, {80, 0}, 20, 30, 1, false));
	Animation->add(SDLCSE::Objects::GAME_OBJECT_STATE_WALK,
		new SDLCSE::animationFrames({140, 0}, {80, 0}, -20, 30, 10, true));
	Animation->add(SDLCSE::Objects::GAME_OBJECT_STATE_RUN,
		new SDLCSE::animationFrames({80, 0}, {140, 0}, 20, 30, 10, true));
	Animation->add(SDLCSE::Objects::GAME_OBJECT_STATE_JUMP,
		new SDLCSE::animationFrames({160, 0}, {160, 0}, 20, 30, 1, true));
	
	Animation->set(SDLCSE::Objects::GAME_OBJECT_STATE_IDLE);
	Animation->start();
	
	auto* Control = &(CSE->addControlKeyBoardComponent(CSE->Player1));
	Control->KBControls[SDLCSE::Commands::KBCommand_Jump] = SDLK_UP;
	Control->KBControls[SDLCSE::Commands::KBCommand_Left] = SDLK_LEFT;
	Control->KBControls[SDLCSE::Commands::KBCommand_Right] = SDLK_RIGHT;
}


// Mainly this is a testing program to see what's working in Crystal Serpent Engine already.
int main(int argc, char** args) {
	// prepare engine initialization
	int winX = 150;
	int winY = 150;
	int winWidth = 1200;
	int winHeight = 720;
	std::string winTitle = SDLCSE::GameEngineName + " " + SDLCSE::GameEngineVersion;

	// THIS IS THE ENGINE ENTRY POINT
	cGame Game;
	Game.Init(winTitle, winX, winY, winWidth, winHeight, 0);
	
	// Fill the game world with objects
	addObjects(&Game);
	
	// Start the game cycle and enjoy the magic done by Crystal Serpent Engine
	Game.Start();
	
	return 0;
}
