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
		m_Data.Init( size );
		m_System.Init( m_Data );
		m_System.puEmit->Init( systemPosition, emitRateSec, size );
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
		m_Data.puEmit->SpawnTimeAcc = m_Data.puEmit->EmitRateMs;
	}

	void AddColor( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv = true )
	{
		m_System.puColor->Init( rStartMin, rStartMax, rEndMin, rEndMax, hsv );
	}

	void AddScale( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{
		m_System.puScale->Init( rStartMin, rStartMax, rEndMin, rEndMax );
	}

	void AddVelocity( const grV2f& rDegreeMinMax, const grV2f& rForceMinMax )
	{
		m_System.puVelocity->Init( rDegreeMinMax, rForceMinMax );
	}

	void AddPosition( const EPositionType type, const grV2f& rOffsetMin, const grV2f& rOffsetMax )
	{
		m_System.puPosition->Init( type, rOffsetMin, rOffsetMax ); // Only box now, ellipse later
	}

	void AddMass( const grV2f& rMinMax )
	{
		m_System.puMass->Init( rMinMax );
	}

	void AddLife( const grV2f& rMinMax )
	{
		m_System.puLife->Init( rMinMax );
	}

	void Run( const float dt )
	{
		m_System.Generate( dt );
		m_System.Update();
	}

	void Render( sf::RenderWindow& rRenderWin )
	{
		// TEST DRAW
		grSEmitData& rEmit{ *m_Data.puEmit };
		grSArrayData& rArray{ *m_Data.puArray };
		for ( sizeT i = 0; i < rEmit.Alive; ++i )
		{
			grColor::Rgba& rgba = rArray.ColorStart[ i ];
			sf::Color c{ rgba.R, rgba.G, rgba.B, rgba.A };
			grBBox b{ rArray.ScaleStart[ i ], rArray.Position[ i ] };
			grDebugManager::Instance().AddBBox( b, c );
		}
		// TEST DRAW
	}

private:
	grSParticleData m_Data;
	grSParticleSystem m_System;
};

#endif // _H_GRPARTICLE_
