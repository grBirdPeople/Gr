#ifndef		_H_GRPARTICLEPB_
#define		_H_GRPARTICLEPB_

#include    "grCore.h"
#include	"grV2.h"


// TODO: When there is an API for particles hook it up to grParticleAttributePB

// grParticleAttributePB // One copy per particle setup
//////////////////////////////////////////////////
struct grParticleAttributePB
{
	grParticleAttributePB( void )
		: Velocity		( grV2f() )
		, Gravity		( grV2f( 0.0f, 100.0f ) )
		, Acceleration	( grV2f( 5000.0f, -20000.0f ) )
		, Mass			( 1.0f )
		, LifeTime		( 3.5f )
	{
		grCore& rCore = grCore::Instance();
		Position = grV2f( rCore.GetRenderWin().getSize().x * 0.5f, rCore.GetRenderWin().getSize().y * 0.5f );
	}

	grV2f	Position;
	grV2f	Velocity;
	grV2f	Acceleration;
	grV2f	Gravity;
	float	Mass,
			LifeTime;
};


// grParticlePB // Multiple instances
//////////////////////////////////////////////////
struct grParticlePB
{
	grV2f	Position;
	grV2f	Velocity;
	grV2f	Acceleration;
	float	Mass,
			LifeTime;
};

#endif	// _H_GRPARTICLEPB_