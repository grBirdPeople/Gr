#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleData.h"
#include "grParticleSystem.h"


class grCParticle
{
public:
	grCParticle( const intU size = 10000 )
	{
		m_puData = std::make_unique<grCParticleData>( size );
		m_puSystem = std::make_unique<grCParticleSystem>( *m_puData );
	}
	grCParticle( const grCParticle& ) = delete;
	grCParticle& operator=( const grCParticle& ) = delete;
	grCParticle( grCParticle&& ) noexcept = delete;
	grCParticle& operator=( grCParticle&& ) noexcept = delete;

	void SetSystemPosition( const grV2f& rPosition )
	{
		m_puData->EmitData.SystemPosition = rPosition;
	}

	void SetEmitRate( const float emitRateSec )
	{
		m_puData->EmitData.EmitRateSec = emitRateSec;
		m_puData->EmitData.EmitRateMs = 1.0f / emitRateSec;
		m_puData->EmitData.SpawnTimeAcc = m_puData->EmitData.EmitRateMs;
	}

	void AddColor( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv = true )
	{
		m_puSystem->ColorSystem.Init( rStartMin, rStartMax, rEndMin, rEndMax, hsv );
	}

	void AddScale( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{
		m_puSystem->ScaleSystem.Init( rStartMin, rStartMax, rEndMin, rEndMax );
	}

	void AddVelocity( const grV2f& rDegreeMinMax, const grV2f& rForceMinMax )
	{
		m_puSystem->VelocitySystem.Init( rDegreeMinMax, rForceMinMax );
	}

	void AddPositionBox( const grV2f& rOffsetMin, const grV2f& rOffsetMax, const float frameThickness = 0.0f )
	{
		m_puSystem->PositionSystem.InitBox( rOffsetMin, rOffsetMax, frameThickness );
	}

	void AddPositionEllipse( const grV2f& rRadiusMinMax )
	{
		m_puSystem->PositionSystem.InitCircle( rRadiusMinMax );
	}

	void AddMass( const grV2f& rMinMax )
	{
		m_puSystem->MassSystem.Init( rMinMax );
	}

	void AddLife( const grV2f& rMinMax )
	{
		m_puSystem->LifeSystem.Init( rMinMax );
	}

	void Update( const float dt )
	{
		m_puData->EmitData.Dt = dt;
		m_puSystem->Run();
	}

	void Render( sf::RenderWindow& rRenderWin )
	{
		// TEST DRAW
		grSArrayData& rArray{ m_puData->ArrayData };
		sizeT alive{ m_puData->EmitData.Alive };
		for ( sizeT i = 0; i < alive; ++i )
		{
			grColor::Rgba& rgba = rArray.ColorStart[ i ];
			sf::Color c{ rgba.R, rgba.G, rgba.B, rgba.A };
			grBBox b{ rArray.ScaleStart[ i ], rArray.Position[ i ] };
			grDebugManager::Instance().AddBBox( b, c );
		}

		//printf( "%d\n", alive );
		// TEST DRAW
	}

private:
	pU<grCParticleData> m_puData;
	pU<grCParticleSystem> m_puSystem;
};

#endif // _H_GRPARTICLE_
