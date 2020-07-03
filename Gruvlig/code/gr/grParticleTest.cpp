#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const intU particleSize, const float spawnRate, const grV2f& position )
	: m_SysPosition		( grV2f() )
	, m_MaxParticleSize	( particleSize )
{
	// Particle data
	m_puParticle.reset( new grSParticle( m_MaxParticleSize ) );

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puColor[ i ] = grSColor();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puScale[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puAcceleration[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puVelocity[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puPosition[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puMass[ i ] = 0.0f;

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->puLife[ i ] = 0.0f;


	// Emitters & Updaters
	m_puEmitter.reset( new grSEmitter( 1.0f / grMath::Abs( spawnRate ), m_MaxParticleSize ) );
	m_puUpdater.reset( new grSUpdater( m_MaxParticleSize ) );
}


grCParticleSys::~grCParticleSys( void )
{}


void
grCParticleSys::SetSpawnRate( const float spawnRate )
{
	m_puEmitter->SpawnRate = 1.0f / grMath::Abs( spawnRate );
}


void
grCParticleSys::SetPosition( const grV2f& position )
{
	m_SysPosition = position;
}


// TODO: Seperate generator & updater into unique calls
void
grCParticleSys::CreateForceBasic( const grV2f& min, const grV2f& max )
{
	m_puEmitter->puForceBasicGen.reset( new grSForceBasicGenerator( min, max ) );
	m_puUpdater->upForceBasicUp.reset( new grSForceBasicUpdater() );
}


void
grCParticleSys::CreatePosition( const grV2f& min, const grV2f& max )
{
	m_puEmitter->puPositionGen.reset( new grSPositionGenerator( min, max ) );
}


// TODO: Seperate generator & updater into unique calls
void
grCParticleSys::CreateLife( const grV2f& minMax )
{
	m_puEmitter->puLifeGen.reset( new grSLifeGenerator( minMax ) );
	m_puUpdater->upLifeUp.reset( new grSLifeUpdater() );
}


// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

void
grCParticleSys::Update( const float deltaT )
{
	m_puEmitter->Emit( m_puParticle, m_SysPosition, deltaT );
	m_puUpdater->Update( m_puParticle, deltaT );


	// TEST DRAW
	//printf( "Max: %d %2s Alive: %d \n", m_MaxParticleSize, "", m_puParticle->Alive );
	for ( sizeT idx = 0; idx < m_puParticle->Alive; ++idx )
	{
		grBBox box( grV2f( 1.5f, 1.5f ), m_puParticle->puPosition[ idx ] );
		grDebugManager::Instance().AddBBox( box, sf::Color::White );
	}
	// TEST DRAW
}