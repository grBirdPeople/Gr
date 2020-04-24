#ifndef		_H_GRPARTICLE_ATTRIBUTE_
#define		_H_GRPARTICLE_ATTRIBUTE_

#include	"grV2.h"


struct grSParticleAttribute
{
	grSParticleAttribute( void )
		: EmitrPos				( grV2f( 320.0f, 180.0f ) )	// TODO: Actually get screen origo
		, EmitrDir				( grV2f( 0.0f, -1.0 ) )
		, PartMinMaxRotInDeg	( grV2f( 315.0f, 45.0f ) )
		, PartMinMaxSpd			( grV2f( 16.0f, 64.0f ) )
		, PartMinMaxLife		( grV2f( 2.0f, 4.0f ) )

		, PartMinMaxModSpd		( grV2f() )

		, EmitrRotInDeg			( 0.0f )
		, PartPosOffsetRadius	( 20.0f )
		, PartRotInDeg			( 0.0f )

		, PartModSpd			( 1.0f )
	{}

	grV2f	EmitrPos;
	grV2f	EmitrDir;
	grV2f	PartMinMaxRotInDeg;
	grV2f	PartMinMaxSpd;
	grV2f	PartMinMaxLife;

	float	EmitrRotInDeg,
			PartPosOffsetRadius,
			PartRotInDeg;

	grV2f	PartMinMaxModSpd;
	float	PartModSpd;
};

#endif	// _H_GRPARTICLE_ATTRIBUTE_
