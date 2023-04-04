#ifndef COLORPALETTE_H
#define COLORPALETTE_H

namespace SDLCSE{
	namespace Colors {
		// --- The type which is essential for all graphics ---
		struct RGBA{
			int r; // red
			int g; // green
			int b; // blue
			int a; // alpha
		};
		
		// --- 9 tones of eight basic RGB colors ---
		// DARKER COLORS
		const int RGBA_Tone0 = 0x1F;
		const RGBA RGBA_Black0 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red0 = {RGBA_Tone0, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue0 = {0x00, 0x00, RGBA_Tone0, 0xFF};
		const RGBA RGBA_Green0 = {0x00, RGBA_Tone0, 0x00, 0xFF};
		const RGBA RGBA_Yellow0 = {RGBA_Tone0, RGBA_Tone0, 0x00, 0xFF};
		const RGBA RGBA_Purple0 = {RGBA_Tone0, 0x00, RGBA_Tone0, 0xFF};
		const RGBA RGBA_Cyan0 = {0x00, RGBA_Tone0, RGBA_Tone0, 0xFF};
		const RGBA RGBA_White0 = {RGBA_Tone0, RGBA_Tone0, RGBA_Tone0, 0xFF};
		
		const int RGBA_Tone1 = 0x3F;
		const RGBA RGBA_Black1 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red1 = {RGBA_Tone1, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue1 = {0x00, 0x00, RGBA_Tone1, 0xFF};
		const RGBA RGBA_Green1 = {0x00, RGBA_Tone1, 0x00, 0xFF};
		const RGBA RGBA_Yellow1 = {RGBA_Tone1, RGBA_Tone1, 0x00, 0xFF};
		const RGBA RGBA_Purple1 = {RGBA_Tone1, 0x00, RGBA_Tone1, 0xFF};
		const RGBA RGBA_Cyan1 = {0x00, RGBA_Tone1, RGBA_Tone1, 0xFF};
		const RGBA RGBA_White1 = {RGBA_Tone1, RGBA_Tone1, RGBA_Tone1, 0xFF};
		
		const int RGBA_Tone2 = 0x5F;
		const RGBA RGBA_Black2 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red2 = {RGBA_Tone2, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue2 = {0x00, 0x00, RGBA_Tone2, 0xFF};
		const RGBA RGBA_Green2 = {0x00, RGBA_Tone2, 0x00, 0xFF};
		const RGBA RGBA_Yellow2 = {RGBA_Tone2, RGBA_Tone2, 0x00, 0xFF};
		const RGBA RGBA_Purple2 = {RGBA_Tone2, 0x00, RGBA_Tone2, 0xFF};
		const RGBA RGBA_Cyan2 = {0x00, RGBA_Tone2, RGBA_Tone2, 0xFF};
		const RGBA RGBA_White2 = {RGBA_Tone2, RGBA_Tone2, RGBA_Tone2, 0xFF};
		
		const int RGBA_Tone3 = 0x7F;
		const RGBA RGBA_Black3 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red3 = {RGBA_Tone3, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue3 = {0x00, 0x00, RGBA_Tone3, 0xFF};
		const RGBA RGBA_Green3 = {0x00, RGBA_Tone3, 0x00, 0xFF};
		const RGBA RGBA_Yellow3 = {RGBA_Tone3, RGBA_Tone3, 0x00, 0xFF};
		const RGBA RGBA_Purple3 = {RGBA_Tone3, 0x00, RGBA_Tone3, 0xFF};
		const RGBA RGBA_Cyan3 = {0x00, RGBA_Tone3, RGBA_Tone3, 0xFF};
		const RGBA RGBA_White3 = {RGBA_Tone3, RGBA_Tone3, RGBA_Tone3, 0xFF};
		
		// GENERAL COLORS
		const int RGBA_Tone4 = 0x9F;
		const RGBA RGBA_Black4 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red4 = {RGBA_Tone4, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue4 = {0x00, 0x00, RGBA_Tone4, 0xFF};
		const RGBA RGBA_Green4 = {0x00, RGBA_Tone4, 0x00, 0xFF};
		const RGBA RGBA_Yellow4 = {RGBA_Tone4, RGBA_Tone4, 0x00, 0xFF};
		const RGBA RGBA_Purple4 = {RGBA_Tone4, 0x00, RGBA_Tone4, 0xFF};
		const RGBA RGBA_Cyan4 = {0x00, RGBA_Tone4, RGBA_Tone4, 0xFF};
		const RGBA RGBA_White4 = {RGBA_Tone4, RGBA_Tone4, RGBA_Tone4, 0xFF};
		
		// LIGHTER COLORS
		const int RGBA_Tone5 = 0xAF;
		const RGBA RGBA_Black5 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red5 = {RGBA_Tone5, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue5 = {0x00, 0x00, RGBA_Tone5, 0xFF};
		const RGBA RGBA_Green5 = {0x00, RGBA_Tone5, 0x00, 0xFF};
		const RGBA RGBA_Yellow5 = {RGBA_Tone5, RGBA_Tone5, 0x00, 0xFF};
		const RGBA RGBA_Purple5 = {RGBA_Tone5, 0x00, RGBA_Tone5, 0xFF};
		const RGBA RGBA_Cyan5 = {0x00, RGBA_Tone5, RGBA_Tone5, 0xFF};
		const RGBA RGBA_White5 = {RGBA_Tone5, RGBA_Tone5, RGBA_Tone5, 0xFF};
		
		const int RGBA_Tone6 = 0xBF;
		const RGBA RGBA_Black6 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red6 = {RGBA_Tone6, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue6 = {0x00, 0x00, RGBA_Tone6, 0xFF};
		const RGBA RGBA_Green6 = {0x00, RGBA_Tone6, 0x00, 0xFF};
		const RGBA RGBA_Yellow6 = {RGBA_Tone6, RGBA_Tone6, 0x00, 0xFF};
		const RGBA RGBA_Purple6 = {RGBA_Tone6, 0x00, RGBA_Tone6, 0xFF};
		const RGBA RGBA_Cyan6 = {0x00, RGBA_Tone6, RGBA_Tone6, 0xFF};
		const RGBA RGBA_White6 = {RGBA_Tone6, RGBA_Tone6, RGBA_Tone6, 0xFF};
		
		const int RGBA_Tone7 = 0xDF;
		const RGBA RGBA_Black7 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red7 = {RGBA_Tone7, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue7 = {0x00, 0x00, RGBA_Tone7, 0xFF};
		const RGBA RGBA_Green7 = {0x00, RGBA_Tone7, 0x00, 0xFF};
		const RGBA RGBA_Yellow7 = {RGBA_Tone7, RGBA_Tone7, 0x00, 0xFF};
		const RGBA RGBA_Purple7 = {RGBA_Tone7, 0x00, RGBA_Tone7, 0xFF};
		const RGBA RGBA_Cyan7 = {0x00, RGBA_Tone7, RGBA_Tone7, 0xFF};
		const RGBA RGBA_White7 = {RGBA_Tone7, RGBA_Tone7, RGBA_Tone7, 0xFF};
		
		const int RGBA_Tone8 = 0xFF;
		const RGBA RGBA_Black8 = {0x00, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Red8 = {RGBA_Tone8, 0x00, 0x00, 0xFF};
		const RGBA RGBA_Blue8 = {0x00, 0x00, RGBA_Tone8, 0xFF};
		const RGBA RGBA_Green8 = {0x00, RGBA_Tone8, 0x00, 0xFF};
		const RGBA RGBA_Yellow8 = {RGBA_Tone8, RGBA_Tone8, 0x00, 0xFF};
		const RGBA RGBA_Purple8 = {RGBA_Tone8, 0x00, RGBA_Tone8, 0xFF};
		const RGBA RGBA_Cyan8 = {0x00, RGBA_Tone8, RGBA_Tone8, 0xFF};
		const RGBA RGBA_White8 = {RGBA_Tone8, RGBA_Tone8, RGBA_Tone8, 0xFF};
	}
}
#endif
