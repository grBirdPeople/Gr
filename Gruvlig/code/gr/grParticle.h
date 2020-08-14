#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleData.h"
#include "grParticleSystem.h"


class grCParticle
{
public:
	grCParticle( const grV2f& systemPosition = grV2f( 320.0f, 180.0f ), const float emitRateSec = 500.0f, const intU size = 10000 )
	{
		m_Data.Init( systemPosition, emitRateSec, size );
		m_System.Init( m_Data );
	}
	grCParticle( const grCParticle& ) = delete;
	grCParticle& operator=( const grCParticle& ) = delete;
	grCParticle( grCParticle&& ) noexcept = delete;
	grCParticle& operator=( grCParticle&& ) noexcept = delete;

	void SetSystemPosition( const grV2f& rPosition )
	{
		m_Data.puEmit->SystemPosition = rPosition;
	}

	void AddSpawnVelocity()
	{

	}

	void AddPosition( const EPositionType type, const grV2f& rPositionOffsetMin = grV2f(), const grV2f& rPositionOffsetMax = grV2f() )
	{
		//if ( !m_System.Position )
		//{
		//	m_Data.Position = std::make_unique<grSPositionData>();
		//	m_System.Position = std::make_unique<grSPositionSystem>( *m_Data.Emit, *m_Data.Position, *m_Data.Array );
		//}
		//m_System.Position->Init( type, rPositionOffsetMin, rPositionOffsetMax );
	}

	void AddLife( const grV2f& rMinMax )
	{
		//if ( !m_System.Life )
		//{
		//	m_Data.Life = std::make_unique<grSLifeData>();
		//	m_System.Life = std::make_unique<grSLifeSystem>( *m_Data.Emit, *m_Data.Life, *m_Data.Array );
		//}
		//m_System.Life->Init( rMinMax ); // Only box for now, ellipse later
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
		grSEmitData& emit{ *m_Data.puEmit };
		emit.Dt = dt;
		emit.SpawnTimeAcc += emit.Dt;
		while ( emit.SpawnTimeAcc >= emit.EmitRateMs )
		{
			emit.SpawnTimeAcc -= emit.EmitRateMs;
			emitAcc += 1;
		}

		if ( emitAcc > 0 )
		{
			sizeT last{ emit.Size - 1 };
			emit.StartIdx = emit.Alive;
			emit.EndIdx = grMath::Min<sizeT>( emit.StartIdx + emitAcc, last );
			if ( emit.StartIdx == emit.EndIdx )
				return;

			emit.Alive += emit.EndIdx - emit.StartIdx;

			// All system generate calls
			if ( m_System.puVelocity ) m_System.puVelocity->Generate();
			if( m_System.puPosition ) m_System.puPosition->Generate();
			if ( m_System.puLife ) m_System.puLife->Generate();
		}
	}

	void Update( const float dt )
	{
		if ( m_Data.puEmit->Alive > 0 )
		{
			// All system update calls
			if ( m_System.puVelocity ) m_System.puVelocity->Update();
			if ( m_System.puPosition ) m_System.puPosition->Update();
			if ( m_System.puLife ) m_System.puLife->Update();
		}

		printf( "%d\n", m_Data.puEmit->Alive );
	}

	grSParticleData m_Data;
	grSParticleSystem m_System;
};

#endif // _H_GRPARTICLE_
