#ifndef		_H_GRPARTICLEPB_
#define		_H_GRPARTICLEPB_

#include    "grCore.h"
#include	"grMath.h"
#include	"grV2.h"

struct		grParticlePB;

//using		vecParticle = std::vector<uPtr<grParticlePB>>;
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
		: Acceleration		( grV2f( 5000.0f, -22500.0f ) )
		, Gravity			( grV2f( 0.0f, 500.0f ) )

		, LifetimeRange		( grV2f( 2.0f, 2.0f ) )

		, Mass				( 1.0f )
		, SpawnInMilliSec	( 1.0f / 45.0f )
		, SpawnCounter		( 0.0f )

		, bDirectionRange	( false )
		, bLifetimeRange	( false )

		, bDirectionOsc		( false )
	{
		grCore& rCore = grCore::Instance();
		Position = grV2f( rCore.GetRenderWin().getSize().x * 0.5f, rCore.GetRenderWin().getSize().y * 0.5f );
	}

	//////////////////////////////////////////////////

	void SetLifetime( const float min, const float max )
	{
		grV2f life = grV2f( std::abs( min ), std::abs( max ) );

		if ( life.x > life.y )
			life.x = life.y;
		else if ( life.y < life.x )
			life.y = life.x;

		LifetimeRange = life;
		bLifetimeRange = ( life.x == life.y ) ? false : true;
	}
	void SetDirection( const float min, const float max, const float osc )
	{

	}

	//////////////////////////////////////////////////

private:

	grV2f	Position;
	grV2f	Velocity;
	grV2f	Acceleration;
	grV2f	Gravity;

	// Range
	grV2f	LifetimeRange;

	float	Mass;

	float	SpawnInMilliSec,
			SpawnCounter;

	// Range
	bool	bDirectionRange,
			bLifetimeRange;
	// Oscillation
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
		pAttribute->LifetimeRange = rAttribute.LifetimeRange;
		pAttribute->Mass = rAttribute.Mass;
		pAttribute->SpawnCounter = rAttribute.SpawnCounter;
		pAttribute->SpawnInMilliSec = rAttribute.SpawnInMilliSec;

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