#include	"grParticleSystem.h"

#include	"grBBox.h"
#include	"grDefine.h"
#include	"grDebugManager.h"
#include	"grRandom.h"


// cTor
//////////////////////////////////////////////////
grParticleSystem::grParticleSystem( void )
	: m_RandPosRange			( grV2f() )
	, m_SpawnsPerSec			( 4 )
	, m_SpawnInMilliSec			( 1.0f / m_SpawnsPerSec )
	, m_SpawnTimer				( m_SpawnInMilliSec )
	, m_RandSpreadRange			( 90.0f * 0.5f )
	, m_RandGravityRange		( 0.0f )
	, m_RandSpeedRange			( 0.0f )
	, m_RandSpeedChangeRange	( 0.0f )
	, m_SysRotInDeg				( 0.0f )
	, m_ActiveParticles			( 0 )
	, m_bRandPosition			( false )
	, m_bRandSpread				( true )
	, m_bRandGravity			( false )
	, m_bRandSpeed				( false )
	, m_bRandSpeedChange		( false )
	, m_pRandGen				( new grRandom() )
{
	m_ParticleBlueprint = new Particle();

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
	if ( m_pRandGen != nullptr )
	{
		DELANDNULL( m_pRandGen );
	}

	if ( m_ParticleBlueprint != nullptr )
	{
		DELANDNULL( m_ParticleBlueprint );
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
grParticleSystem::Init( const grV2f& position, const grV2f& direction, const float speed, const float lifetime, const float spawnsPerSec )
{
	m_SpawnsPerSec = spawnsPerSec;
	m_SpawnInMilliSec = 1.0f / m_SpawnsPerSec;
	m_SpawnTimer = m_SpawnInMilliSec;

	// TODO: Set some auto init deviation, like 45-90 deg

	m_ParticleBlueprint->Position = position;
	grV2f dir = direction;
	dir.Normalize();
	m_ParticleBlueprint->Direction = dir;
	m_ParticleBlueprint->Speed = speed;
	m_ParticleBlueprint->LifeTime = grMath::Abs( lifetime );

	for ( uInt i = 0; i < PARTICLE_QUANTITY; ++i )
	{
		m_VecParticles[ i ]->Position = m_ParticleBlueprint->Position;
		m_VecParticles[ i ]->Direction = m_ParticleBlueprint->Direction;
		m_VecParticles[ i ]->Speed = m_ParticleBlueprint->Speed;
		m_VecParticles[ i ]->LifeTime = m_ParticleBlueprint->LifeTime;
	}
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
			pTmp->Position = m_ParticleBlueprint->Position;
			//pTmp->Direction = m_ParticleBlueprint->Direction;
			//pTmp->Speed = m_ParticleBlueprint->Speed;
			//pTmp->VelocitySpeed = m_ParticleBlueprint->SpeedChange;
			pTmp->LifeTime = m_ParticleBlueprint->LifeTime;
			++m_ActiveParticles;

			// TEST
			// TODO: Make this into private functions

			// Rand spread // TESTED
			{
				if ( m_bRandSpread == true )
				{
					grV2f dir = m_ParticleBlueprint->Direction;
					float rand = m_pRandGen->GetRandFloat( -m_RandSpreadRange, m_RandSpreadRange ) * grMath::DegToRad;
					grMath::RotatePoint( &dir, rand );
					pTmp->Direction = dir;
				}
				else
				{
					pTmp->Direction = m_ParticleBlueprint->Direction;
				}
			}

			// Rand speed // TESTED
			{
				if ( m_bRandSpeed == true )
				{
					float randVel = m_pRandGen->GetRandFloat( -m_RandSpeedRange, m_RandSpeedRange );
					pTmp->Speed = m_ParticleBlueprint->Speed + randVel;
				}
				else
				{
					pTmp->Speed = m_ParticleBlueprint->Speed;
				}
			}

			// Rand speed change // TESTED
			{
				if ( m_bRandSpeedChange == true )
				{
					float randChange = m_pRandGen->GetRandFloat( -m_RandSpeedChangeRange, m_RandSpeedChangeRange );
					pTmp->SpeedChange = m_ParticleBlueprint->SpeedChange + randChange;
				}
				else
				{
					pTmp->SpeedChange = m_ParticleBlueprint->SpeedChange;
				}
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

		pTmp->Speed += pTmp->SpeedChange;
		pTmp->Position += pTmp->Direction * pTmp->Speed * deltaT;

		pTmp->LifeTime -= deltaT;



		//Particle* pTmp = m_VecParticles[ i ];

		//pTmp->Speed += pTmp->SpeedChange;
		//pTmp->Position += pTmp->Direction * pTmp->Speed * deltaT;

		//pTmp->LifeTime -= deltaT;

		// TEST
		grBBox box( grV2f( 1.0f, 1.0f ), pTmp->Position );
		grDebugManager::Instance().AddBBox( box, sf::Color::White );
		// TEST
	}
}
