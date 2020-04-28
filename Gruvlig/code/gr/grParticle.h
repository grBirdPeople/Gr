#ifndef		_H_GRPARTICLE_
#define		_H_GRPARTICLE_

#include	"grV2.h"
#include	<SFML/Graphics/Color.hpp>


struct grSParticle
{
	sf::Color Color;

	grV2f	Position;
	grV2f	Direction;

	float	Spd,
			SpdMod;

	float	Lifetime;

	bool	SpdOsc;

	// insert Color
	//grV2f	Position;
	//grV2f	Velocity;
	//float	Lifetime;
};

#endif	// _H_GRPARTICLE_
