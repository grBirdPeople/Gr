#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const intU particleSize, const float spawnRate, const grV2f& position )
	: m_PositionSys		( grV2f() )
	, m_MaxParticleSize	( particleSize )
{
	// Particle data
	m_puParticle.reset( new grSParticle( m_MaxParticleSize ) );

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->m_upColor[ i ] = grSColor();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->m_upAcceleration[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->m_upVelocity[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->m_upPosition[ i ] = grV2f();

	for ( sizeT i = 0; i < m_MaxParticleSize; ++i )
		m_puParticle->m_upLife[ i ] = 0.0f;


	// Emitters & Updaters
	m_puEmitter.reset( new grSEmitter( spawnRate, m_MaxParticleSize ) );
	m_puUpdater.reset( new grSUpdater( m_MaxParticleSize ) );
}


grCParticleSys::~grCParticleSys( void )
{}


void
grCParticleSys::SetPosition( const grV2f& position )
{
	m_PositionSys = position;
}


// TODO: Seperate generator & updater into unique calls
void
grCParticleSys::CreateForceBasic( const grV2f& min, const grV2f& max )
{
	m_puEmitter->m_upForceBasicGen.reset( new grSForceBasicGenerator( min, max ) );
	m_puUpdater->m_upForceBasicUp.reset( new grSForceBasicUpdater() );
}


void
grCParticleSys::CreatePosition( const grV2f& min, const grV2f& max )
{
	m_puEmitter->m_upPositionGen.reset( new grSPositionGenerator( min, max ) );
}


// TODO: Seperate generator & updater into unique calls
void
grCParticleSys::CreateLife( const grV2f& minMax )
{
	m_puEmitter->m_upLifeGen.reset( new grSLifeGenerator( minMax ) );
	m_puUpdater->m_upLifeUp.reset( new grSLifeUpdater() );
}


// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

void
grCParticleSys::Update( const float deltaT )
{
	m_puEmitter->Emit( m_puParticle, m_PositionSys, deltaT );
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