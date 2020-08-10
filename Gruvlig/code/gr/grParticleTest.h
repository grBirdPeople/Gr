#ifndef		_GRPARTICLETEST_H_
#define		_GRPARTICLETEST_H_

#include	<SFML/Graphics/RenderWindow.hpp>
#include	<SFML/Graphics/VertexBuffer.hpp>
#include	<SFML/Graphics/VertexArray.hpp>

#include	"grCommon.h"
#include	"grV2.h"
#include	"grRandom.h"
#include	"grAlgo.h"
#include	"grColor.h"
#include	"grStruct.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
typedef grColor::SRgba SRgba;

////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSParticleArr
{
	grSParticleArr( const sizeT size )
		: puVerts( std::make_unique<sf::Vertex[]>( size ) )
		, puColorStart( std::make_unique<SRgba[]>( size ) )
		, puColorEnd( std::make_unique<SRgba[]>( size ) )
		, puScaleStart( std::make_unique<grV2f[]>( size ) )
		, puScaleEnd( std::make_unique<grV2f[]>( size ) )
		, puForce( std::make_unique<float[]>( size ) )
		, puAcceleration( std::make_unique<grV2f[]>( size ) )
		, puVelocity( std::make_unique<grV2f[]>( size ) )
		, puMass( std::make_unique<float[]>( size ) )
		, puLife( std::make_unique<float[]>( size ) )
	{}
	grSParticleArr( const grSParticleArr& ) = delete;
	grSParticleArr& operator=( const grSParticleArr& ) = delete;
	grSParticleArr( grSParticleArr&& ) noexcept = delete;
	grSParticleArr& operator=( grSParticleArr&& ) noexcept = delete;

	pU<sf::Vertex[]> puVerts;	// Holds position, color, texcoord
	pU<SRgba[]> puColorStart;
	pU<SRgba[]> puColorEnd;
	pU<grV2f[]> puScaleStart;
	pU<grV2f[]> puScaleEnd;
	pU<float[]> puForce;
	pU<grV2f[]> puAcceleration;
	pU<grV2f[]> puVelocity;
	pU<float[]> puMass;
	pU<float[]> puLife;
};

struct grSParticleVar
{
	grV2f SystemPosition;
	grV2f GravityV;
	float GravityF;
	float EmitRateSec;
	float EmitRate;
	float SpawnAccT;
	sizeT Size;
	sizeT Alive;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
enum class EPartValueEqual
{
	NO = 0,
	YES = 1
};

struct grSBaseGenerate
{
	inline void SetBaseColor( grColor::SRgba& rStart, grColor::SRgba& rEnd )
	{
		rStart.R = grMath::Clamp<float>( rStart.R, 0.0f, 255.0f );
		rStart.G = grMath::Clamp<float>( rStart.G, 0.0f, 255.0f );
		rStart.B = grMath::Clamp<float>( rStart.B, 0.0f, 255.0f );
		rStart.A = grMath::Clamp<float>( rStart.A, 0.0f, 255.0f );

		rEnd.R = grMath::Clamp<float>( rEnd.R, 0.0f, 255.0f );
		rEnd.G = grMath::Clamp<float>( rEnd.G, 0.0f, 255.0f );
		rEnd.B = grMath::Clamp<float>( rEnd.B, 0.0f, 255.0f );
		rEnd.A = grMath::Clamp<float>( rEnd.A, 0.0f, 255.0f );

		if ( grMath::CmpFloat( rStart.R, rEnd.R ) &&
			 grMath::CmpFloat( rStart.G, rEnd.G ) &&
			 grMath::CmpFloat( rStart.B, rEnd.B ) &&
			 grMath::CmpFloat( rStart.A, rEnd.A ) )
		{
			Equal = EPartValueEqual::YES;
		}
	}

