#ifndef		_H_GRPARTICLEPB_
#define		_H_GRPARTICLEPB_

#include    "grCore.h"
#include	"grMath.h"
#include	"grV2.h"

struct		grParticlePB;

using		arrParticle = uPtr<grParticlePB>[ PARTICLE_PER_SETUP ];


// TODO: When there is an API for particles hook it up to grParticleAttributePB

// grParticleAttributePB // One copy per particle setup
//////////////////////////////////////////////////
class grParticleAttributePB
{
public:

	friend class grParticleSetupPB;
	friend class grParticleSystemPB;

	//////////////////////////////////////////////////

	grParticleAttributePB( void )
		: Velocity			( grV2f() )
		, Acceleration		( grV2f() )
		, Gravity			( grV2f( 0.0f, 0.0f ) )

		, SpeedRange		( grV2f() )
		, DirectionRange	( grV2f() )
		, LifetimeRange		( grV2f() )

		, Mass				( 1.0f )
		, SpawnInMilliSec	( 1.0f / 100.0f )
		, SpawnCounter		( 0.0f )

		, bSpeedRange		( false )
		, bDirectionRange	( false )
		, bLifetimeRange	( false )

		, bDirectionOsc		( false )
	{
		grCore& rCore = grCore::Instance();
		Position = grV2f( rCore.GetRenderWin().getSize().x * 0.5f, rCore.GetRenderWin().getSize().y * 0.5f );

		// TEST
		//Acceleration += grV2f( 5000.0f, -22500.0f );
		// TEST
	}

	//////////////////////////////////////////////////
	void SetSpeed( const float min, const float max )
	{
		grV2f spd = MinMaxFixer( grMath::Abs( min ), grMath::Abs( max ) );
		SpeedRange = spd;
		bSpeedRange = ( grMath::CmpFloat( spd.x, spd.y ) ) ? false : true;
	}
	void SetDirection( const float minDeg, const float maxDeg )
	{
		grV2f dir = MinMaxFixer( minDeg, maxDeg );
		dir = grV2f( grMath::Clamp( minDeg, -359.9, 359.9f ), grMath::Clamp( maxDeg, -359.9, 359.9f ) );
		//if ( dir.x < 0.0f )
		//	dir.x = 360.0f + dir.x;
		//if ( dir.y < 0.0f )
		//	dir.y = 360.0f + dir.y;

		DirectionRange = dir;
		bDirectionRange = ( grMath::CmpFloat( dir.x, dir.y ) ) ? false : true;
	}
	void SetLifetime( const float minT, const float maxT )
	{
		grV2f life = MinMaxFixer( grMath::Abs( minT ), grMath::Abs( maxT ) );
		LifetimeRange = life;
		bLifetimeRange = ( grMath::CmpFloat( life.x, life.y ) ) ? false : true;
	}

	//////////////////////////////////////////////////

private:

	grV2f MinMaxFixer( const float min, const float max )
	{
		return grV2f( grMath::Min( min , max ), grMath::Max( min, max ) );
	}

	//////////////////////////////////////////////////

	grV2f	Position;
	grV2f	Velocity;
	grV2f	Acceleration;
	grV2f	Gravity;

	float	Mass;

	float	SpawnInMilliSec,
			SpawnCounter;

	// Range values
	grV2f	SpeedRange;
	grV2f	DirectionRange;
	grV2f	LifetimeRange;

	// Range flags
	bool	bSpeedRange,
			bDirectionRange,
			bLifetimeRange;
	// Oscillation flags
	bool	bDirectionOsc;
};


// grParticlePB // Multiple instances
//////////////////////////////////////////////////
struct grParticlePB
{
	grParticlePB( void )
		: Mass		( 0.0f )
		, Lifetime	( 0.0f )
	{}

	grV2f	Position;
	grV2f	Velocity;
	grV2f	Acceleration;
	float	Mass,
			Lifetime;
};


// grParticleSetupPB
//////////////////////////////////////////////////
struct grParticleSetupPB
{
	grParticleSetupPB( const uInt id )
		: Id				( id )
		, ParticlesActive	( 0 )

	{
		for( sizeT i = 0; i < PARTICLE_PER_SETUP; ++i )
			arrParticle[ i ] = nullptr;

		pAttribute = std::make_unique<grParticleAttributePB>();
	}

	~grParticleSetupPB( void )
	{
		pAttribute.reset();

		for( sInt i = 0; i < PARTICLE_PER_SETUP; ++i )
			arrParticle[ i ].reset();
	}

	//////////////////////////////////////////////////

	void SetParticleAttribute( const grParticleAttributePB& rAttribute )
	{
		pAttribute->Position = rAttribute.Position;
		pAttribute->Velocity = rAttribute.Velocity;
		pAttribute->Acceleration = rAttribute.Acceleration;
		pAttribute->Gravity = rAttribute.Gravity;

		pAttribute->SpeedRange = rAttribute.SpeedRange;
		pAttribute->DirectionRange = rAttribute.DirectionRange;
		pAttribute->LifetimeRange = rAttribute.LifetimeRange;

		pAttribute->Mass = rAttribute.Mass;
		pAttribute->SpawnCounter = rAttribute.SpawnCounter;
		pAttribute->SpawnInMilliSec = rAttribute.SpawnInMilliSec;

		pAttribute->bSpeedRange = rAttribute.bSpeedRange;
		pAttribute->bDirectionRange = rAttribute.bDirectionRange;
		pAttribute->bLifetimeRange = rAttribute.bLifetimeRange;
	}

	//////////////////////////////////////////////////

	arrParticle					arrParticle;
	uPtr<grParticleAttributePB>	pAttribute;

	uInt						Id,
								ParticlesActive;
};

#endif	// _H_GRPARTICLEPB_