#ifndef CSOUNDS_H
#define CSOUNDS_H

#include "./common.h"

namespace SDLCSE {
	/* ------------------
	  This is an audio management class
	  ------------------ */
	class cSounds {
	public:
		uint32_t* soundBuffer;
		uint32_t NiquistFrequency;
		uint32_t chunkSize;
		uint32_t* leftChannel;
		uint32_t* rightChannel;
		
		int i, j, k, l; // indexes for cycles
		
		cResourceManager* ResourceManager;
		
		cSounds() {}
		~cSounds() {}
		
		void loadSound() {}
		void playSound() {}
		void saveSound() {}
		void synthesizer() {}
	};
}

#endif
