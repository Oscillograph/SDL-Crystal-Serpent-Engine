# SDL-Crystal-Serpent-Engine
A simple 2D game engine in C++ with SDL2

The status of the engine currently is "work-in-progress".
Planned usage is simple:

1. Include the engine header in your main file like this:
#include "SDLCrystalSerpentEngine.h"

2. Start working with the engine by defining its main object:
SDLCSE::cCrystalSerpentEngine* CSE = new SDLCSE::cCrystalSerpentEngine(winTitle, winX, winY, winWidth, winHeight, numberOfLayers);

4. (WiP) Write your own void-functions for IntroState(), MenuState(), GameState(), ScoresState(), CreditsState() and assign them to the engine function pointers with corresponding names.

5. (WiP) Add game objects like text messages, background images, doodads, props, actors, etc., and set their properties and behaviour.

6. (WiP) Define behaviour of the game cycle by directly setting up and calling methods of the engine and its subsystems (graphics, sounds, physics, etc.).

7. Start the game cycle by:
CSE->Start();

8. Have a cup of tasty tea.
