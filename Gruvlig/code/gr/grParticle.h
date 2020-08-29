#ifndef _H_GRPARTICLE_
#define _H_GRPARTICLE_

#include "grParticleData.h"
#include "grParticleSystem.h"


// Interface class which will be returned on system creation as ref by the particle manager when that exists
// An id will be added here and instances of grCParticleData and grCParticleSystem will live in the manager and possible be stored as ref's here
// Update and render will be moved to the manager
class grCParticle
{
public:
	grCParticle( const sizeT size )
	{
		m_puData = std::make_unique<grCParticleData>( size );
		m_puSystem = std::make_unique<grCParticleSystem>( *m_puData );
	}
	grCParticle( const grCParticle& ) = delete;
	grCParticle& operator=( const grCParticle& ) = delete;
	grCParticle( grCParticle&& ) noexcept = delete;
	grCParticle& operator=( grCParticle&& ) noexcept = delete;

	void Start()
	{
		m_puSystem->EmissionSystem.Start();
	}

	void Stop()
	{
		m_puSystem->EmissionSystem.Stop();
	}

	void SetPositionSystem( const grV2f& rPosition )
	{
		m_puData->EmitData.SystemPosition = rPosition;
	}

	void SetEmission( const float emitRateSec, const float burstTimeSec = 0.0f )
	{
		m_puSystem->EmissionSystem.SetData( emitRateSec, burstTimeSec );
	}

	void SetColor( const grColor::Rgba& rStartMin, const grColor::Rgba& rStartMax, const grColor::Rgba& rEndMin, const grColor::Rgba& rEndMax, const bool hsv = true )
	{
		m_puSystem->ColorSystem.SetData( rStartMin, rStartMax, rEndMin, rEndMax, hsv );
	}

	void SetScale( const grV2f& rStartMin, const grV2f& rStartMax, const grV2f& rEndMin, const grV2f& rEndMax )
	{
		m_puSystem->ScaleSystem.SetData( rStartMin, rStartMax, rEndMin, rEndMax );
	}

	void SetVelocity( const grV2f& rDegreeMinMax, const grV2f& rForceMinMax )
	{
		m_puSystem->VelocitySystem.SetData( rDegreeMinMax, rForceMinMax );
	}

	void SetPositionBox( const grV2f& rOffsetMin, const grV2f& rOffsetMax, const float frameThickness = 0.0f )
	{
		m_puSystem->PositionSystem.SetBoxData( rOffsetMin, rOffsetMax, frameThickness );
	}

	void SetPositionEllipse( const grV2f& rRadiusMinMax )
	{
		m_puSystem->PositionSystem.SetEllipseData( rRadiusMinMax );
	}

	void SetMass( const grV2f& rMinMax )
	{
		m_puSystem->MassSystem.SetData( rMinMax );
	}

	void SetLife( const grV2f& rMinMax )
	{
		m_puSystem->LifeSystem.SetData( rMinMax );
	}

	void Update( const float dt )
	{
		//grStruct::grSTimer t( "Run", grStruct::grSTimer::ETimeType::MS );

		m_puSystem->Run( dt );

		//printf( "Alive: %d\n", m_puData->EmitData.Size );
	}

	void Render( sf::RenderWindow& rRenderWin )
	{
		m_puSystem->Render( rRenderWin );

		//// SCALE TEST
		//grSArrayData& rArray{ m_puData->ArrayData };
		//sizeT size{ m_puData->EmitData.Size };
		//for ( sizeT i = 0; i < size; ++i )
		//{
		//	grColor::Rgba& rgba = rArray.ColorStart[ i ];
		//	sf::Color c{ rgba.R, rgba.G, rgba.B, rgba.A };
		//	grBBox b{ rArray.ScaleStart[ i ], rArray.Position[ i ] };
		//	grDebugManager::Instance().AddBBox( b, c );
		//}
		////
	}

private:
	// TODO: Move this when/if a particle manager exists
	pU<grCParticleData> m_puData;
	pU<grCParticleSystem> m_puSystem;	
};

#endif // _H_GRPARTICLE_
