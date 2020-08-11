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
	inline void InitBaseColor( grColor::SRgba& rStart, grColor::SRgba& rEnd )
	{
		rStart.R = grMath::Clamp<float>( rStart.R, 0.0f, 255.0f );
		rStart.G = grMath::Clamp<float>( rStart.G, 0.0f, 255.0f );
		rStart.B = grMath::Clamp<float>( rStart.B, 0.0f, 255.0f );
		rStart.A = grMath::Clamp<float>( rStart.A, 0.0f, 255.0f );

		rEnd.R = grMath::Clamp<float>( rEnd.R, 0.0f, 255.0f );
		rEnd.G = grMath::Clamp<float>( rEnd.G, 0.0f, 255.0f );
		rEnd.B = grMath::Clamp<float>( rEnd.B, 0.0f, 255.0f );
		rEnd.A = grMath::Clamp<float>( rEnd.A, 0.0f, 255.0f );
	}

	inline void InitBaseStartEnd( grV2f& rStart, grV2f& rEnd )
	{
		IsEqual = grMath::CmpV2f( rStart, rEnd ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	inline void InitBaseMinMaxX2( grV2f& rMin, grV2f& rMax )
	{
		if ( rMin.x > rMax.x )
			grAlgo::Swap( rMin.x, rMax.x );
		if ( rMin.y > rMax.y )
			grAlgo::Swap( rMin.y, rMax.y );

		IsEqual = grMath::CmpV2f( rMin, rMax ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	inline void InitBaseMinMax( grV2f& rMinMax )
	{
		if ( rMinMax.x > rMinMax.y )
			grAlgo::Swap( rMinMax.x, rMinMax.y );

		IsEqual = grMath::CmpFloat( rMinMax.x, rMinMax.y ) ? EPartValueEqual::YES : EPartValueEqual::NO;
	}

	EPartValueEqual IsEqual;
};

struct grSColorGenerate : public grSBaseGenerate
{
	grSColorGenerate()
		: Rand( false )
	{}

	inline void Init( const grColor::SRgba& rStart, const grColor::SRgba& rEnd, const bool randomize )
	{
		ColorStart = rStart;
		ColorEnd = rEnd;
		Rand = randomize;
		InitBaseColor( ColorStart, ColorEnd );
	}

	inline void Generate( pU<SRgba[]>& rColorStart, pU<SRgba[]>& rColorEnd, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( Rand )
		{
			float midR{ grMath::AbsF( ColorStart.R - ColorEnd.R ) * 0.5f };
			float midG{ grMath::AbsF( ColorStart.G - ColorEnd.G ) * 0.5f };
			float midB{ grMath::AbsF( ColorStart.B - ColorEnd.B ) * 0.5f };
			//float midA = grMath::Abs( LocalStart.A - LocalEnd.A ) * 0.5f;		// Unsure if random alpha is desired

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grColor::SRgba result{ ColorStart.R > midR ? ( uint16_t )rRand->Float( midR, ColorStart.R ) : ( uint16_t )rRand->Float( ColorStart.R, midR ),
									   ColorStart.G > midG ? ( uint16_t )rRand->Float( midG, ColorStart.G ) : ( uint16_t )rRand->Float( ColorStart.G, midG ),
									   ColorStart.B > midB ? ( uint16_t )rRand->Float( midB, ColorStart.B ) : ( uint16_t )rRand->Float( ColorStart.B, midB ),
										( uint16_t )ColorStart.A };

				rColorStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grColor::SRgba result{ ColorEnd.R > midR ? ( uint16_t )rRand->Float( midR, ColorEnd.R ) : ( uint16_t )rRand->Float( ColorEnd.R, midR ),
									   ColorEnd.G > midG ? ( uint16_t )rRand->Float( midG, ColorEnd.G ) : ( uint16_t )rRand->Float( ColorEnd.G, midG ) ,
									   ColorEnd.B > midB ? ( uint16_t )rRand->Float( midB, ColorEnd.B ) : ( uint16_t )rRand->Float( ColorEnd.B, midB ),
										( uint16_t )ColorEnd.A };

				rColorEnd[ i ] = result;
			}

			return;
		}
		
		for ( sizeT i = startIdx; i < endIdx; ++i )
			rColorStart[ i ] = ColorStart;

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rColorEnd[ i ] = ColorEnd;
	}

	SRgba ColorStart, ColorEnd;
	bool Rand;
};

struct grSScaleGenerate : public grSBaseGenerate
{
	inline void Init( const grV2f& start, const grV2f& end )
	{
		ScaleStart = start;
		ScaleEnd = end;
		InitBaseStartEnd( ScaleStart, ScaleEnd );
	}

	inline void Generate( pU<grV2f[]>& rScaleStart, pU<grV2f[]>& rScaleEnd, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( IsEqual == EPartValueEqual::NO )
		{			
			float midX{ grMath::AbsF( ScaleStart.x - ScaleEnd.x ) * 0.5f };
			float midY{ grMath::AbsF( ScaleStart.y - ScaleEnd.y ) * 0.5f };

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( ScaleStart.x > midX ? rRand->Float( midX, ScaleStart.x ) : rRand->Float( ScaleStart.x, midX ),
							  ScaleStart.y > midY ? rRand->Float( midY, ScaleStart.y ) : rRand->Float( ScaleStart.y, midY ) );

				rScaleStart[ i ] = result;
			}

			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				grV2f result( ScaleEnd.x > midX ? rRand->Float( midX, ScaleEnd.x ) : rRand->Float( ScaleEnd.x, midX ),
							  ScaleEnd.y > midY ? rRand->Float( midY, ScaleEnd.y ) : rRand->Float( ScaleEnd.y, midY ) );

				rScaleEnd[ i ] = result;
			}

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rScaleStart[ i ] = ScaleStart;
	}

	grV2f ScaleStart, ScaleEnd;
};

struct grSForceGenerate : public grSBaseGenerate
{
	inline void Init( const grV2f& dirInDegMinMax, const grV2f& forceMinMax )
	{
		DirMinMax = grV2f( grMath::Clamp( dirInDegMinMax.x, 0.0f, 359.9f ), grMath::Clamp( dirInDegMinMax.y, 0.0f, 359.9f ) );
		ForceMinMax = forceMinMax;
		InitBaseMinMax ( ForceMinMax );
	}

	inline void Generate( pU<grV2f[]>& rAcceleration, pU<float[]>& rMass, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( IsEqual == EPartValueEqual::NO )
		{
			float d{ 0.0f };
			if ( DirMinMax.x > DirMinMax.y )
			{
				float diff{ 359.9f - DirMinMax.x };
				d = rRand->Float( 0.0f, diff + DirMinMax.y );
				d -= diff;
				if ( d < 0.0f )
					d = 359.9f + d;
			}
			else
			{
				d = rRand->Float( DirMinMax.x, DirMinMax.y );
			}

			float f{ rRand->Float( ForceMinMax.x, ForceMinMax.y ) };
			grV2f v{ grMath::DegToVec( d ) * f };
			for ( sizeT i = startIdx; i < endIdx; ++i )
			{
				float m{ rMass[ i ] };
				rAcceleration[ i ] += { v / m };
			}

			return;
		}

		grV2f v{ grMath::DegToVec( DirMinMax.x ) * ForceMinMax.x };
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			float m{ rMass[ i ] };
			rAcceleration[ i ] += { v / m };
		}
	}

	grV2f DirMinMax;
	grV2f ForceMinMax;
};