	inline void SetBaseStartEnd( grV2f& rStart, grV2f& rEnd )
	{
		rStart = grMath::AbsV2f( rStart );
		rEnd = grMath::AbsV2f( rEnd );
		Equal = grMath::CmpV2f( rStart, rEnd ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	inline void SetBaseMinMaxX2( grV2f& rMin, grV2f& rMax )
	{
		grMath::RangeCheckV2fx2( rMin, rMax );
		Equal = grMath::CmpV2f( rMin, rMax ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	inline void SetBaseMinMax( grV2f& rMinMax )
	{
		grMath::RangeCheckV2f( rMinMax );
		Equal = grMath::CmpFloat( rMinMax.x, rMinMax.y ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	EPartValueEqual Equal;
};

struct grSColorGenerate : public grSBaseGenerate
{
	grSColorGenerate()
		: Rand( false )
	{}

	inline void Set( const grColor::SRgba& rStart, const grColor::SRgba& rEnd, const bool randomize )
	{
		LocalStart = rStart;
		LocalEnd = rEnd;
		Rand = randomize;
		SetBaseColor( LocalStart, LocalEnd );
	}

	inline void Generate( pU<SRgba[]>& rColorStart, pU<SRgba[]>& rColorEnd, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Rand )
		{
			float midR{ grMath::AbsF( LocalStart.R - LocalEnd.R ) * 0.5f };
			float midG{ grMath::AbsF( LocalStart.G - LocalEnd.G ) * 0.5f };
			float midB{ grMath::AbsF( LocalStart.B - LocalEnd.B ) * 0.5f };
			//float midA = grMath::Abs( LocalStart.A - LocalEnd.A ) * 0.5f;		// Unsure if random alpha is desired

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grColor::SRgba result{ LocalStart.R > midR ? ( uint16_t )rRand->Float( midR, LocalStart.R ) : ( uint16_t )rRand->Float( LocalStart.R, midR ),
									   LocalStart.G > midG ? ( uint16_t )rRand->Float( midG, LocalStart.G ) : ( uint16_t )rRand->Float( LocalStart.G, midG ),
									   LocalStart.B > midB ? ( uint16_t )rRand->Float( midB, LocalStart.B ) : ( uint16_t )rRand->Float( LocalStart.B, midB ),
										( uint16_t )LocalStart.A };

				rColorStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grColor::SRgba result{ LocalEnd.R > midR ? ( uint16_t )rRand->Float( midR, LocalEnd.R ) : ( uint16_t )rRand->Float( LocalEnd.R, midR ),
									   LocalEnd.G > midG ? ( uint16_t )rRand->Float( midG, LocalEnd.G ) : ( uint16_t )rRand->Float( LocalEnd.G, midG ) ,
									   LocalEnd.B > midB ? ( uint16_t )rRand->Float( midB, LocalEnd.B ) : ( uint16_t )rRand->Float( LocalEnd.B, midB ),
										( uint16_t )LocalEnd.A };

				rColorEnd[ i ] = result;
			}

			return;
		}
		
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rColorStart[ i ] = LocalStart;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rColorEnd[ i ] = LocalEnd;
	}

	SRgba LocalStart, LocalEnd;
	bool Rand;
};

struct grSScaleGenerate : public grSBaseGenerate
{
	inline void Set( const grV2f& start, const grV2f& end )
	{
		LocalStart = start;
		LocalEnd = end;
		SetBaseStartEnd( LocalStart, LocalEnd );
	}

	inline void Generate( pU<grV2f[]>& rScaleStart, pU<grV2f[]>& rScaleEnd, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Equal == EPartValueEqual::NO )
		{			
			float midX{ grMath::AbsF( LocalStart.x - LocalEnd.x ) * 0.5f };
			float midY{ grMath::AbsF( LocalStart.y - LocalEnd.y ) * 0.5f };

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( LocalStart.x > midX ? rRand->Float( midX, LocalStart.x ) : rRand->Float( LocalStart.x, midX ),
							  LocalStart.y > midY ? rRand->Float( midY, LocalStart.y ) : rRand->Float( LocalStart.y, midY ) );

				rScaleStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( LocalEnd.x > midX ? rRand->Float( midX, LocalEnd.x ) : rRand->Float( LocalEnd.x, midX ),
							  LocalEnd.y > midY ? rRand->Float( midY, LocalEnd.y ) : rRand->Float( LocalEnd.y, midY ) );

				rScaleEnd[ i ] = result;
			}

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rScaleStart[ i ] = LocalStart;
	}

	grV2f LocalStart, LocalEnd;
};

struct grSForceGenerate : public grSBaseGenerate
{
	enum class EForceType
	{
		GRAVITY = 0x1,
		WIND = 0x2
	};

	inline void Set( const grV2f& min, const grV2f& max )
	{
		LocalMin = min;
		LocalMax = max;
		SetBaseMinMaxX2( LocalMin, LocalMax );
	}

	inline void Generate( pU<grV2f[]>& rAcceleration, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Equal == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rAcceleration[ i ] += rRand->V2fx2( LocalMin, LocalMax );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rAcceleration[ i ] += LocalMin;
	}

	grV2f LocalMin, LocalMax;
};

struct grSPositionGenerate : public grSBaseGenerate
{
	inline void Set( const grV2f& min, const grV2f& max )
	{
		LocalMin = min;
		LocalMax = max;
		SetBaseMinMaxX2( LocalMin, LocalMax );
	}

	inline void Generate( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Equal == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f p{ rRand->V2fx2( LocalMin, LocalMax ) + sysPos };
				rPosition[ i ].position.x = p.x;
				rPosition[ i ].position.y = p.y;
			}

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			grV2f p{ LocalMin + sysPos };
			rPosition[ i ].position.x = p.x;
			rPosition[ i ].position.y = p.y;
		}
	}

	grV2f LocalMin, LocalMax;
};

struct grSMassGenerate : public grSBaseGenerate
{
	inline void Set( const grV2f& rMinMax )
	{
		LocalMinMax = rMinMax;
		if ( LocalMinMax.x < 1.0f )
			LocalMinMax.x = 1.0f;

		SetBaseMinMax( LocalMinMax );
	}

