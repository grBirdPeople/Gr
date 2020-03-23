#include	"grParticleSystem.h"

#include	"grBBox.h"
#include	"grDefine.h"
#include	"grDebugManager.h"


// cTor
//////////////////////////////////////////////////
grParticleSystem::grParticleSystem( void )
	: m_SpawnsPerSec	( 4 )
	, m_SpawnInMilliSec	( 1.0f / m_SpawnsPerSec )
	, m_SpawnTimer		( m_SpawnInMilliSec )
	, m_ActiveParticles	( 0 )
{
	m_VecParticles.reserve( PARTICLE_QUANTITY );
	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		m_VecParticles.push_back( new Particle() );
	}
}


// dTor
//////////////////////////////////////////////////
grParticleSystem::~grParticleSystem( void )
{
	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		DELANDNULL( m_VecParticles[ i ] );
	}
}


// Init
//////////////////////////////////////////////////
void
grParticleSystem::Init( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float spawnsPerSec )
{
	m_SpawnsPerSec = spawnsPerSec;
	m_SpawnInMilliSec = 1.0f / m_SpawnsPerSec;
	m_SpawnTimer = m_SpawnInMilliSec;

	m_ParticleBlueprint.Position			= position;
	m_ParticleBlueprint.Direction			= direction;
	//m_ParticleBlueprint.Gravity			= 
	m_ParticleBlueprint.Velocity			= velocity;
	//m_ParticleBlueprint.VelocityChange	= 
	m_ParticleBlueprint.LifeTime			= lifetime;

	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		m_VecParticles[ i ]->Position		= m_ParticleBlueprint.Position;
		m_VecParticles[ i ]->Direction		= m_ParticleBlueprint.Direction;
		//m_VecParticles[ i ]->Gravity		= m_ParticleBlueprint.Gravity;
		m_VecParticles[ i ]->Velocity		= m_ParticleBlueprint.Velocity;
		//m_VecParticles[ i ]->VelocityChange	= m_ParticleBlueprint.VelocityChange;
		m_VecParticles[ i ]->LifeTime		= m_ParticleBlueprint.LifeTime;
	}
}


// Update
//////////////////////////////////////////////////
void
grParticleSystem::Update( const float deltaT )
{
	DeactivateParticle();
	ActivateParticle( deltaT );
	UpdateParticle( deltaT );
}


// DeactivateParticle
//////////////////////////////////////////////////
void
grParticleSystem::DeactivateParticle( void )
{
	for ( sInt i = 0; i < m_ActiveParticles; ++i )
	{
		if ( m_VecParticles[ i ]->LifeTime < 0.0f )
		{
			--m_ActiveParticles;
			if ( i == m_ActiveParticles || m_ActiveParticles == PARTICLE_QUANTITY )
			{
				continue;
			}

			Particle* pTmp = m_VecParticles[ i ];
			m_VecParticles[ i ] = m_VecParticles[ m_ActiveParticles ];
			m_VecParticles[ m_ActiveParticles ] = pTmp;
			--i;
		}
	}
}


// ActivateParticle
//////////////////////////////////////////////////
void
grParticleSystem::ActivateParticle( const float deltaT )
{
	m_SpawnTimer -= deltaT;
	if ( m_SpawnTimer < 0.0f )
	{
		m_SpawnTimer += m_SpawnInMilliSec;
		if ( m_ActiveParticles < PARTICLE_QUANTITY )
		{
			Particle* pTmp = m_VecParticles[ m_ActiveParticles ];
			pTmp->Position = m_ParticleBlueprint.Position;
			pTmp->LifeTime = m_ParticleBlueprint.LifeTime;
			++m_ActiveParticles;
		}
	}
}


// UpdateParticle
//////////////////////////////////////////////////
void
grParticleSystem::UpdateParticle( const float deltaT )
{
	for ( uInt i = 0; i < m_ActiveParticles; ++i )
	{
		Particle* pTmp = m_VecParticles[ i ];
		pTmp->Position += pTmp->Direction * pTmp->Velocity * deltaT;
		pTmp->LifeTime -= deltaT;

		// TEST
		grBBox box( grV2f( 5.0f, 5.0f ), pTmp->Position );
		grDebugManager::Instance().AddBBox( box, sf::Color::Yellow );
		// TEST
	}
}