struct grSPositionGenerate : public grSBaseGenerate
{
	enum class EPosGenType
	{
		BOX = 0,
		ELLIPSE
	};

	inline void Init( const grV2f& min, const grV2f& max )
	{
		PosMin = min;
		PosMax = max;
		InitBaseMinMaxX2( PosMin, PosMax );
		PosType = EPosGenType::BOX;
	}

	inline void Init( const grV2f& min, const grV2f& max, const float step )
	{
		PosMin = min;
		PosMax = max;
		InitBaseMinMaxX2( PosMin, PosMax );
		PosType = EPosGenType::ELLIPSE;

		EllipseStepX = step;
		EllipseStepY = EllipseStepX;
		EllipseFull = grMath::Pi * 2.0f; // grMath::Pi * 2.0f = 360 degrees
		EllipseRad.x = PosMin.x;
		EllipseRad.y = PosMin.y;
	}

	inline void Generate( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( IsEqual == EPartValueEqual::NO )
		{
			if ( PosType == EPosGenType::BOX )
			{
				BoxEqualNo( rPosition, sysPos, startIdx, endIdx, rRand );
				return;
			}
			EllipseEqualNo( rPosition, sysPos, startIdx, endIdx, rRand, deltaT );
			return;
		}

		if ( PosType == EPosGenType::BOX )
		{
			BoxEqualYes( rPosition, sysPos, startIdx, endIdx );
			return;
		}
		EllipseEqualYes( rPosition, sysPos, startIdx, endIdx, rRand );
	}