	inline void Generate( pU<float[]>& rMass, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Equal == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rMass[ i ] = rRand->Float( LocalMinMax.x, LocalMinMax.y );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rMass[ i ] = LocalMinMax.x;
	}

	grV2f LocalMinMax;
};

struct grSLifeGenerate : public grSBaseGenerate
{
	inline void Set( const grV2f& minMax )
	{
		LocalMinMax = minMax;
		if ( LocalMinMax.x < 0.0f )
			LocalMinMax.x = 0.0f;

		SetBaseMinMax( LocalMinMax );
	}

	inline void Generate( pU<float[]>& rLife, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Equal == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rLife[ i ] = rRand->Float( LocalMinMax.x, LocalMinMax.y );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rLife[ i ] = LocalMinMax.x;
	}

	grV2f LocalMinMax;
};

struct grSEmitter
{
	grSEmitter()
		: puRand( std::make_unique<grRandom>() )
	{}
	grSEmitter( const grSEmitter& ) = delete;
	grSEmitter& operator=( const grSEmitter& ) = delete;
	grSEmitter( grSEmitter&& ) noexcept = delete;
	grSEmitter& operator=( grSEmitter&& ) noexcept = delete;

	inline void Emit( pU<grSParticleVar>& rParticleData, pU<grSParticleArr>& rParticleArr, const float deltaT )
	{
		// This could be done smoother ex; NewSpawns = dt * SpawnRate
		// Problem is I haven't figured out how to make the spawnrate 'API' call non arbitrary in relation to real time, for example seconds

		// Catch things
		grV2f sysPos{ rParticleData->SystemPosition };
		float spawnAccT{ rParticleData->SpawnAccT };
		float emitRate{ rParticleData->EmitRate };
		sizeT last{ rParticleData->Size - 1 };
		sizeT startIdx{ rParticleData->Alive };
		sizeT endIdx{ 0 };
		sizeT emitAcc{ 0 };

		// If spawns per frame are be greater than frame time
		spawnAccT += deltaT;
		while ( spawnAccT >= emitRate )
		{
			spawnAccT -= emitRate;
			emitAcc += 1;
		}

		if ( emitAcc > 0 )
		{
			endIdx = { grMath::Min<sizeT>( startIdx + emitAcc, last ) };
			if ( startIdx == endIdx )
				return;

			if ( puPosition ) puPosition->Generate( rParticleArr->puVerts, sysPos, startIdx, endIdx, puRand );
			if ( puColor ) puColor->Generate( rParticleArr->puColorStart, rParticleArr->puColorEnd, startIdx, endIdx, puRand );
			if ( puScale ) puScale->Generate( rParticleArr->puScaleStart, rParticleArr->puScaleEnd, startIdx, endIdx, puRand );
			if ( puForce ) puForce->Generate( rParticleArr->puAcceleration, startIdx, endIdx, puRand );
			if ( puMass ) puMass->Generate( rParticleArr->puMass, startIdx, endIdx, puRand );
			if ( puLife ) puLife->Generate( rParticleArr->puLife, startIdx, endIdx, puRand );
		}

		rParticleData->SpawnAccT = spawnAccT;
		rParticleData->Alive += endIdx - startIdx;
	}

