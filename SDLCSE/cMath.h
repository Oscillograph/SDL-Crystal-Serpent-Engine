#ifndef CMATH_H
#define CMATH_H

#include "./common.h"

// =============================================
// The pattern behind developing this library is
// that when a physics engine or other part of 
// the game engine needs a math function, then
// it is implemented here.
// That's it, no glm.
// ---------------------------------------------

namespace SDLCSE{
	// =============================================
	// Operators overloads
	// ---------------------------------------------
	
	// add vector to vector
	SDL_FPoint operator+(SDL_FPoint lhs, SDL_FPoint rhs){
		float x = lhs.x + rhs.x;
		float y = lhs.y + rhs.y;
		return {x, y};
	}
	
	SDL_FPoint operator-(SDL_FPoint lhs, SDL_FPoint rhs){
		float x = lhs.x - rhs.x;
		float y = lhs.y - rhs.y;
		return {x, y};
	}
	
	// multiply vector by a scalar / ver.1
	SDL_FPoint operator*(float lhs, SDL_FPoint rhs){
		float x = rhs.x*lhs;
		float y = rhs.y*lhs;
		return {x, y};
	}
	
	// multiply vector by a scalar / ver.2
	SDL_FPoint operator*(SDL_FPoint lhs, float rhs){
		float x = lhs.x*rhs;
		float y = lhs.y*rhs;
		return {x, y};
	}
	
	namespace Math{
		typedef enum {
			VECTORS_COLLINEAR,
			VECTORS_COLLINEAR_OPPOSITE,
			VECTORS_PERPENDICULAR,
			VECTORS_ANGLE_SHARP,
			VECTORS_ANGLE_BLUNT,
		} VECTORS_TEST;

		// =============================================
		// Types
		// ---------------------------------------------

		// typedef std::pair<SDL_FPoint, SDL_FPoint> CSE_Line;
		// typedef std::vector<SDL_FPoint> CSE_Line;
		// typedef SDL_FPoint CSE_Line[2];
		struct CSE_Line{
			SDL_FPoint a, b;
		};

		struct CSE_Complex{
			float real, imag;
		};
		
		struct CSE_FPoint{
			float x;
			float y;
			
			// add vector to vector
			CSE_FPoint& operator+(CSE_FPoint rhs){
				x += rhs.x;
				y += rhs.y;
				return *this;
			}
			
			// multiply vector by a scalar / ver.1
			CSE_FPoint& operator*(float lhs){
				x = x*lhs;
				y = y*lhs;
				return *this;
			}
			
			// multiply vector by a scalar / ver.2
			/*CSE_FPoint& operator*(float rhs){
				x = x*rhs;
				y = y*rhs;
				return *this;
			}*/
		};

		// =============================================
		// Functions
		// ---------------------------------------------
		
		// just a square
		float sqr(float number){
			return number*number;
		}
		
		// scalar product of vectors
		float dotProduct(SDL_FPoint A, SDL_FPoint B){
			return (A.x*B.x + A.y*B.y);
		}
		
		// distance from point A to point B
		float distance(SDL_FPoint A, SDL_FPoint B){
			float dx = B.x - A.x;
			float dy = B.y - A.y;
			return sqrtf(dx*dx + dy*dy);
		}
		
		// just a sum of the same squares as in distance function
		float distance2(SDL_FPoint A, SDL_FPoint B){
			float dx = B.x - A.x;
			float dy = B.y - A.y;
			return (dx*dx + dy*dy);
		}
		
		// length of a vector A
		float length(SDL_FPoint A){
			return distance({0.0f, 0.0f}, A);
		}
		
		// find out how vectors A and B are related to each other
		int testVectors(SDL_FPoint A, SDL_FPoint B){
			float ab = dotProduct(A, B);
			float AB = length(A)*length(B);
			if (ab == AB){
				return VECTORS_COLLINEAR;
			}
			if (ab == -AB){
				return VECTORS_COLLINEAR_OPPOSITE;
			}
			if (ab == 0){
				return VECTORS_PERPENDICULAR;
			}
			if (ab > 0){
				return VECTORS_ANGLE_SHARP;
			}
			if (ab < 0){
				return VECTORS_ANGLE_BLUNT;
			}
			return -1; // Terrible, terrible outcome 
		}
		
		// normalize vector A
		void normalize(SDL_FPoint &A){
			float norm = length(A);
			A.x = A.x / norm;
			A.y = A.y / norm;
		}
		
		SDL_FPoint findNorm(SDL_FPoint A){
			float len = length(A);
			A.x = A.x / len;
			A.y = A.y / len;
			return A;
		}
		
		// find height of point A relative to the plane defined by "norm" vector
		float findHeight(SDL_FPoint A, SDL_FPoint norm){
			return dotProduct(A, norm);
		}
		
		// project vector A on vector "base"
		SDL_FPoint project(SDL_FPoint A, SDL_FPoint base){
			return (dotProduct(A, base) * findNorm(base));
		}
		
		// ----------------------------------------------
		// we don't use vector products in 2D, by the way
		// ----------------------------------------------
		
		
		// find tangencial vector to a line defined by points A and B
		SDL_FPoint findTangencial(SDL_FPoint A, SDL_FPoint B){
			return {B.x - A.x, B.y - A.y};
		}
		
		// find normal vector to a line defined by points A and B
		SDL_FPoint findNormal(SDL_FPoint A, SDL_FPoint B){
			SDL_FPoint tangencial = findTangencial(A, B);
			return {-tangencial.y, tangencial.x};
		}
		
		// find the center of a line defined by points A and B
		SDL_FPoint findCenter(SDL_FPoint A, SDL_FPoint B){
			return {(B.x - A.x)/2, (B.y - A.y)/2};
		}
		
		// get a line which starts at the center of a given and aimed with the normal vector 
		CSE_Line findNormalToCenter(SDL_FPoint A, SDL_FPoint B){
			SDL_FPoint normal = findNormal(A, B);
			SDL_FPoint center = findCenter(A, B);
			normal.x += center.x;
			normal.y += center.y;
			
			CSE_Line out; 
			out.a = center;
			out.b = normal;
			return out;
			// return std::make_pair(center, normal);
		}
	}
}

#endif
