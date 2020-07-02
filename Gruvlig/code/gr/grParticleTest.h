#ifndef		_GRPARTICLETEST_H_
#define		_GRPARTICLETEST_H_

#include	"grCommon.h"
#include	"grV2.h"
#include	"grRandom.h"
#include	"grMath.h"
#include	"grAlgo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////

//rPart.Color.r = rAtt.PartColMinMax.From.R;
//rPart.Color.g = rAtt.PartColMinMax.From.G;
//rPart.Color.b = rAtt.PartColMinMax.From.B;
//rPart.Color.a = rAtt.PartColMinMax.From.A;
//
//if ( rAtt.PartColMinMax.From != rAtt.PartColMinMax.Too )
//{
//	float r = ( float )rAtt.PartColMinMax.Too.R - ( float )rAtt.PartColMinMax.From.R;
//	float g = ( float )rAtt.PartColMinMax.Too.G - ( float )rAtt.PartColMinMax.From.G;
//	float b = ( float )rAtt.PartColMinMax.Too.B - ( float )rAtt.PartColMinMax.From.B;
//	float a = ( float )rAtt.PartColMinMax.Too.A - ( float )rAtt.PartColMinMax.From.A;
//
//	float t = rPart.Life * 60.0f;
//	rPart.ColRate.R = !grMath::CmpFloat( r, 0.0f ) ? r / t : 0.0f;
//	rPart.ColRate.G = !grMath::CmpFloat( g, 0.0f ) ? g / t : 0.0f;
//	rPart.ColRate.B = !grMath::CmpFloat( b, 0.0f ) ? b / t : 0.0f;
//	rPart.ColRate.A = !grMath::CmpFloat( a, 0.0f ) ? a / t : 0.0f;
//}


struct grSColor
{
	grSColor( void )
		: R( 255 )
		, G( 255 )
		, B( 255 )
		, A( 1.0f )
	{}

	float	A;
	intU	R, G, B;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSParticle
{
	grSParticle( const sizeT size )
		: Alive	( 0 )
	{
		puColor.reset( new grSColor[ size ] );
		puScale.reset( new grV2f[ size] );
		puAcceleration.reset( new grV2f[ size ] );
		puVelocity.reset( new grV2f[ size ] );
		puPosition.reset( new grV2f[ size ] );
		puMass.reset( new float[ size ] );
		puLife.reset( new float[ size ] );
	}

	grSParticle( const grSParticle& ) = delete;
	grSParticle& operator=( grSParticle& ) = delete;

	pU<grSColor[]>	puColor;
	pU<grV2f[]>		puScale;
	pU<grV2f[]>		puAcceleration;
	pU<grV2f[]>		puVelocity;
	pU<grV2f[]>		puPosition;
	pU<float[]>		puMass;
	pU<float[]>		puLife;

	sizeT	Alive;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSForceBasicGenerator
{
	grSForceBasicGenerator( const grV2f& min, const grV2f& max )
		: Min	( min )
		, Max	( max )
		, Equal	( true )
	{
		// Make sure both of the min values are less then the max values for rand to work in the generate method
		grMath::RangeCheckPairV2f( Min, Max );

		if ( grMath::CmpV2f( min, max ) == false )
			Equal = !Equal;
	}

	void Generate( pU<grSParticle>& rParticle, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( Equal )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rParticle->puAcceleration[ i ] += Min;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rParticle->puAcceleration[ i ] += rRand->V2fx2( Min, Max );
	}

	grV2f	Min, Max;
	bool	Equal;
};


struct grSPositionGenerator
{
	grSPositionGenerator( const grV2f& min, const grV2f& max )
		: Min	( min )
		, Max	( max )
		, Equal	( true )
	{
		// Make sure both of the min values are less then the max values for rand to work in the generate method
		grMath::RangeCheckPairV2f( Min, Max );

		if ( grMath::CmpV2f( min, max ) == false )
			Equal = !Equal;
	}

	void Generate( pU<grSParticle>& rParticle, const grV2f& positionalBasis, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( Equal )
		{
			for ( sizeT i = startIdx; i < endIdx + 1; ++i )
				rParticle->puPosition[ i ] = Min + positionalBasis;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx + 1; ++i )
			rParticle->puPosition[ i ] = grV2f( rRand->V2fx2( Min, Max ) ) + positionalBasis;
	}

	grV2f	Min, Max;
	bool	Equal;
};


struct grSLifeGenerator
{
	grSLifeGenerator( const grV2f& minMax )
		: MinMax	( grV2f( minMax ) )
		, Equal		( true )
	{
		if ( MinMax.x < 0.0f )
			MinMax.x = 0.0f;

		if ( MinMax.x > MinMax.y )
			MinMax.y = MinMax.x;

		if ( grMath::CmpFloat( MinMax.x, MinMax.y ) == false )
			Equal = !Equal;
	}

	void Generate( pU<grSParticle>& rParticle, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		rParticle->Alive += endIdx - startIdx;

		if ( Equal )
		{
			for ( sizeT i = startIdx; i < endIdx + 1; ++i )
				rParticle->puLife[ i ] = MinMax.x;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx + 1; ++i )
			rParticle->puLife[ i ] = rRand->Float( MinMax.x, MinMax.y );
	}

	grV2f	MinMax;
	bool	Equal;
};


struct grSEmitter
{
	grSEmitter( const float spawnRate, const sizeT size )
		: SpawnRate			( spawnRate )
		, SpawnAccT			( 0.0f )
		, NewSpawns			( 0 )
		, MaxParticleSize	( size )
	{
		puRand.reset( new grRandom() );
	}

	grSEmitter( const grSEmitter& ) = delete;
	grSEmitter& operator=( const grSEmitter& ) = delete;

	void Emit( pU<grSParticle>& rParticle, const grV2f& positionalBasis, const float deltaT )
	{
		SpawnAccT += deltaT;
		while ( SpawnAccT >= SpawnRate )
		{
			SpawnAccT -= SpawnRate;
			NewSpawns += 1;
		}

		if ( NewSpawns > 0 )
		{
			// Find array indices
			sizeT startIdx, endIdx;
			startIdx = rParticle->Alive;
			endIdx = grMath::Min( rParticle->Alive + NewSpawns, MaxParticleSize - 1 );

			// Update all generators
			if ( puForceBasicGen )	puForceBasicGen->Generate( rParticle, startIdx, endIdx, puRand, deltaT );
			if ( puPositionGen )	puPositionGen->Generate( rParticle, positionalBasis, startIdx, endIdx, puRand, deltaT );
			if ( puLifeGen )		puLifeGen->Generate( rParticle, startIdx, endIdx, puRand, deltaT );
		}

		NewSpawns = 0;
	}

	// All types of generators goes here
	// No slow virtual stuff allowed so each generator has it's own place
	// If multiple generators of the same type would produce cool results perhaps change to arrays
	pU<grSLifeGenerator>		puLifeGen;
	pU<grSForceBasicGenerator>	puForceBasicGen;
	pU<grSPositionGenerator>	puPositionGen;

	pU<grRandom>	puRand;

	float	SpawnRate,
			SpawnAccT;
	sizeT	NewSpawns,
			MaxParticleSize;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSForceBasicUpdater
{
	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		for ( sizeT i = 0; i < rParticle->Alive; ++i )
			rParticle->puVelocity[ i ] = rParticle->puAcceleration[ i ];
	}
};


struct grSLifeUpdater
{
	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		for ( sizeT i = 0; i < rParticle->Alive; ++i )
		{
			rParticle->puLife[ i ] -= deltaT;
			if ( rParticle->puLife[ i ] <= 0.0f )
			{
				Kill( rParticle, i );
				--rParticle->Alive;
			}
		}
	}

	void Kill( pU<grSParticle>& rParticle, const sizeT nowIdx )
	{
		//for( sizeT i = 0; i < rParticle->Alive; ++i )
		//	printf( "%g \n", rParticle->puLife[ i ] );

		sizeT last = rParticle->Alive - 1;

		rParticle->puColor[ nowIdx ] = grSColor();
		rParticle->puScale[ nowIdx ] = grV2f();
		rParticle->puAcceleration[ nowIdx ] = grV2f();
		rParticle->puVelocity[ nowIdx ] = grV2f();
		rParticle->puPosition[ nowIdx ] = grV2f();
		rParticle->puMass[ nowIdx ] = 0.0f;
		rParticle->puLife[ nowIdx ] = 0.0f;

		grAlgo::Swap( rParticle->puColor[ nowIdx ],			rParticle->puColor[ last ] );
		grAlgo::Swap( rParticle->puScale[ nowIdx ],			rParticle->puScale[ last ] );
		grAlgo::Swap( rParticle->puAcceleration[ nowIdx ],	rParticle->puAcceleration[ last ] );
		grAlgo::Swap( rParticle->puVelocity[ nowIdx ],		rParticle->puVelocity[ last ] );
		grAlgo::Swap( rParticle->puPosition[ nowIdx ],		rParticle->puPosition[ last ] );
		grAlgo::Swap( rParticle->puMass[ nowIdx ],			rParticle->puMass[ last ] );
		grAlgo::Swap( rParticle->puLife[ nowIdx ],			rParticle->puLife[ last ] );


	}
};


struct grSUpdater
{
	grSUpdater( const sizeT size )
		: MaxParticleSize	( size )
	{}