	// All types of generators goes here
	// No slow virtual stuff allowed so each generator has it's own place
	// TODO: If multiple generators of the same type would be allowed and produce cool results perhaps change to arrays
	pU<grSPositionGenerate> puPosition;
	pU<grSColorGenerate> puColor;
	pU<grSScaleGenerate> puScale;
	pU<grSForceGenerate> puForce;
	pU<grSMassGenerate> puMass;
	pU<grSLifeGenerate> puLife;

	pU<grRandom> puRand;	// Is slightly greater than 5mb so instead of each generator containing one it's passed. Is not thread safe so if that happens it needds to change
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSColorUpdate
{
	// TODO: Only create updater instance if colors are NOT equal

	grSColorUpdate( const bool hsv )
		: Hsv( hsv )
	{}

	inline void Update( const sizeT alive, pU<SRgba[]>& rColorStart, const pU<SRgba[]>& rColorEnd, const pU<float[]>& rLife, pU<sf::Vertex[]>& rVerts, const float deltaT )
	{
		if ( Hsv )
		{
			// RGB -> HSV lerp -> RGB
			for ( sizeT i = 0; i < alive; ++i )
			{
				// Faster to store in local block and than lerp then passing to lerp by directly indexing the array // Measured with std::chronos
				grColor::SHsva start{ grColor::Rgba2Hsva( rColorStart[ i ] ) };
				grColor::SHsva end{ grColor::Rgba2Hsva( rColorEnd[ i ] ) };

				float lerpValue{ 1.0f / rLife[ i ] * deltaT };

				start.H = grMath::Lerp( start.H, end.H, lerpValue );
				start.S = grMath::Lerp( start.S, end.S, lerpValue );
				start.V = grMath::Lerp( start.V, end.V, lerpValue );
				start.A = grMath::Lerp( start.A, end.A, lerpValue );

				rColorStart[ i ] = grColor::Hsva2Rgba( start );
			}

			for ( sizeT i = 0; i < alive; ++i )
			{
				SRgba c{ rColorStart[ i ] };
				rVerts[ i ].color.r = ( uint8_t )c.R;
				rVerts[ i ].color.g = ( uint8_t )c.G;
				rVerts[ i ].color.b = ( uint8_t )c.B;
				rVerts[ i ].color.a = ( uint8_t )c.A;
			}

			return;
		}

		for ( sizeT i = 0; i < alive; ++i )
		{
			grColor::SRgba start{ rColorStart[ i ] };
			grColor::SRgba end{ rColorEnd[ i ] };
			float lerpValue{ 1.0f / rLife[ i ] * deltaT };

			rColorStart[ i ].R = grMath::Lerp( start.R, end.R, lerpValue );
			rColorStart[ i ].G = grMath::Lerp( start.G, end.G, lerpValue );
			rColorStart[ i ].B = grMath::Lerp( start.B, end.B, lerpValue );
			rColorStart[ i ].A = grMath::Lerp( start.A, end.A, lerpValue );
		}

		for ( sizeT i = 0; i < alive; ++i )
		{
			SRgba c { rColorStart[ i ] };
			rVerts[ i ].color.r = ( uint8_t )c.R;
			rVerts[ i ].color.g = ( uint8_t )c.G;
			rVerts[ i ].color.b = ( uint8_t )c.B;
			rVerts[ i ].color.a = ( uint8_t )c.A;
		}
	}

	bool Hsv;
};

struct grSScaleUpdate
{
	inline void Update( const sizeT alive, pU<grV2f[]>& rScaleStart, const pU<grV2f[]>& rScaleEnd,const pU<float[]>& rLife, const float deltaT )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			grV2f start{ rScaleStart[ i ] };
			grV2f end{ rScaleEnd[ i ] };
			float lerpValue{ 1.0f / rLife[ i ] * deltaT };
			rScaleStart[ i ] = grMath::LerpV2f( start, end, lerpValue );
		}
	}
};

struct grSForceUpdate
{
	inline void Update( const sizeT alive, const grV2f& rGravity, pU<grV2f[]>& rAcceleration, const pU<float[]>& rMass )
	{
		grV2f g{ rGravity };
		float m{ 0.0f };
		float maxForce{ 10.0f }; // TODO: Implement and move this correctly
		for ( sizeT i = 0; i < alive; ++i )
		{
			m = rMass[ i ];
			rAcceleration[ i ] += grV2f( g / m ).LimitMax( maxForce );
		}
	}
};

struct grSVelocityUpdate
{
	inline void Update( const sizeT alive, pU<grV2f[]>& rVelocity, const pU<grV2f[]>& rAcceleration, const float deltaT )
	{
		float maxSpeed{ 100.0f };	// TODO: Implement and move this correctly
		for ( sizeT i = 0; i < alive; ++i )
			rVelocity[ i ] = grV2f( rAcceleration[ i ] ).LimitMax( maxSpeed );
	}
};

struct grSPositionUpdate
{
	inline void Update( const sizeT alive, pU<sf::Vertex[]>& rVerts, pU<grV2f[]>& rVelocity, const float deltaT )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			rVerts[ i ].position.x += rVelocity[ i ].x * deltaT;
			rVerts[ i ].position.y += rVelocity[ i ].y * deltaT;
		}
	}
};

