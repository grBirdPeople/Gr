#ifndef		_H_GRPARTICLE_ATTRIBUTE_
#define		_H_GRPARTICLE_ATTRIBUTE_

#include	"grV2.h"


struct grSParticleAttribute
{
	grSParticleAttribute( void )
		: EmitrPos				( grV2f( 320.0f, 180.0f ) )	// TODO: Actually get screen origo
		, EmitrDir				( grV2f( 0.0f, -1.0 ) )
		, PartRotInDegMinMax	( grV2f( 315.0f, 45.0f ) )
		, PartSpdMinMax			( grV2f( 16.0f, 64.0f ) )
		, PartLifeMinMax		( grV2f( 2.0f, 4.0f ) )

		, EmitrRotInDeg			( 0.0f )
		, PartRadiusPosOffset	( 20.0f )
		, PartRotInDeg			( 0.0f )

		, PartSpdMod			( 0.0f )
		, PartSpdOsc			( 0.0f )
	{}

	grV2f	EmitrPos;
	grV2f	EmitrDir;
	grV2f	PartRotInDegMinMax;
	grV2f	PartSpdMinMax;
	grV2f	PartLifeMinMax;

	float	EmitrRotInDeg,
			PartRadiusPosOffset,
			PartRotInDeg;

	float	PartSpdMod,
			PartSpdOsc;
};

#endif	// _H_GRPARTICLE_ATTRIBUTE_
