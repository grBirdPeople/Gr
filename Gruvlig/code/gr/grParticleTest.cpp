#include	"grParticleTest.h"

#include	"grCore.h"


grCParticleSys::grCParticleSys( const intU size, const float emitRate, const grV2f& position )
	: m_puParticle	( std::make_unique<grSParticle>( size ) )
	, m_puEmit		( std::make_unique<grSEmitter>( size, 1.0f / grMath::Abs( emitRate ), position ) )
	, m_puUpdate	( std::make_unique<grSUpdate>() )
{}

void
grCParticleSys::EmitRate( const float emitRate )
{
	m_puEmit->EmitRate = 1.0f / grMath::Abs( emitRate );
}

void
grCParticleSys::PositionSystem( const grV2f& position )
{
	m_puEmit->PositionSys = position;
}

void
grCParticleSys::Color( const grColor::SRgba& start, const grColor::SRgba& end, const bool hsv, const bool randomize )
{
	if ( m_puEmit->puColor == nullptr )
		m_puEmit->puColor = std::make_unique<grSColorGenerate>();

	m_puEmit->puColor->Set( start, end, randomize );

	if( m_puUpdate->puColor == nullptr )
		m_puUpdate->puColor = std::make_unique<grSColorUpdate>( hsv );
}

void
grCParticleSys::Scale( const grV2f& start, const grV2f& end )
{
	if ( m_puEmit->puScale == nullptr )
		m_puEmit->puScale = std::make_unique<grSScaleGenerate>();

	m_puEmit->puScale->Set( start, end );

	if( m_puEmit->puScale->Equal == EParticleEqual::NO )
		m_puUpdate->puScale = std::make_unique<grSScaleUpdate>();
}

void
grCParticleSys::Force( const grV2f& min, const grV2f& max )
{
	// const float dirMin, const float dirMax, const forceMin, const forceMax

	if ( m_puEmit->puForce == nullptr )
		m_puEmit->puForce = std::make_unique<grSForceGenerate>();

	m_puEmit->puForce->Set( min, max );

	if ( m_puUpdate->puForce == nullptr )
		m_puUpdate->puForce = std::make_unique<grSForceUpdate>();
}

void
grCParticleSys::Position( const grV2f& min, const grV2f& max )
{
	if( m_puEmit->puPosition == nullptr )
		m_puEmit->puPosition = std::make_unique<grSPositionGenerate>();

	m_puEmit->puPosition->Set( min, max );
}

void
grCParticleSys::Mass( const grV2f& minMax )
{
	if ( m_puEmit->puMass == nullptr )
		m_puEmit->puMass = std::make_unique<grSMassGenerate>();

	m_puEmit->puMass->Set( minMax );
}

void
grCParticleSys::Life( const grV2f& minMax )
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
	m_puEmit->Emit( m_puParticle, deltaT );
	m_puUpdate->Update( m_puParticle, deltaT );
}

void
grCParticleSys::Render( sf::RenderWindow& rRenderWin )
{
	rRenderWin.draw( &m_puParticle->puVerts.get()[ 0 ], m_puParticle->Alive, sf::PrimitiveType::Points );

	//printf( "Max: %d %2s Alive: %d \n", m_puEmit->Size, "", m_puParticle->Alive );





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
