#include	"grParticleTest.h"


grCParticleSys::grCParticleSys( const intU particleSize, const float spawnRate )
	: m_MaxParticleSize			( particleSize )
	, m_TrackedGeneratorSize	( 0 )
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


	// Tracker for which generators that have been added. This exists to avoid using if statement in the runtime loop
	sizeT size = ( sizeT )EGeneratorTypes::SIZE;
	m_puTrackedGenerator.reset( new EGeneratorTypes[ size ] );
	for( sizeT i = 0; i < size; ++i )
		m_puTrackedGenerator[ i ] = EGeneratorTypes::SIZE;
}


grCParticleSys::~grCParticleSys( void )
{}


void grCParticleSys::AddLifeGenerator( const grV2f& minMax )
{
	m_puEmitter->m_LifeGen.reset( new grSLifeGenerator( minMax ) );
	m_puUpdater->LifeUp.reset( new grSLifeUpdater() );
	AddTrackedGenerator( EGeneratorTypes::LIFE );
}


void
grCParticleSys::AddPositionGenerator( const grV2f& min, const grV2f& max )
{
	m_puEmitter->m_PositionGen.reset( new grSPositionGenerator( min, max ) );
	m_puUpdater->PositionUp.reset( new grSPositionUpdater() );
	AddTrackedGenerator( EGeneratorTypes::POSITION );
}


// TEST DRAW
#include	"grBBox.h"
#include	"grDebugManager.h"
// TEST DRAW

void
grCParticleSys::Update( const float deltaT )
{
	m_puEmitter->Emit( m_puParticle, m_puTrackedGenerator, m_TrackedGeneratorSize, deltaT );
	m_puUpdater->Update( m_puParticle, m_puTrackedGenerator, m_TrackedGeneratorSize, deltaT );


	// TEST DRAW
	printf( "%s %d \n", "PartsAlive: ", m_puParticle->m_Alive );
	for ( sizeT idx = 0; idx < m_puParticle->m_Alive; ++idx )
	{
		grBBox box( grV2f( 5.5f, 5.5f ), m_puParticle->m_upPosition[ idx ] );
		grDebugManager::Instance().AddBBox( box, sf::Color::Green );
	}
	// TEST DRAW
}


void grCParticleSys::AddTrackedGenerator( const EGeneratorTypes type )
{
	if ( m_puTrackedGenerator[ ( sizeT )type ] == EGeneratorTypes::SIZE )
	{
		m_puTrackedGenerator[ ( sizeT )type ] = type;
		++m_TrackedGeneratorSize;
	}
}
