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

	friend struct grParticleSetupPB;
	friend struct grParticleSystemPB;

	//////////////////////////////////////////////////

	grParticleAttributePB( void )
		: Velocity			( grV2f() )
		, Acceleration		( grV2f() )
		, Gravity			( grV2f() )
		, Mass				( 1.0f )
		, SpawnInMilliSec	( 1.0f / 100.0f )
		, SpawnCounter		( 0.0f )

		// Range values
		, SpeedRange		( grV2f() )
		, DirectionRange	( grV2f() )
		, LifetimeRange		( grV2f() )

		// Osc values
		, SpeedOsc			( 0.0f )

		// Range flags
		, bSpeedRange		( false )
		, bDirectionRange	( false )
		, bLifetimeRange	( false )

		// Osc flags
		, bSpeedOsc			( false )
	{
		grCore& rCore = grCore::Instance();
		Position = grV2f( rCore.GetRenderWin().getSize().x * 0.5f, rCore.GetRenderWin().getSize().y * 0.5f );
	}

	//////////////////////////////////////////////////
	void SetSpeed( const float min, const float max, const float osc = 0.0f )
	{
		SpeedRange = MinMaxFixer( grMath::Abs( min ), grMath::Abs( max ) );;
		bSpeedRange = ( grMath::CmpFloat( SpeedRange.x, SpeedRange.y ) ) ? false : true;
		bSpeedOsc = ( osc != 0.0f ) ? true : false;
	}
	void SetDirection( const float fromDeg, const float toDeg )	// Zero is north. Only positive values work.
	{
		DirectionRange = grV2f( grMath::Clamp( fromDeg, 0.0f, 359.9f ), grMath::Clamp( toDeg, 0.0f, 359.9f ) );;
		bDirectionRange = ( grMath::CmpFloat( DirectionRange.x, DirectionRange.y ) ) ? false : true;
	}
	void SetGravity( const float amount, const float dirInDeg )
	{
		Gravity = grV2f( 0.0f, -1.0f ) * amount;
		grMath::RotatePoint( &Gravity, grMath::Clamp( dirInDeg, 0.0f, 359.9f ) * grMath::DegToRad );
	}
	void SetLifetime( const float minT, const float maxT )
	{
		LifetimeRange = MinMaxFixer( grMath::Abs( minT ), grMath::Abs( maxT ) );
		bLifetimeRange = ( grMath::CmpFloat( LifetimeRange.x, LifetimeRange.y ) ) ? false : true;
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

	// Oscillation values
	float	SpeedOsc;

	// Range flags
	bool	bSpeedRange,
			bDirectionRange,
			bLifetimeRange;
	// Oscillation flags
	bool	bSpeedOsc;
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
		for ( sizeT i = 0; i < PARTICLE_PER_SETUP; ++i )
				arrParticle[ i ] = nullptr;

		pAttribute = std::make_unique<grParticleAttributePB>();
	}

	~grParticleSetupPB( void )
	{
		pAttribute.reset();

		for ( sInt i = 0; i < PARTICLE_PER_SETUP; ++i )
		{
			if( arrParticle[ i ] != nullptr )
				arrParticle[ i ].reset();
		}
	}

	grParticleSetupPB( grParticleSetupPB const& ) = delete;
	grParticleSetupPB& operator=( grParticleSetupPB const& ) = delete;

	//////////////////////////////////////////////////

	void SetParticleAttribute( const grParticleAttributePB& rAttribute )
	{
		pAttribute->Position = rAttribute.Position;
		pAttribute->Velocity = rAttribute.Velocity;
		pAttribute->Acceleration = rAttribute.Acceleration;
		pAttribute->Gravity = rAttribute.Gravity;
		pAttribute->Mass = rAttribute.Mass;
		pAttribute->SpawnCounter = rAttribute.SpawnCounter;
		pAttribute->SpawnInMilliSec = rAttribute.SpawnInMilliSec;

		// Range values
		pAttribute->SpeedRange = rAttribute.SpeedRange;
		pAttribute->DirectionRange = rAttribute.DirectionRange;
		pAttribute->LifetimeRange = rAttribute.LifetimeRange;

		// Osc values
		pAttribute->SpeedOsc = rAttribute.SpeedOsc;

		// Range flags
		pAttribute->bSpeedRange = rAttribute.bSpeedRange;
		pAttribute->bDirectionRange = rAttribute.bDirectionRange;
		pAttribute->bLifetimeRange = rAttribute.bLifetimeRange;

		// Osc flags
		pAttribute->bSpeedOsc = rAttribute.bSpeedOsc;
	}

	//////////////////////////////////////////////////

	arrParticle					arrParticle;
	uPtr<grParticleAttributePB>	pAttribute;

	uInt						Id,
								ParticlesActive;
};

#endif	// _H_GRPARTICLEPB_