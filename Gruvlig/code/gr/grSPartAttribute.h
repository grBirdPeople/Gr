#ifndef		_H_GRPARTICLE_ATTRIBUTE_
#define		_H_GRPARTICLE_ATTRIBUTE_

#include	"grV2.h"


struct grSParticleAttribute
{
	grSParticleAttribute( void )
		: Position			( grV2f( 320.0f, 180.0f ) )	// TODO: Actually get screen origo
		, MinMaxDirInDeg	( grV2f( 315.0f, 45.0f ) )
		, MinMaxSpeed		( grV2f( 16.0f, 64.0f ) )
		, MinMaxLife		( grV2f( 2.0f, 4.0f ) )
		, PosOffsetRadius	( 20.0f )
	{}

	grV2f	Position;
	grV2f	MinMaxDirInDeg;
	grV2f	MinMaxSpeed;
	grV2f	MinMaxLife;

	float	PosOffsetRadius;
};

#endif	// _H_GRPARTICLE_ATTRIBUTE_
