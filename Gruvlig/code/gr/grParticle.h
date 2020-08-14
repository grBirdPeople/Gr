#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleData.h"
#include "grParticleSystem.h"


class grCParticle
{
public:
	grCParticle( const grV2f& systemPosition = grV2f( 320.0f, 180.0f ), const float emitRateSec = 500.0f, const intU size = 10000 )
		: m_Data( systemPosition, emitRateSec, size )
	{}
	grCParticle( const grCParticle& ) = delete;
	grCParticle& operator=( const grCParticle& ) = delete;
	grCParticle( grCParticle&& ) noexcept = delete;
	grCParticle& operator=( grCParticle&& ) noexcept = delete;

	void SetSystemPosition( const grV2f& rPosition )
	{
		m_Data.Emit->SystemPosition = rPosition;
	}

	void AddLife( const grV2f& rMinMax )
	{
		if ( !m_System.Life )
		{
			m_Data.Life = std::make_unique<grSLifeData>();
			m_System.Life = std::make_unique<grSLifeSystem>( *m_Data.Life, *m_Data.Array );
		}
		m_System.Life->Init( rMinMax );
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
		sizeT emitAcc{ 0 };
		grSEmitData& emit{ *m_Data.Emit };
		emit.SpawnTimeAcc += dt;
		while ( emit.SpawnTimeAcc >= emit.EmitRateMs )
		{
			emit.SpawnTimeAcc -= emit.EmitRateMs;
			emitAcc += 1;
		}

		if ( emitAcc > 0 )
		{
			sizeT last{ emit.Size - 1 };
			sizeT startIdx{ emit.Alive };
			sizeT endIdx{ grMath::Min<sizeT>( startIdx + emitAcc, last ) };
			if ( startIdx == endIdx )
				return;

			emit.Alive += endIdx - startIdx;
			grV2f sysPos{ emit.SystemPosition };

			// All system generate calls
			if ( m_System.Life ) m_System.Life->Generate( startIdx, endIdx );
		}
	}

	void Update( const float dt )
	{
		grSEmitData& emit{ *m_Data.Emit };
		sizeT alive{ emit.Alive };
		if ( alive > 0 )
		{
			// All system update calls
			if ( m_System.Life ) m_System.Life->Update( alive, dt );
		}
		emit.Alive = alive;		
	}

	grSParticleData m_Data;
	grSParticleSystem m_System;
};

#endif // _H_GRPARTICLE_