	void BoxEqualNo( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			grV2f p{ rRand->V2fx2( PosMin, PosMax ) + sysPos };
			rPosition[ i ].position.x = p.x;
			rPosition[ i ].position.y = p.y;
		}
	}

	void BoxEqualYes( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			grV2f p{ PosMin + sysPos };
			rPosition[ i ].position.x = p.x;
			rPosition[ i ].position.y = p.y;
		}
	}

	void EllipseEqualNo( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		EllipseRad.x += EllipseStepX * deltaT;
		EllipseRad.y += EllipseStepY * deltaT;

		EllipseStepX *= ( EllipseRad.x < PosMin.x ) ? -1.0f : ( EllipseRad.x > PosMax.x ) ? -1.0f : 1.0f;
		EllipseStepY *= ( EllipseRad.y < PosMin.y ) ? -1.0f : ( EllipseRad.y > PosMax.y ) ? -1.0f : 1.0f;

		EllipseEqualYes( rPosition, sysPos, startIdx, endIdx, rRand );
	}

	void EllipseEqualYes( pU<sf::Vertex[]>& rPosition, const grV2f& sysPos, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		for ( sizeT i = startIdx; i < endIdx; ++i )
		{
			float a{ rRand->Float( 0.0f, EllipseFull ) };
			grV2f v{ EllipseRad.x * std::sin( a ), EllipseRad.y * std::cos( a ) };
			rPosition[ i ].position.x = v.x + sysPos.x;
			rPosition[ i ].position.y = v.y + sysPos.y;
		}
	}

	grV2f PosMin, PosMax;
	grV2f EllipseRad;
	float EllipseStepX, EllipseStepY, EllipseFull;
	EPosGenType PosType;
};

struct grSMassGenerate : public grSBaseGenerate
{
	inline void Init( const grV2f& rMinMax )
	{
		MassMinMax = rMinMax;
		if ( MassMinMax.x < 1.0f )
			MassMinMax.x = 1.0f;

		InitBaseMinMax( MassMinMax );
	}

	inline void Generate( pU<float[]>& rMass, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( IsEqual == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rMass[ i ] = rRand->Float( MassMinMax.x, MassMinMax.y );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rMass[ i ] = MassMinMax.x;
	}

	grV2f MassMinMax;
};

struct grSLifeGenerate : public grSBaseGenerate
{
	inline void Init( const grV2f& minMax )
	{
		LifeMinMax = minMax;
		if ( LifeMinMax.x < 0.0f )
			LifeMinMax.x = 0.0f;

		InitBaseMinMax( LifeMinMax );
	}

