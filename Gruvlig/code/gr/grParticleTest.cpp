#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const intU particleSize, const float spawnRate )
	: m_MaxSize	( particleSize )
{
	m_puParticle.reset( new grSParticle( m_MaxSize ) );

	for ( sizeT i = 0; i < m_MaxSize; ++i )
		m_puParticle->m_upColor[ i ] = grSColor();

	for ( sizeT i = 0; i < m_MaxSize; ++i )
		m_puParticle->m_upAcceleration[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxSize; ++i )
		m_puParticle->m_upVelocity[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxSize; ++i )
		m_puParticle->m_upPosition[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxSize; ++i )
		m_puParticle->m_upLife[ i ] = 0.0f;


	m_puEmitter.reset( new grSEmitter( spawnRate, m_MaxSize ) );
	m_puUpdater.reset( new grSUpdater( m_MaxSize ) );
}


grCParticleSys::~grCParticleSys( void )
{}


void grCParticleSys::AddLifeGenerator( const grV2f& minMax )
{
	m_puEmitter->m_LifeGen.reset( new grSLifeGenerator( minMax ) );
	m_puUpdater->LifeUp.reset( new grSLifeUpdater() );
}


void
grCParticleSys::AddPositionGenerator( const grV2f& min, const grV2f& max )
{
	m_puEmitter->m_PositionGen.reset( new grSPositionGenerator( min, max ) );
	m_puUpdater->PositionUp.reset( new grSPositionUpdater() );
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
	printf( "%s %d \n", "PartsAlive: ", m_puParticle->m_Alive );
	for ( sizeT idx = 0; idx < m_puParticle->m_Alive; ++idx )
	{
		grBBox box( grV2f( 5.5f, 5.5f ), m_puParticle->m_upPosition[ idx ] );
		grDebugManager::Instance().AddBBox( box, sf::Color::Green );
	}
	// TEST DRAW
}
