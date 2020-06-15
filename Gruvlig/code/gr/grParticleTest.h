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
		: r( 255 )
		, g( 255 )
		, b( 255 )
		, a( 1.0f )
	{}

	float	a;
	intU	r, g, b;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSParticle
{
	grSParticle( const sizeT size )
		: m_Alive	( 0 )
	{
		m_upColor.reset( new grSColor[ size ] );
		m_upAcceleration.reset( new grV2f[ size ] );
		m_upVelocity.reset( new grV2f[ size ] );
		m_upPosition.reset( new grV2f[ size ] );
		m_upLife.reset( new float[ size ] );
	}

	grSParticle( const grSParticle& ) = delete;
	grSParticle& operator=( grSParticle& ) = delete;

	pU<grSColor[]>	m_upColor;
	pU<grV2f[]>		m_upAcceleration;
	pU<grV2f[]>		m_upVelocity;
	pU<grV2f[]>		m_upPosition;
	pU<float[]>		m_upLife;

	sizeT	m_Alive;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSForceBasicGenerator
{
	grSForceBasicGenerator( const grV2f& min, const grV2f& max )
		: Min	( min )
		, Max	( max )
		, Same	( true )
	{
		// Make sure both of the min values are less then the max values for rand to work in the generate method
		grMath::RangeCheckPairV2f( Min, Max );

		if ( grMath::CmpV2f( min, max ) == false )
			Same = !Same;
	}

	void Generate( pU<grSParticle>& rParticle, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( Same )
		{
			for ( sizeT i = startIdx; i < endIdx; ++i )
				rParticle->m_upAcceleration[ i ] = Min;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx; ++i )
			rParticle->m_upAcceleration[ i ] = rRand->V2f( Min, Max );
	}

	grV2f	Min, Max;
	bool	Same;
};


struct grSPositionGenerator
{
	grSPositionGenerator( const grV2f& min, const grV2f& max )
		: Min	( min )
		, Max	( max )
		, Same	( true )
	{
		// Make sure both of the min values are less then the max values for rand to work in the generate method
		grMath::RangeCheckPairV2f( Min, Max );

		if ( grMath::CmpV2f( min, max ) == false )
			Same = !Same;
	}

	void Generate( pU<grSParticle>& rParticle, const grV2f& positionalBasis, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( Same )
		{
			for ( sizeT i = startIdx; i < endIdx + 1; ++i )
				rParticle->m_upPosition[ i ] = Min + positionalBasis;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx + 1; ++i )
			rParticle->m_upPosition[ i ] = grV2f( rRand->Float( Min.x, Max.x ), rRand->Float( Min.y, Max.y ) ) + positionalBasis;
	}

	grV2f	Min, Max;
	bool	Same;
};


struct grSLifeGenerator
{
	grSLifeGenerator( const grV2f& minMax )
		: MinMax	( grV2f( minMax ) )
		, Same		( true )
	{
		if ( MinMax.x < 0.0f )
			MinMax.x = 0.0f;

		if ( MinMax.x > MinMax.y )
			MinMax.y = MinMax.x;

		if ( grMath::CmpFloat( MinMax.x, MinMax.y ) == false )
			Same = !Same;
	}

	void Generate( pU<grSParticle>& rParticle, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		rParticle->m_Alive += endIdx - startIdx;

		if ( Same )
		{
			for ( sizeT i = startIdx; i < endIdx + 1; ++i )
				rParticle->m_upLife[ i ] = MinMax.x;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx + 1; ++i )
			rParticle->m_upLife[ i ] = rRand->Float( MinMax.x, MinMax.y );
	}

	grV2f	MinMax;
	bool	Same;
};


struct grSEmitter
{
	grSEmitter( const float spawnRate, const sizeT size )
		: SpawnRate			( spawnRate )
		, MaxParticleSize	( size )
	{
		m_puRand.reset( new grRandom() );
	}

	grSEmitter( const grSEmitter& ) = delete;
	grSEmitter& operator=( const grSEmitter& ) = delete;

	void Emit( pU<grSParticle>& rParticle, const grV2f& positonalBasis, const float deltaT )
	{
		// Find out how many new particles to generate new data for
		sizeT maxNew, startIdx, endIdx;
		maxNew = ( sizeT )( SpawnRate * deltaT );	// Ok to ignore this warning? Seems to be generated because of the multiplie operator expecting doubles. Don't really want to cast
		startIdx = rParticle->m_Alive;
		endIdx = grMath::Min( rParticle->m_Alive + maxNew, MaxParticleSize - 1 );

		// TODO: Would be cool to not need conditionals. Do not wan't the indirections from a virtual interface base so some type of eventlist might be an option
		// Update all generators
		if ( m_upForceBasicGen )	m_upForceBasicGen->Generate( rParticle, startIdx, endIdx, m_puRand, deltaT );
		if ( m_upPositionGen )		m_upPositionGen->Generate( rParticle, positonalBasis, startIdx, endIdx, m_puRand, deltaT );
		if ( m_upLifeGen )			m_upLifeGen->Generate( rParticle, startIdx, endIdx, m_puRand, deltaT );
	}

	// No slow virtual stuff allowed so each generator has it's own place
	// If multiple generators of the same type can produce cool results change to arrays
	// All types of generators goes here
	pU<grSLifeGenerator>		m_upLifeGen;
	pU<grSForceBasicGenerator>	m_upForceBasicGen;
	pU<grSPositionGenerator>	m_upPositionGen;

	pU<grRandom>	m_puRand;

	float	SpawnRate;
	sizeT	MaxParticleSize;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSForceBasicUpdater
{
	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		for ( sizeT i = 0; i < rParticle->m_Alive; ++i )
			rParticle->m_upVelocity[ i ] += rParticle->m_upAcceleration[ i ] * deltaT;

		for ( sizeT i = 0; i < rParticle->m_Alive; ++i )
			rParticle->m_upPosition[ i ] += rParticle->m_upVelocity[ i ] * deltaT;

		// Reset all velocity values
		for ( sizeT i = 0; i < rParticle->m_Alive; ++i )
			rParticle->m_upVelocity[ i ] = grV2f();
	}
};


struct grSLifeUpdater
{
	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		for ( sizeT i = 0; i < rParticle->m_Alive; ++i )
		{
			rParticle->m_upLife[ i ] -= deltaT;
			if ( rParticle->m_upLife[ i ] <= 0.0f )
			{
				rParticle->m_upLife[ i ] = 0.0f;
				SwapNowWithLast( rParticle, i );
				--rParticle->m_Alive;
			}
		}
	}

	void SwapNowWithLast( pU<grSParticle>& rParticle, const sizeT nowIdx )
	{
		sizeT last = rParticle->m_Alive - 1;
		grAlgo::Swap( rParticle->m_upColor[ nowIdx ],			rParticle->m_upColor[ last ] );
		grAlgo::Swap( rParticle->m_upAcceleration[ nowIdx ],	rParticle->m_upAcceleration[ last ] );
		grAlgo::Swap( rParticle->m_upVelocity[ nowIdx ],		rParticle->m_upVelocity[ last ] );
		grAlgo::Swap( rParticle->m_upPosition[ nowIdx ],		rParticle->m_upPosition[ last ] );
		grAlgo::Swap( rParticle->m_upLife[ nowIdx ],			rParticle->m_upLife[ last ] );
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
		if ( m_upForceBasicUp )	m_upForceBasicUp->Update( rParticle, deltaT );
		if ( m_upLifeUp )		m_upLifeUp->Update( rParticle, deltaT );
	}

	// No slow virtual stuff allowed so each updater has it's own place
	// All types of updaters goes here
	pU<grSLifeUpdater>			m_upLifeUp;
	pU<grSForceBasicUpdater>	m_upForceBasicUp;

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

	// TODO: Implement this as a positional basis, ex; if the particles would spawn from a moving target
	grV2f	m_PositionSys;

	sizeT	m_MaxParticleSize;
};

#endif	// _GRPARTICLETEST_H_