	inline void Generate( pU<float[]>& rLife, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand )
	{
		if ( IsEqual == EPartValueEqual::NO )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rLife[ i ] = rRand->Float( LifeMinMax.x, LifeMinMax.y );

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rLife[ i ] = LifeMinMax.x;
	}

	grV2f LifeMinMax;
};

enum class EParticleGenerateType
{
	COLOR = 0,
	SCALE,
	MASS,
	FORCE,
	POSITION,
	LIFE,
	SIZE
};

struct grSEmitter
{
	grSEmitter()
		: puRand( std::make_unique<grRandom>() )
		, puGenerateExist( std::make_unique<EParticleGenerateType[]>( ( sizeT )EParticleGenerateType::SIZE ) )
		, GenerateExistCount( 0 )
	{
		for ( sizeT i = 0; i < ( sizeT )EParticleGenerateType::SIZE; ++i )
			puGenerateExist[ i ] = EParticleGenerateType::SIZE;
	}
	grSEmitter( const grSEmitter& ) = delete;
	grSEmitter& operator=( const grSEmitter& ) = delete;
	grSEmitter( grSEmitter&& ) noexcept = delete;
	grSEmitter& operator=( grSEmitter&& ) noexcept = delete;

	void AddGenerate( EParticleGenerateType type )
	{
		switch ( type )
		{
			case EParticleGenerateType::COLOR:
			if ( puColor != nullptr ) break;
			puColor = std::make_unique<grSColorGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::COLOR;
			break;
			case EParticleGenerateType::SCALE:
			if ( puScale != nullptr ) break;
			puScale = std::make_unique<grSScaleGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::SCALE;
			break;
			case EParticleGenerateType::MASS:
			if ( puMass != nullptr ) break;
			puMass = std::make_unique<grSMassGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::MASS;
			break;
			case EParticleGenerateType::FORCE:
			if ( puForce != nullptr ) break;
			puForce = std::make_unique<grSForceGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::FORCE;
			break;
			case EParticleGenerateType::POSITION:
			if ( puPosition != nullptr ) break;
			puPosition = std::make_unique<grSPositionGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::POSITION;
			break;
			case EParticleGenerateType::LIFE:
			if ( puLife != nullptr ) break;
			puLife = std::make_unique<grSLifeGenerate>();
			puGenerateExist[ GenerateExistCount++ ] = EParticleGenerateType::LIFE;
			break;
		}
	}

	inline void Emit( pU<grSParticleVar>& rParticleData, pU<grSParticleArr>& rParticleArr, const float deltaT )
	{
		// This could be done smoother ex; NewSpawns = dt * SpawnRate
		// Problem is I haven't figured out how to make the spawnrate 'API' call non arbitrary in relation to real time, for example seconds

		// If spawns per frame are be greater than frame time
		sizeT emitAcc{ 0 };
		float emitRate{ rParticleData->EmitRate };
		float spawnAccT{ rParticleData->SpawnAccT };
		spawnAccT += deltaT;
		while ( spawnAccT >= emitRate )
		{
			spawnAccT -= emitRate;
			emitAcc += 1;
		}
		
		if ( emitAcc > 0 )
		{
			sizeT last{ rParticleData->Size - 1 };
			sizeT startIdx{ rParticleData->Alive };
			sizeT endIdx{ grMath::Min<sizeT>( startIdx + emitAcc, last ) };
			if ( startIdx == endIdx )
				return;

			grV2f sysPos{ rParticleData->SystemPosition };

			for ( sizeT i = 0; i < GenerateExistCount; ++i )
			{
				switch ( puGenerateExist[ i ] )
				{
					case EParticleGenerateType::COLOR:
					puColor->Generate( rParticleArr->puColorStart, rParticleArr->puColorEnd, startIdx, endIdx, puRand );
					break;
					case EParticleGenerateType::SCALE:
					puScale->Generate( rParticleArr->puScaleStart, rParticleArr->puScaleEnd, startIdx, endIdx, puRand );
					break;
					case EParticleGenerateType::MASS:
					puMass->Generate( rParticleArr->puMass, startIdx, endIdx, puRand );
					break;
					case EParticleGenerateType::FORCE:
					puForce->Generate( rParticleArr->puAcceleration, rParticleArr->puMass, startIdx, endIdx, puRand );
					break;
					case EParticleGenerateType::POSITION:
					puPosition->Generate( rParticleArr->puVerts, sysPos, startIdx, endIdx, puRand, deltaT );
					break;
					case EParticleGenerateType::LIFE:
					puLife->Generate( rParticleArr->puLife, startIdx, endIdx, puRand );
					break;
				}
			}

			rParticleData->SpawnAccT = spawnAccT;
			rParticleData->Alive += endIdx - startIdx;
		}
	}

