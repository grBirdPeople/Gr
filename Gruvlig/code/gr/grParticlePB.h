#ifndef		_H_GRPARTICLEPB_
#define		_H_GRPARTICLEPB_

#include	"grDefine.h"
#include	"grMath.h"
#include	"grStruct.h"
#include	"grV2.h"


// TODO: When there is an API for particles hook it up to grParticleAttributePB

struct grSColor
{
	grSColor( void )
	{
		R = 255;
		G = 255;
		B = 255;
		A = 255;
	}
	grSColor( const uInt r, const uInt g, const uInt b, const uInt a )
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}

	uInt R, G, B, A;
};


class grParticlAttributePB
{
public:

	friend struct SParticleEmitter;

	grParticlAttributePB( void )
		: Color		( grSColor() )
		, Position	( grV2f() )
		, Velocity	( grV2f() )
		, Lifetime	( 0.0f )
	{}

	grSColor	Color;

	grV2f	Position;
	grV2f	Velocity;

	float	Lifetime;
};


//// grParticleAttributePB // One copy per particle setup
////////////////////////////////////////////////////
//class grParticleAttributePB
//{
//public:
//
//	friend struct grParticleSystemPB;
//
//	//////////////////////////////////////////////////
//
//	grParticleAttributePB( void )
//		: Velocity			( grV2f() )
//		, Acceleration		( grV2f() )
//		, Gravity			( grV2f() )
//		, Mass				( 1.0f )
//		, SpawnInMilliSec	( 1.0f / 100.0f )
//		, SpawnCounter		( 0.0f )
//		, Id				( 0 )
//		, ParticlesActive	( 0 )
//		, ParticleQuantity	( 0 )
//
//		// Range values
//		, SpeedRange		( grV2f() )
//		, DirectionRange	( grV2f() )
//		, LifetimeRange		( grV2f() )
//
//		// Osc values
//		, SpeedOsc			( 0.0f )
//
//		// Range flags
//		, bSpeedRange		( false )
//		, bDirectionRange	( false )
//		, bLifetimeRange	( false )
//
//		// Osc flags
//		, bSpeedOsc			( false )
//	{
//		grCore& rCore = grCore::Instance();
//		Position = grV2f( rCore.GetRenderWin().getSize().x * 0.5f, rCore.GetRenderWin().getSize().y * 0.5f );
//	}
//
//	//////////////////////////////////////////////////
//	void SetPosition( const grV2f& rPosition )
//	{
//		Position = rPosition;
//	}
//	void SetSpeed( const float min, const float max, const float osc = 0.0f )
//	{
//		SpeedRange = MinMaxFixer( grMath::Abs( min ), grMath::Abs( max ) );;
//		bSpeedRange = ( grMath::CmpFloat( SpeedRange.x, SpeedRange.y ) ) ? false : true;
//		bSpeedOsc = ( osc != 0.0f ) ? true : false;
//		SpeedOsc = osc;
//	}
//	void SetDirection( const float fromDeg, const float toDeg )	// Zero is north. Only positive values work.
//	{
//		DirectionRange = grV2f( grMath::Clamp( fromDeg, 0.0f, 359.9f ), grMath::Clamp( toDeg, 0.0f, 359.9f ) );;
//		bDirectionRange = ( grMath::CmpFloat( DirectionRange.x, DirectionRange.y ) ) ? false : true;
//	}
//	void SetGravity( const float amount, const float dirInDeg )
//	{
//		Gravity = grV2f( 0.0f, -1.0f ) * amount;
//		grMath::RotatePoint( &Gravity, grMath::Clamp( dirInDeg, 0.0f, 359.9f ) * grMath::DegToRad );
//	}
//	void SetLifetime( const float minT, const float maxT )
//	{
//		LifetimeRange = MinMaxFixer( grMath::Abs( minT ), grMath::Abs( maxT ) );
//		bLifetimeRange = ( grMath::CmpFloat( LifetimeRange.x, LifetimeRange.y ) ) ? false : true;
//	}
//
//	//////////////////////////////////////////////////
//
//private:
//
//	grV2f MinMaxFixer( const float min, const float max )
//	{
//		return grV2f( grMath::Min( min , max ), grMath::Max( min, max ) );
//	}
//
//	//////////////////////////////////////////////////
//
//	grV2f	Position;
//	grV2f	Velocity;
//	grV2f	Acceleration;
//	grV2f	Gravity;
//	float	Mass;
//
//	float	SpawnInMilliSec,
//			SpawnCounter;
//
//	uInt	Id,
//			ParticlesActive;
//
//public:
//	uInt	ParticleQuantity;
//
//private:
//	// Range values
//	grV2f	SpeedRange;
//	grV2f	DirectionRange;
//	grV2f	LifetimeRange;
//
//	// Oscillation values
//	float	SpeedOsc;
//
//	// Range flags
//	bool	bSpeedRange,
//			bDirectionRange,
//			bLifetimeRange;
//
//	// Oscillation flags
//	bool	bSpeedOsc;
//};


// grParticlePB // Multiple instances
//////////////////////////////////////////////////
struct grParticlePB
{
	//Phys based
	grParticlePB( void )
		: Color		( grSColor() )
		, Position	( grV2f() )
		, Velocity	( grV2f( 0.0f, 0.0f ) )
		, Lifetime	( 0.0f )
	{}

	grSColor	Color;

	grV2f	Position;
	grV2f	Velocity;
	float	Lifetime;
};


struct SParticleEmitter
{
	SParticleEmitter( const uInt id, const sizeT size )
		: uPArrParticle			( new grParticlePB* [ size ]() )
		, uPPartDeactivateQue	( new grStruct::grLoopQue<uInt>( size ) )
		, uPAttribute			( new grParticlAttributePB() )
		, SpawnCounter			( 0.0f )
		, SpawnInMilliSec		( 1.0f / 25.0f )	// Set to zero when particle API or their likes exists
		, Id					( id )
		, PartActive			( 0 )
		, PartSize				( size )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
			uPArrParticle[ i ] = new grParticlePB();
	}
	~SParticleEmitter( void )
	{
		for ( sizeT i = 0; i < PartSize; ++i )
		{
			if ( uPArrParticle[ i ] != nullptr )
				DELANDNULL( uPArrParticle[ i ] );
		}
		delete[] uPArrParticle.release();

		if ( uPPartDeactivateQue != nullptr )
			delete uPPartDeactivateQue.release();

		if ( uPAttribute != nullptr )
			delete uPAttribute.release();
	}
	SParticleEmitter( SParticleEmitter const& ) = delete;
	SParticleEmitter& operator=( SParticleEmitter  const& ) = delete;

	//////////////////////////////////////////////////

	uP<grParticlePB * []>			uPArrParticle;
	uP<grStruct::grLoopQue<uInt>>			uPPartDeactivateQue;
	uP<grParticlAttributePB>	uPAttribute;

	float	SpawnCounter,
		SpawnInMilliSec;

	uInt	Id,
		PartActive;

	sizeT	PartSize;
};

#endif	// _H_GRPARTICLEPB_