#include	"grParticleTest.h"

#include	"grCore.h"


grCParticleSys::grCParticleSys( const intU size, const float emitRate, const grV2f& position )
	: m_puParticleArr( std::make_unique<grSParticleArr>( size ) )
	, m_puParticleVar( std::make_unique<grSParticleVar>() )
	, m_puEmit( std::make_unique<grSEmitter>() )
	, m_puUpdate( std::make_unique<grSUpdate>() )
{
	float e{ emitRate };
	if( e < 0.0f )
		e = 0.0f;

	m_puParticleVar->SystemPosition = position;
	m_puParticleVar->EmitRateSec = e;
	m_puParticleVar->EmitRate = 1.0f / e;
	m_puParticleVar->SpawnAccT = 0.0f;
	m_puParticleVar->Size = size;
}

float
grCParticleSys::GetEmitRate() const
{
	return m_puParticleVar->EmitRateSec;
}

void
grCParticleSys::SetEmitRate( const float emitRate )
{
	float e{ emitRate };
	if( e < 0.0f )
		e = 0.0f;

	m_puParticleVar->EmitRateSec = e;
	m_puParticleVar->EmitRate = 1.0f / e;
}

void
grCParticleSys::SetSystemPosition( const grV2f& position )
{
	m_puParticleVar->SystemPosition = position;
}

void
grCParticleSys::SetGravity( const float force, const float direction )
{
	float d{ grMath::Clamp( direction, 0.0f, 360.0f ) };
	m_puParticleVar->GravityV = { grMath::DegToVec( d ) * force };
	m_puParticleVar->GravityF = force;
}

void
grCParticleSys::SetColor( const grColor::SRgba& start, const grColor::SRgba& end, const bool hsv, const bool randomize )
{
	if ( m_puEmit->puColor == nullptr )
		m_puEmit->puColor = std::make_unique<grSColorGenerate>();

	m_puEmit->puColor->Set( start, end, randomize );

	if( m_puUpdate->puColor == nullptr )
		m_puUpdate->puColor = std::make_unique<grSColorUpdate>( hsv );
}

void
grCParticleSys::SetScale( const grV2f& start, const grV2f& end )
{
	if ( m_puEmit->puScale == nullptr )
		m_puEmit->puScale = std::make_unique<grSScaleGenerate>();

	m_puEmit->puScale->Set( start, end );

	if( m_puEmit->puScale->Equal == EPartValueEqual::NO )
		m_puUpdate->puScale = std::make_unique<grSScaleUpdate>();
}

void
grCParticleSys::SetForce( const grV2f& min, const grV2f& max )
{
	// wind or something of the likes: const grV2f& dirMinMax, const grV2f& forceMinMax, const grV2f& timeMinMax

	if ( m_puEmit->puForce == nullptr )
		m_puEmit->puForce = std::make_unique<grSForceGenerate>();

	m_puEmit->puForce->Set( min, max );

	if ( m_puUpdate->puForce == nullptr )
		m_puUpdate->puForce = std::make_unique<grSForceUpdate>();
}

void
grCParticleSys::SetPosition( const grV2f& min, const grV2f& max )
{
	if( m_puEmit->puPosition == nullptr )
		m_puEmit->puPosition = std::make_unique<grSPositionGenerate>();

	m_puEmit->puPosition->Set( min, max );
}

void
grCParticleSys::SetMass( const grV2f& minMax )
{
	if ( m_puEmit->puMass == nullptr )
		m_puEmit->puMass = std::make_unique<grSMassGenerate>();

	m_puEmit->puMass->Set( minMax );
}

void
grCParticleSys::SetLife( const grV2f& minMax )
{
	if ( m_puEmit->puLife == nullptr )
		m_puEmit->puLife = std::make_unique<grSLifeGenerate>();

	m_puEmit->puLife->Set( minMax );

	if ( m_puUpdate->puLife == nullptr )
		m_puUpdate->puLife = std::make_unique<grSLifeUpdate>();
}

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

void
grCParticleSys::Update( const float deltaT )
{
	m_puEmit->Emit( m_puParticleVar, m_puParticleArr, deltaT );
	m_puUpdate->Update( m_puParticleVar, m_puParticleArr, deltaT );
}

void
grCParticleSys::Render( sf::RenderWindow& rRenderWin )
{
	rRenderWin.draw( &m_puParticleArr->puVerts.get()[ 0 ], m_puParticleVar->Alive, sf::PrimitiveType::Points );

	//printf( "Max: %d %2s Alive: %d \n", m_puParticleData->Size, "", m_puParticleData->Alive );
	




	//// TEST DRAW
	//printf( "Max: %d %2s Alive: %d \n", m_puEmit->Size, "", m_puParticle->Alive );
	//for ( sizeT i = 0; i < m_puParticle->Alive; ++i )
	//{
	//	grBBox box( m_puParticle->puScaleStart[ i ], m_puParticle->puPosition[ i ] );
	//	grDebugManager::Instance().AddBBox( box, sf::Color( ( sf::Uint8 )m_puParticle->puColorStart[ i ].R,
	//														( sf::Uint8 )m_puParticle->puColorStart[ i ].G,
	//														( sf::Uint8 )m_puParticle->puColorStart[ i ].B,
	//														( sf::Uint8 )m_puParticle->puColorStart[ i ].A ) );
	//}
	//// TEST DRAW
}