	// All types of generators goes here
	// No slow virtual stuff allowed so each generator has it's own place
	pU<grSColorGenerate> puColor;
	pU<grSScaleGenerate> puScale;
	pU<grSMassGenerate> puMass;
	pU<grSForceGenerate> puForce;
	pU<grSPositionGenerate> puPosition;
	pU<grSLifeGenerate> puLife;

	pU<grRandom> puRand;	// Is slightly greater than 5mb so instead of each generator containing one it's passed. Is not thread safe so if that happens it needds to change

	pU<EParticleGenerateType[]> puGenerateExist;
	sizeT GenerateExistCount;
};


////////////////////////////////////////////////////////////////////////////////////////////////////

struct grSColorUpdate
{
	inline void SetHsv( const bool hsv)
	{
		Hsv = hsv;
	}

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

struct grSVelocityUpdate
{
	inline void Update( const sizeT alive, const float gravityF, const grV2f& rGravityV, const pU<float[]>& rMass, const pU<grV2f[]>& rAcceleration, pU<grV2f[]>& rVelocity, const float deltaT )
	{
		grV2f g{ rGravityV };
		for ( sizeT i = 0; i < alive; ++i )
		{
			float m{ rMass[ i ] };
			rAcceleration[ i ] += ( g / m ).LimitMax( gravityF );
		}

		float maxSpeed{ 1000.0f };	// TODO: Implement and move this correctly
		for ( sizeT i = 0; i < alive; ++i )
			rVelocity[ i ] = ( rAcceleration[ i ] ).LimitMax( maxSpeed );
	}
};

struct grSPositionUpdate
{
	inline void Update( const sizeT alive, pU<sf::Vertex[]>& rVerts, pU<grV2f[]>& rVelocity, const float deltaT )
	{
		for ( sizeT i = 0; i < alive; ++i )
		{
			auto v = rVelocity[ i ];

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
		grAlgo::Swap( rParticleArr->puMass[ nowIdx ], rParticleArr->puMass[ last ] );
		grAlgo::Swap( rParticleArr->puLife[ nowIdx ], rParticleArr->puLife[ last ] );

		// Most values are generated and set by operator= and does not need zeroing (all above)
		// Values that are generated and set by operator+= needs resetting obviously (all below)
		rParticleArr->puAcceleration[ last ] = { 0.0f, 0.0f };
	}
};

enum class EParticleUpdateType
{
	COLOR = 0,
	SCALE,
	VELOCITY,
	POSITION,
	LIFE,
	SIZE
};

struct grSUpdate
{
	grSUpdate( void )
		: puVelocity( std::make_unique<grSVelocityUpdate>() ) // Updaters that dont get created thru an API call in the system class gets created here
		, puPosition( std::make_unique<grSPositionUpdate>() )
		, puUpdateExist( std::make_unique<EParticleUpdateType[]>( ( sizeT )EParticleUpdateType::SIZE ) )
		, UpdateExistCount( 0 )
	{
		puUpdateExist[ UpdateExistCount++ ] = EParticleUpdateType::VELOCITY;
		puUpdateExist[ UpdateExistCount++ ] = EParticleUpdateType::POSITION;
		for ( sizeT i = 2; i < ( sizeT )EParticleUpdateType::SIZE; ++i )
			puUpdateExist[ i ] = EParticleUpdateType::SIZE;

		grAlgo::InsrtSort( puUpdateExist.get(), ( sizeT )EParticleUpdateType::SIZE );
	}
	grSUpdate( const grSUpdate& ) = delete;
	grSUpdate& operator=( const grSUpdate& ) = delete;
	grSUpdate( grSUpdate&& ) noexcept = delete;
	grSUpdate& operator=( grSUpdate&& ) noexcept = delete;