	grSUpdater( const grSUpdater& ) = delete;
	grSUpdater& operator=( const grSUpdater& ) = delete;

	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		// TODO: Would be cool to not need conditionals. Do not wan't the indirections from a virtual interface base so some type of eventlist might be an option
		// Update all updaters
		for ( sizeT i = 0; i < rParticle->Alive; ++i )
			rParticle->puPosition[ i ] += rParticle->puVelocity[ i ] * deltaT;

		if ( upForceBasicUp )	upForceBasicUp->Update( rParticle, deltaT );
		if ( upLifeUp )			upLifeUp->Update( rParticle, deltaT );
	}

	// No slow virtual stuff allowed so each updater has it's own place
	// All types of updaters goes here
	pU<grSLifeUpdater>			upLifeUp;
	pU<grSForceBasicUpdater>	upForceBasicUp;

	sizeT	MaxParticleSize;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
class grCParticleSys
{
public:
	grCParticleSys( const intU particleSize = 1000, const float spawnRate = 150.0f, const grV2f& position = grV2f( 320.0f, 180.0f ) );
	~grCParticleSys( void );

	grCParticleSys( const grCParticleSys& ) = delete;
	grCParticleSys& operator=( const grCParticleSys& ) = delete;

	void SetSpawnRate( const float spawnRate );
	void SetPosition( const grV2f& position );

	// TODO: Could be better to use injection here as that would make it possible store and share behaviours externaly
	// If min<->max are equal, the value will be that. If not equal, the value will be rand inbetween
	void CreateForceBasic( const grV2f& min, const grV2f& max );
	void CreatePosition( const grV2f& min, const grV2f& max );
	void CreateLife( const grV2f& minMax );

	void Update( const float deltaT );

private:
	pU<grSParticle>	m_puParticle;
	pU<grSEmitter>	m_puEmitter;
	pU<grSUpdater>	m_puUpdater;

	grV2f	m_SysPosition;

	sizeT	m_MaxParticleSize;
};

#endif	// _GRPARTICLETEST_H_