struct grSLifeUpdate
{
	inline sizeT Update( const sizeT alive, pU<grSParticleArr>& rParticleArr, const float deltaT )
	{
		sizeT last{ alive };
		sizeT killCount{ 0 };
		for ( sizeT i = 0; i < last; ++i )
		{
			rParticleArr->puLife[ i ] -= deltaT;
			if ( rParticleArr->puLife[ i ] <= 0.0f )
			{
				++killCount;
				--last;
				Kill( i, last, rParticleArr );
			}
		}

		return killCount;
	}

	inline void Kill( const sizeT nowIdx, const sizeT last, pU<grSParticleArr>& rParticleArr )
	{
		grAlgo::Swap( rParticleArr->puVerts[ nowIdx ], rParticleArr->puVerts[ last ] );
		grAlgo::Swap( rParticleArr->puColorStart[ nowIdx ], rParticleArr->puColorStart[ last ] );
		grAlgo::Swap( rParticleArr->puColorEnd[ nowIdx ], rParticleArr->puColorEnd[ last ] );
		grAlgo::Swap( rParticleArr->puScaleStart[ nowIdx ], rParticleArr->puScaleStart[ last ] );
		grAlgo::Swap( rParticleArr->puScaleEnd[ nowIdx ], rParticleArr->puScaleEnd[ last ] );
		grAlgo::Swap( rParticleArr->puAcceleration[ nowIdx ], rParticleArr->puAcceleration[ last ] );
		grAlgo::Swap( rParticleArr->puVelocity[ nowIdx ], rParticleArr->puVelocity[ last ] );
		grAlgo::Swap( rParticleArr->puForce[ nowIdx ], rParticleArr->puForce[ last ] );
		grAlgo::Swap( rParticleArr->puMass[ nowIdx ], rParticleArr->puMass[ last ] );
		grAlgo::Swap( rParticleArr->puLife[ nowIdx ], rParticleArr->puLife[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		rParticleArr->puAcceleration[ last ] = { 0.0f, 0.0f };
	}
};

struct grSUpdate
{
	grSUpdate( void )
	: puPosition( std::make_unique<grSPositionUpdate>() ) // Updaters that dont have an API call in the system class gets created here
	, puVelocity( std::make_unique<grSVelocityUpdate>() )
	{}
	grSUpdate( const grSUpdate& ) = delete;
	grSUpdate& operator=( const grSUpdate& ) = delete;
	grSUpdate( grSUpdate&& ) noexcept = delete;
	grSUpdate& operator=( grSUpdate&& ) noexcept = delete;

	inline void Update( pU<grSParticleVar>& rParticleVar, pU<grSParticleArr>& rParticleArr, const float deltaT )
	{
		grV2f gravity{ rParticleVar->GravityV };
		sizeT alive{ rParticleVar->Alive };

		// TODO: Would be cool to not need the if's. Do not wan't the indirections from a virtual base so some type of eventlist might be an option
		// Update all updaters
		if ( puPosition ) puPosition->Update( alive, rParticleArr->puVerts, rParticleArr->puVelocity, deltaT );
		if ( puColor ) puColor->Update( alive, rParticleArr->puColorStart, rParticleArr->puColorEnd, rParticleArr->puLife, rParticleArr->puVerts, deltaT );
		if ( puScale ) puScale->Update( alive, rParticleArr->puScaleStart, rParticleArr->puScaleEnd, rParticleArr->puLife, deltaT );
		if ( puForce ) puForce->Update( alive, gravity, rParticleArr->puAcceleration, rParticleArr->puMass );
		if ( puVelocity ) puVelocity->Update( alive, rParticleArr->puVelocity, rParticleArr->puAcceleration, deltaT );
		if ( puLife ) alive = puLife->Update( alive, rParticleArr, deltaT );

		rParticleVar->Alive -= alive;
	}

	// No slow virtual stuff allowed so each updater has it's own place
	// All types of updaters goes here
	pU<grSPositionUpdate> puPosition;
	pU<grSColorUpdate> puColor;
	pU<grSScaleUpdate> puScale;
	pU<grSForceUpdate> puForce;
	pU<grSVelocityUpdate> puVelocity;
	pU<grSLifeUpdate> puLife;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
class grCParticleSys
{
public:
	grCParticleSys( const intU size = 2000, const float emitRate = 1000.0f, const grV2f& position = grV2f( 320.0f, 180.0f ) );
	grCParticleSys( const grCParticleSys& ) = delete;
	grCParticleSys& operator=( const grCParticleSys& ) = delete;
	grCParticleSys( grCParticleSys&& ) noexcept = delete;
	grCParticleSys& operator=( grCParticleSys&& ) noexcept = delete;

	float GetEmitRate() const;

	void SetEmitRate( const float emitRate );
	void SetSystemPosition( const grV2f& position );
	void SetGravity( const float dirInDeg, const float force );

	// If min<->max are equal, the value will be that. If not equal, the value will be rand inbetween
	// TODO: Could be better to use injection here as that would make it possible store behaviours externaly
	void SetColor( const grColor::SRgba& start, const grColor::SRgba& end, const bool hsv = false, const bool randomize = false );
	void SetScale( const grV2f& start, const grV2f& end );
	void SetForce( const grV2f& min, const grV2f& max );
	void SetPosition( const grV2f& min, const grV2f& max );
	void SetMass( const grV2f& minMax );
	void SetLife( const grV2f& minMax );

	// TODO: Remeber to make this call once in the particle manager whenever that class exists
	void Update( const float deltaT );
	void Render( sf::RenderWindow& rRenderWin );

private:
	pU<grSParticleArr> m_puParticleArr;
	pU<grSParticleVar> m_puParticleVar;
	pU<grSEmitter> m_puEmit;
	pU<grSUpdate> m_puUpdate;

	// TODO: Needs activation choices for emission: start/stop/timer/infinite
};

#endif	// _GRPARTICLETEST_H_