	inline void AddUpdate( EParticleUpdateType type )
	{
		switch ( type )
		{
			case EParticleUpdateType::COLOR:
			if ( puColor != nullptr ) break;
			puColor = std::make_unique<grSColorUpdate>();
			puUpdateExist[ UpdateExistCount++ ] = EParticleUpdateType::COLOR;
			break;

			case EParticleUpdateType::SCALE:
			if ( puScale != nullptr ) break;
			puScale = std::make_unique<grSScaleUpdate>();
			puUpdateExist[ UpdateExistCount++ ] = EParticleUpdateType::SCALE;
			break;

			case EParticleUpdateType::LIFE:
			if ( puLife != nullptr ) break;
			puLife = std::make_unique<grSLifeUpdate>();
			puUpdateExist[ UpdateExistCount++ ] = EParticleUpdateType::LIFE;
			break;
		}

		grAlgo::InsrtSort( puUpdateExist.get(), ( sizeT )EParticleUpdateType::SIZE );
	}

	inline void Update( pU<grSParticleVar>& rParticleVar, pU<grSParticleArr>& rParticleArr, const float deltaT )
	{
		grV2f gravityV{ rParticleVar->GravityV };
		float gravityF{ rParticleVar->GravityF };
		sizeT alive{ rParticleVar->Alive };

		for ( sizeT i = 0; i < UpdateExistCount; ++i )
		{
			switch ( puUpdateExist[ i ] )
			{
				case EParticleUpdateType::COLOR:
				puColor->Update( alive, rParticleArr->puColorStart, rParticleArr->puColorEnd, rParticleArr->puLife, rParticleArr->puVerts, deltaT );
				break;
				case EParticleUpdateType::SCALE:
				puScale->Update( alive, rParticleArr->puScaleStart, rParticleArr->puScaleEnd, rParticleArr->puLife, deltaT );
				break;
				case EParticleUpdateType::VELOCITY:
				puVelocity->Update( alive, gravityF, gravityV, rParticleArr->puMass, rParticleArr->puAcceleration, rParticleArr->puVelocity, deltaT );
				break;
				case EParticleUpdateType::POSITION:
				puPosition->Update( alive, rParticleArr->puVerts, rParticleArr->puVelocity, deltaT );
				break;
				case EParticleUpdateType::LIFE:
				alive = puLife->Update( alive, rParticleArr, deltaT );
				break;
			}
		}

		rParticleVar->Alive -= alive;
	}

	// No slow virtual stuff allowed so each updater has it's own place
	pU<grSColorUpdate> puColor;
	pU<grSScaleUpdate> puScale;
	pU<grSVelocityUpdate> puVelocity;
	pU<grSPositionUpdate> puPosition;
	pU<grSLifeUpdate> puLife;

	pU<EParticleUpdateType[]> puUpdateExist;
	sizeT UpdateExistCount;
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
	void SetMass( const grV2f& minMax );
	void SetLife( const grV2f& minMax );

	void AddGeneratorPositionBox( const grV2f& min, const grV2f& max ); // There can only be one type of position generator
	void AddGeneratorPositionEllipse( const grV2f& min, const grV2f& max, const float step = 0.0f );
	void AddGeneratorDirectionForce( const grV2f& dirInDegMinMax, const grV2f& forceMinMax );

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