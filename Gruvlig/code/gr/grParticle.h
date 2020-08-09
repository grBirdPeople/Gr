#ifndef		_H_GRPARTICLE_
#define		_H_GRPARTICLE_

#include	<SFML/Graphics/Color.hpp>

#include	"grV2.h"


//struct grSColor
//{
//	grSColor( void )
//	{
//		R = 255;
//		G = 255;
//		B = 255;
//		A = 255;
//	}
//	grSColor( const intU r, const intU g, const intU b, const intU a )
//	{
//		R = r;
//		G = g;
//		B = b;
//		A = a;
//	}
//
//	intU R, G, B, A;
//};


struct grSParticleArr
{
	struct SCol { float R = 0.0f, G = 0.0f, B = 0.0f, A = 0.0f; };

	sf::Color	Color;
	SCol		ColRate;
	SCol		ColAccumulator;

	grV2f		Pos;
	grV2f		Dir;

	float		Spd,
				SpdMod;

	float		Life;

	bool		bSpdOsc;
};

#endif	// _H_GRPARTICLE_
