#ifndef		_GRPARTICLETEST_H_
#define		_GRPARTICLETEST_H_

#include	"grCommon.h"
#include	"grV2.h"
#include	"grRandom.h"
#include	"grMath.h"
#include	"grAlgo.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSColor
{
	grSColor( void )
		: r( 255 ), g( 255 ), b( 255 ), a( 1.0f )
	{}

	float a;
	intU r, g, b;
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

	void SwapWithLast( const sizeT idx )
	{
		sizeT last = m_Alive - 1;
		grAlgo::Swap( m_upColor[ idx ],			m_upColor[ last ] );
		grAlgo::Swap( m_upAcceleration[ idx ],	m_upAcceleration[ last ] );
		grAlgo::Swap( m_upVelocity[ idx ],		m_upVelocity[ last ] );
		grAlgo::Swap( m_upPosition[ idx ],		m_upPosition[ last ] );
		grAlgo::Swap( m_upLife[ idx ],			m_upLife[ last ] );
	}

	pU<grSColor[]>	m_upColor;
	pU<grV2f[]>		m_upAcceleration;
	pU<grV2f[]>		m_upVelocity;
	pU<grV2f[]>		m_upPosition;
	pU<float[]>		m_upLife;

	sizeT	m_Alive;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSLifeGenerator
{
	grSLifeGenerator( const grV2f& minMax )
		: MinMax	( grV2f( minMax ) )
		, Same		( true )
	{
		if( grMath::CmpFloat( MinMax.x, MinMax.y ) == false )
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


struct grSPositionGenerator
{
	grSPositionGenerator( const grV2f& min, const grV2f& max )
		: Same	( true )
	{
		if ( grMath::CmpV2f( min, max ) == false )
			Same = !Same;

		// Make sure both of the min values are less then the max values for rand to work in the generate method
		Min = min;
		Max = max;
		if ( max.x < min.x )
		{
			Min.x = max.x;
			Max.x = min.x;
		}
		if ( max.y < min.y )
		{
			Min.y = max.y;
			Max.y = min.y;
		}
	}

	void Generate( pU<grSParticle>& rParticle, const sizeT startIdx, const sizeT endIdx, const pU<grRandom>& rRand, const float deltaT )
	{
		if ( Same )
		{
			for ( sizeT i = startIdx; i < endIdx + 1; ++i )
				rParticle->m_upPosition[ i ] = Max;

			return;
		}

		for ( sizeT i = startIdx; i < endIdx + 1; ++i )
			rParticle->m_upPosition[ i ] = grV2f( rRand->Float( Min.x, Max.x ), rRand->Float( Min.y, Max.y ) );
	}

	grV2f	Max, Min;
	bool	Same;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSEmitter
{
	grSEmitter( const float rate, const sizeT size )
		: Rate	( rate )
		, Size	( size )
	{
		m_PositionGen.reset( nullptr );
		m_LifeGen.reset( nullptr );
		m_puRand.reset( new grRandom() );
	}

	grSEmitter( const grSEmitter& ) = delete;
	grSEmitter& operator=( const grSEmitter& ) = delete;

	void Emit( pU<grSParticle>& rParticle, const float deltaT )
	{
		sizeT maxNew = ( sizeT )( Rate * (double)deltaT );
		sizeT startIdx = rParticle->m_Alive;
		sizeT endIdx = grMath::Min( rParticle->m_Alive + maxNew, Size - 1 );

		if ( m_LifeGen )		m_LifeGen->Generate( rParticle, startIdx, endIdx, m_puRand, deltaT );
		if ( m_PositionGen )	m_PositionGen->Generate( rParticle, startIdx, endIdx, m_puRand, deltaT );
	}

	// All types of generators goes here
	pU<grSLifeGenerator>		m_LifeGen;
	pU<grSPositionGenerator>	m_PositionGen;

	pU<grRandom>	m_puRand;
	float			Rate;
	sizeT			Size;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
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
				rParticle->SwapWithLast( i );
				--rParticle->m_Alive;
			}
		}
	}
};


struct grSPositionUpdater
{
	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{

	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct grSUpdater
{
	grSUpdater( const sizeT size )
		: Size	( size )
	{
		LifeUp.reset( nullptr );
	}

	grSUpdater( const grSUpdater& ) = delete;
	grSUpdater& operator=( const grSUpdater& ) = delete;

	void Update( pU<grSParticle>& rParticle, const float deltaT )
	{
		if( PositionUp )	PositionUp->Update( rParticle, deltaT );
		if( LifeUp )		LifeUp->Update( rParticle, deltaT );
	}

	// All types of updaters goes here
	pU<grSLifeUpdater>		LifeUp;
	pU<grSPositionUpdater>	PositionUp;

	sizeT	Size;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
class grCParticleSys
{
public:
	grCParticleSys( const intU particleSize, const float spawnRate );
	~grCParticleSys( void );

	grCParticleSys( const grCParticleSys& ) = delete;
	grCParticleSys& operator=( const grCParticleSys& ) = delete;

	// If min<->max are equal, life will be that. If not equal, life will be rand between min<->max
	void AddLifeGenerator( const grV2f& minMax );	
	void AddPositionGenerator( const grV2f& min, const grV2f& max );

	void Update( const float deltaT );

private:
	pU<grSParticle>	m_puParticle;
	pU<grSEmitter>	m_puEmitter;
	pU<grSUpdater>	m_puUpdater;

	sizeT	m_MaxSize;
};

#endif	// _GRPARTICLETEST_H_