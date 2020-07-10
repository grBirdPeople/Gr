#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const grV2f& position, const float emitRate, const intU size )
	: m_puParticle	( std::make_unique<grSParticle>( size ) )
	, m_puEmitter	( std::make_unique<grSEmitter>( position, 1.0f / grMath::Abs( emitRate ), size ) )
	, m_puUpdater	( std::make_unique<grSUpdater>() )
{}

void
grCParticleSys::EmitRate( const float emitRate )
{
	m_puEmitter->EmitRate = 1.0f / grMath::Abs( emitRate );
}

void
grCParticleSys::PositionSys( const grV2f& position )
{
	m_puEmitter->PositionSys = position;
}

void
grCParticleSys::Color( const grColor::SRgba& start, const grColor::SRgba& end, const bool hsv, const bool randomize )
{
	if ( m_puEmitter->puColor == nullptr )
		m_puEmitter->puColor = std::make_unique<grSColorGenerator>();

	m_puEmitter->puColor->Set( start, end, randomize );

	if( m_puUpdater->puColor == nullptr )
		m_puUpdater->puColor = std::make_unique<grSColorUpdater>( hsv );
}

void
grCParticleSys::ForceBasic( const grV2f& min, const grV2f& max )
{
	if( m_puEmitter->puForceBasic == nullptr )
		m_puEmitter->puForceBasic = std::make_unique<grSForceBasicGenerator>();

	m_puEmitter->puForceBasic->Set( min, max );

	if ( m_puUpdater->puForceBasic == nullptr )
		m_puUpdater->puForceBasic = std::make_unique<grSForceBasicUpdater>();
}

void
grCParticleSys::PositionOffset( const grV2f& min, const grV2f& max )
{
	if( m_puEmitter->puPosition == nullptr )
		m_puEmitter->puPosition = std::make_unique<grSPositionGenerator>();

	m_puEmitter->puPosition->Set( min, max );
}

void
grCParticleSys::Mass( const grV2f& minMax )
{
	if ( m_puEmitter->puMass == nullptr )
		m_puEmitter->puMass = std::make_unique<grSMassGenerator>();

	m_puEmitter->puMass->Set( minMax );
}

void
grCParticleSys::Life( const grV2f& minMax )
{
	if ( m_puEmitter->puLife == nullptr )
		m_puEmitter->puLife = std::make_unique<grSLifeGenerator>();

	m_puEmitter->puLife->Set( minMax );

	if ( m_puUpdater->puLife == nullptr )
		m_puUpdater->puLife = std::make_unique<grSLifeUpdater>();
}

// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

void
grCParticleSys::Update( const float deltaT )
{
	m_puEmitter->Emit( m_puParticle, deltaT );
	m_puUpdater->Update( m_puParticle, deltaT );

	// TEST DRAW
	//printf( "Max: %d %2s Alive: %d \n", m_puEmitter->Size, "", m_puParticle->Alive );
	for ( sizeT i = 0; i < m_puParticle->Alive; ++i )
	{
		grBBox box( grV2f( 1.5f, 1.5f ), m_puParticle->puPosition[ i ] );
		grDebugManager::Instance().AddBBox( box, sf::Color( ( sf::Uint8 )m_puParticle->puColorStart[ i ].R,
															( sf::Uint8 )m_puParticle->puColorStart[ i ].G,
															( sf::Uint8 )m_puParticle->puColorStart[ i ].B,
															( sf::Uint8 )m_puParticle->puColorStart[ i ].A ) );
	}
	// TEST DRAW
}