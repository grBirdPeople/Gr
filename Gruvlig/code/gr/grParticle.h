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


struct grSParticle
{
	sf::Color Color;

	grV2f	Position;
	grV2f	Direction;

	float	Spd,
			SpdMod;

	float	Lifetime;

	bool	bSpdOsc;

	// insert Color
	//grV2f	Position;
	//grV2f	Velocity;
	//float	Lifetime;
};

#endif	// _H_GRPARTICLE_
