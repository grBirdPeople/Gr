#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const intU particleSize, const float emitRate, const grV2f& position )
	: m_puParticle		( std::make_unique<grSParticle>( particleSize ) )
	, m_puEmitter		( std::make_unique<grSEmitter>( 1.0f / grMath::Abs( emitRate ), particleSize ) )
	, m_puUpdater		( std::make_unique<grSUpdater>() )
{}


void
grCParticleSys::PositionSys( const grV2f& position )
{
	m_puEmitter->PositionSys = position;
}


void
grCParticleSys::EmitRate( const float emitRate )
{
	m_puEmitter->EmitRate = 1.0f / grMath::Abs( emitRate );
}


void
grCParticleSys::Life( const grV2f& minMax )
{
	if( m_puEmitter->puLife == nullptr )
		m_puEmitter->puLife = std::make_unique<grSLifeGenerator>();

	m_puEmitter->puLife->Set( minMax );

	if ( m_puUpdater->puLife == nullptr )
		m_puUpdater->puLife = std::make_unique<grSLifeUpdater>();
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

	if( m_puUpdater->puPosition == nullptr )
		m_puUpdater->puPosition = std::make_unique<grSPositionUpdater>();
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
	printf( "Max: %d %2s Alive: %d \n", m_puEmitter->MaxParticleSize, "", m_puParticle->Alive );
	for ( sizeT idx = 0; idx < m_puParticle->Alive; ++idx )
	{
		grBBox box( grV2f( 1.5f, 1.5f ), m_puParticle->puPosition[ idx ] );
		grDebugManager::Instance().AddBBox( box, sf::Color::White );
	}
	// TEST DRAW
}