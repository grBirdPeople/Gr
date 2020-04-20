#ifndef		_H_GRPARTICLE_ATTRIBUTE_
#define		_H_GRPARTICLE_ATTRIBUTE_

#include	"grV2.h"


struct grSParticleAttribute
{
	grSParticleAttribute( void )
		: Position	( grV2f() )
		, Velocity	( grV2f( 0.0f, -1.0f ) )
		, Lifetime	( 2.0f )		
	{}

	grV2f	Position;
	grV2f	Velocity;
	float	Lifetime;
};

#endif	// _H_GRPARTICLE_ATTRIBUTE_
