#include	"grParticleSystem.h"

#include	"grBBox.h"
#include	"grDefine.h"
#include	"grDebugManager.h"

#include	"grMath.h"
#include	"grRandom.h"


// cTor
//////////////////////////////////////////////////
grParticleSystem::grParticleSystem( void )
	: m_SpawnsPerSec			( 4 )
	, m_SpawnInMilliSec			( 1.0f / m_SpawnsPerSec )
	, m_SpawnTimer				( m_SpawnInMilliSec )
	, m_HalfDirDiviationInDeg	( 0.0f )
	, m_ActiveParticles			( 0 )
	, m_bDirRandomDiviation		( false )
	, m_bGravityRandomRange		( false )
	, m_pRand					( new grRandom() )
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
	if ( m_pRand != nullptr )
	{
		DELANDNULL( m_pRand );
	}

	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		if ( m_VecParticles[ i ] != nullptr )
		{
			DELANDNULL( m_VecParticles[ i ] );
		}
	}
}


// Init
//////////////////////////////////////////////////
void
grParticleSystem::Init( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float spawnsPerSec )
{
	m_SpawnsPerSec		= spawnsPerSec;
	m_SpawnInMilliSec	= 1.0f / m_SpawnsPerSec;
	m_SpawnTimer		= m_SpawnInMilliSec;

	m_ParticleBlueprint.Position			= position;
	m_ParticleBlueprint.Direction			= direction;	// TODO: Sign it
	m_ParticleBlueprint.Velocity			= velocity;
	m_ParticleBlueprint.LifeTime			= lifetime;

	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		m_VecParticles[ i ]->Position		= m_ParticleBlueprint.Position;
		m_VecParticles[ i ]->Direction		= m_ParticleBlueprint.Direction;
		m_VecParticles[ i ]->Velocity		= m_ParticleBlueprint.Velocity;
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
			pTmp->Direction = m_ParticleBlueprint.Direction;
			pTmp->Velocity = m_ParticleBlueprint.Velocity;
			pTmp->LifeTime = m_ParticleBlueprint.LifeTime;
			++m_ActiveParticles;

			if ( m_bDirRandomDiviation == true )
			{
				float dirInDeg = grMath::VecToDeg( pTmp->Direction );
				float halfDiviationRand = m_pRand->GetRandFloat( -m_HalfDirDiviationInDeg, m_HalfDirDiviationInDeg );
				float newRadDir = halfDiviationRand * grMath::DegToRad;
				pTmp->Direction = grMath::RadToVec( newRadDir );

				int hej = 0;
			}
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
		grDebugManager::Instance().AddBBox( box, sf::Color::White );
		// TEST
	}
}
