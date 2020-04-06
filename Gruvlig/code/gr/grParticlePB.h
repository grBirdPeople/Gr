#ifndef		_H_GRPARTICLEPB_
#define		_H_GRPARTICLEPB_

#include	"grV2.h"


struct grParticlePB
{
	grParticlePB( void )
	{
		Mass = 1.0f;
	}

	grV2f Position;
	grV2f Acceleration;
	grV2f Velocity;
	float Mass;
};

#endif	// _H_GRPARTICLEPB_