#ifndef		_H_GRPARTICLE_
#define		_H_GRPARTICLE_

#include	"grV2.h"


struct grSParticle
{
	grV2f	Position;
	grV2f	Direction;

	float	Lifetime;

	float	Spd,
			SpdMod;

	uInt	SpdOsc;		

	// insert Color
	//grV2f	Position;
	//grV2f	Velocity;
	//float	Lifetime;
};

#endif	// _H_GRPARTICLE_
