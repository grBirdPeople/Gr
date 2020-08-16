#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleData.h"
#include "grParticleSystem.h"


class grCParticle
{
public:
	grCParticle( const grV2f& systemPosition = grV2f( ( float )grCore::Instance().GetWindowSize().x * 0.5f, ( float )grCore::Instance().GetWindowSize().y * 0.5f ),
				 const float emitRateSec = 1000.0f,
				 const intU size = 10000 )
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

	void SetEmitRate( const float emitRateSec )
	{
		m_Data.puEmit->EmitRateSec = emitRateSec;
		m_Data.puEmit->EmitRateMs = 1.0f / emitRateSec;
	}

	void AddScale( const grV2f& rStartMinMAx, const grV2f& rEndMinMax )
	{
		m_System.puScale->Init( rStartMinMAx, rEndMinMax );
	}

	void AddSpawnVelocity( const grV2f& rDegreesMinMax, const grV2f& rForceMinMax )
	{
		m_System.puVelocity->Init( rDegreesMinMax, rForceMinMax );
	}

	void AddPosition( const EPositionType type, const grV2f& rPositionOffsetMin, const grV2f& rPositionOffsetMax )
	{
		m_System.puPosition->Init( type, rPositionOffsetMin, rPositionOffsetMax ); // Only box now, ellipse later
	}

	void AddMass( const grV2f& rMassMinMax )
	{
		m_System.puMass->Init( rMassMinMax );
	}

	void AddLife( const grV2f& rLifeMinMax )
	{
		m_System.puLife->Init( rLifeMinMax );
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
			m_System.puScale->Generate();
			m_System.puMass->Generate();
			m_System.puVelocity->Generate();
			m_System.puPosition->Generate();
			m_System.puLife->Generate();
		}
	}

	void Update( const float dt )
	{
		if ( m_Data.puEmit->Alive > 0 )
		{
			// All system update calls
			m_System.puScale->Update();
			m_System.puVelocity->Update();
			m_System.puPosition->Update();
			m_System.puLife->Update();
		}

		//printf( "%d\n", m_Data.puEmit->Alive );
	}

	grSParticleData m_Data;
	grSParticleSystem m_System;
};

#endif // _H_GRPARTICLE_
