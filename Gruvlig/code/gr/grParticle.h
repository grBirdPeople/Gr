#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleArray.h"
#include "grParticleComponent.h"
#include "grParticleSystem.h"


struct grSParticleData
{
	grSParticleData( const grV2f& systemPosition, const float emitRateSec, const sizeT size )
		: SystemPosition( systemPosition )
		, EmitRateSec( emitRateSec )
		, EmitRateMs( 1.0f / emitRateSec )
		, SpawnTimeAcc( 0.0f )
		, Size( size )
		, Alive( 0 )
	{}

	grV2f SystemPosition;
	float EmitRateSec;
	float EmitRateMs;
	float SpawnTimeAcc;
	sizeT Size;
	sizeT Alive;
};


class grCParticle
{
public:
	grCParticle( const grV2f& systemPosition = grV2f( 320.0f, 180.0f ), const float emitRateSec = 1000.0f, const intU size = 10000 )
		: m_Data( std::make_unique<grSParticleData>( systemPosition, emitRateSec, size ) )
		, m_Components( std::make_unique<grSParticleComponents>() )
		, m_Systems( std::make_unique<grSParticleSystems>() )
		, m_Arrays( std::make_unique<grSParticleArrays>( size ) )
	{}

	void SetSystemPosition( const grV2f& rPosition )
	{
		m_Data->SystemPosition = rPosition;
	}

	void AddLife( const grV2f& rMinMax )
	{
		if ( !m_Systems->Life )
		{
			m_Components->Life = std::make_unique<grSLifeComponent>();
			m_Systems->Life = std::make_unique<grSLifeSystem>( *m_Components->Life, *m_Arrays );
		}
		m_Systems->Life->Init( rMinMax );
	}

	void Run( const float dt )
	{
		Emit( dt );
		Update( dt );
	}

	void Render( sf::RenderWindow& rRenderWin )
	{

	}

private:
	void Emit( const float dt )
	{
		grSParticleData& data{ *m_Data };

		sizeT emitAcc{ 0 };
		data.SpawnTimeAcc += dt;
		while ( data.SpawnTimeAcc >= data.EmitRateMs )
		{
			data.SpawnTimeAcc -= data.EmitRateMs;
			emitAcc += 1;
		}

		if ( emitAcc > 0 )
		{
			sizeT last{ data.Size - 1 };
			sizeT startIdx{ data.Alive };
			sizeT endIdx{ grMath::Min<sizeT>( startIdx + emitAcc, last ) };
			if ( startIdx == endIdx )
				return;

			data.Alive += endIdx - startIdx;
			grV2f sysPos{ data.SystemPosition };

			// All system generate calls
			if ( m_Systems->Life ) m_Systems->Life->Generate( startIdx, endIdx );
		}
	}

	void Update( const float dt )
	{
		sizeT alive{ m_Data->Alive };
		if ( alive > 0 )
		{
			// All system update calls
			if ( m_Systems->Life ) m_Systems->Life->Update( alive, dt );
		}
		m_Data->Alive = alive;
	}

	pU<grSParticleData> m_Data;
	pU<grSParticleComponents> m_Components;
	pU<grSParticleSystems> m_Systems;
	pU<grSParticleArrays> m_Arrays;
};

#endif // _H_GRPARTICLE_
