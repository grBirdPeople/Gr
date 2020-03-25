#include	"grParticleSystem.h"

#include	"grBBox.h"
#include	"grDefine.h"
#include	"grDebugManager.h"
#include	"grRandom.h"


// cTor
//////////////////////////////////////////////////
grParticleSystem::grParticleSystem( void )
	: m_RandPosRange				( grV2f() )
	, m_SpawnsPerSec				( 4 )
	, m_SpawnInMilliSec				( 1.0f / m_SpawnsPerSec )
	, m_SpawnTimer					( m_SpawnInMilliSec )
	, m_RandDirDiviationInDeg		( 0.0f )
	, m_RandGravityRange			( 0.0f )
	, m_RandVelocityRange			( 0.0f )
	, m_RandVelocityChangeRange		( 0.0f )
	, m_ActiveParticles				( 0 )
	, m_bPosRandRange				( false )
	, m_bDirRandDiviation			( false )
	, m_bGravityRandRange			( false )
	, m_bVelocityRandRange			( false )
	, m_bVelocityChangeRandRange	( false )
	, m_pRand						( new grRandom() )
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
	m_SpawnsPerSec = spawnsPerSec;
	m_SpawnInMilliSec = 1.0f / m_SpawnsPerSec;
	m_SpawnTimer = m_SpawnInMilliSec;

	m_ParticleBlueprint.Position = position;
	m_ParticleBlueprint.Direction = direction;	// TODO: Sign it
	m_ParticleBlueprint.Velocity = velocity;
	m_ParticleBlueprint.LifeTime = lifetime;

	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		m_VecParticles[ i ]->Position = m_ParticleBlueprint.Position;
		m_VecParticles[ i ]->Direction = m_ParticleBlueprint.Direction;
		m_VecParticles[ i ]->Velocity = m_ParticleBlueprint.Velocity;
		m_VecParticles[ i ]->LifeTime = m_ParticleBlueprint.LifeTime;
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
			pTmp->VelocityChange = m_ParticleBlueprint.VelocityChange;
			pTmp->LifeTime = m_ParticleBlueprint.LifeTime;
			++m_ActiveParticles;

			// TEST
			if ( m_bDirRandDiviation == true )
			{
				float dirInDeg = grMath::VecToDeg( pTmp->Direction );
				float halfDiviationRand = m_pRand->GetRandFloat( -m_RandDirDiviationInDeg, m_RandDirDiviationInDeg );
				float newRadDir = halfDiviationRand * grMath::DegToRad;
				pTmp->Direction = grMath::RadToVec( newRadDir );
			}

			if ( m_bVelocityRandRange == true )
			{
				float randVel = m_pRand->GetRandFloat( -m_RandVelocityRange, m_RandVelocityRange );
				pTmp->Velocity += randVel;
			}

			if ( m_bVelocityChangeRandRange == true )
			{
				float randChange = m_pRand->GetRandFloat( -m_RandVelocityChangeRange, m_RandVelocityChangeRange );
				pTmp->VelocityChange += randChange;
			}
			// TEST
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
		pTmp->Velocity += pTmp->VelocityChange;
		pTmp->Position += pTmp->Direction * pTmp->Velocity * deltaT;
		pTmp->LifeTime -= deltaT;

		// TEST
		grBBox box( grV2f( 5.0f, 5.0f ), pTmp->Position );
		grDebugManager::Instance().AddBBox( box, sf::Color::White );
		// TEST
	}
}
