#ifndef		_H_GRPARTICLESYSTEM_
#define		_H_GRPARTICLESYSTEM_

#define		PARTICLE_QUANTITY	500		// TODO: Some time in the future, make this to be individual per particle system, based on spawn per sec over lifetime, or something

#include	"grCommon.h"
#include	"grMath.h"
#include	"grV2.h"

class		grRandom;


// grParticleSystem
//////////////////////////////////////////////////
class grParticleSystem
{
public:

	struct Particle
	{
		Particle( void )
		{
			Position = grV2f();
			Direction = grV2f();
			Gravity = grV2f();
			Velocity = 0.0f;
			VelocityChange = 0.0f;
			LifeTime = 0.0f;
		}

		// TODO: Would be cleaner to add rands here instead of in class... but particles would get heavier... Unsure of what's best
		grV2f Position;
		grV2f Direction;
		grV2f Gravity;
		float Velocity;
		float VelocityChange;
		float LifeTime;
	};

	//////////////////////////////////////////////////

	grParticleSystem( void );
	~grParticleSystem( void );

	//////////////////////////////////////////////////

	inline void SetPosition( const grV2f& rPos, const grV2f& randomRange = grV2f() )
	{
		m_ParticleBlueprint.Position = rPos;
		SetPositionRange( randomRange );
	}
	inline void SetDirection( const grV2f& direction, const float randomRangeInDeg = 0.0f )
	{
		grV2f dir = direction;
		m_ParticleBlueprint.Direction = grV2f( grMath::Sign( dir.x ), grMath::Sign( dir.y ) );
		SetDirectionRange( randomRangeInDeg );
	}
	inline void SetGravity( const grV2f& rGravity, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint.Gravity = rGravity;
		SetGravityRange( randomRange );
	}
	inline void	SetVelocity( const float velocity, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint.Velocity = velocity;
		SetVelocityRange( randomRange );
	}
	inline void SetVelocityChange( const float velocityChange, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint.VelocityChange = velocityChange;
		SetVelocityChangeRange( randomRange );
	}

	//////////////////////////////////////////////////

	inline void SetPositionRange( const grV2f& randomRange )
	{
		grV2f range = randomRange;
		m_bRandPosition = range != grV2f() ? true : false;
		if ( m_bRandPosition == true ) { m_RandPosRange = range * 0.5f; }
	}
	inline void SetDirectionRange( const float deviationInDeg )
	{
		float deviation = grMath::Abs( deviationInDeg );
		m_bRandDirDiviation = deviation > 0.0f ? true : false;
		if ( m_bRandDirDiviation == true )
		{
			m_RandDiviationInDeg = grMath::Clamp( deviation, 0.0f, 360.0f ) * 0.5f;
		}
	}
	inline void SetGravityRange( const float randomRange )
	{
		float range = randomRange;
		m_bRandGravity = range != 0.0f ? true : false;
		if ( m_bRandGravity == true ) { m_RandGravityRange = grMath::Abs( range ) * 0.5f; }
	}
	inline void SetVelocityRange( const float randomRange )
	{
		float range = grMath::Clamp( randomRange, 0.0f, grMath::Abs( randomRange ) );
		m_bRandVelocity = ( range > 0.0f ) ? true : false;
		if ( m_bRandVelocity == true )
		{
			m_RandVelocityRange = range * 0.5f;
		}
		else
		{
			m_RandVelocityRange = 0.0f;
		}
	}
	inline void SetVelocityChangeRange( const float randomRange )
	{
		float range = grMath::Abs( randomRange );
		m_bRandVelocityChange = ( range > 0.0f ) ? true : false;
		if ( m_bRandVelocityChange == true ) { m_RandVelocityChangeRange = range * 0.5f; }
	}

	// TODO: Implement set lifetime

	//////////////////////////////////////////////////

	inline float GetDirectionRange( void )
	{
		return m_RandDiviationInDeg * 2.0f;
	}
	inline float GetVelocityRange( void )
	{
		return m_RandVelocityRange * 2.0f;
	}

	//////////////////////////////////////////////////

	void Init ( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float m_SpawnsPerSec );
	void Update ( const float deltaT );

	//////////////////////////////////////////////////

private:

	void DeactivateParticle( void );
	void ActivateParticle( const float deltaT );
	void UpdateParticle( const float deltaT );

	//////////////////////////////////////////////////

	std::vector<Particle*>	m_VecParticles;

	Particle				m_ParticleBlueprint;

	grV2f					m_RandPosRange;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_RandDiviationInDeg,
							m_RandGravityRange,
							m_RandVelocityRange,
							m_RandVelocityChangeRange;

	uInt					m_ActiveParticles;

	bool					m_bRandPosition,
							m_bRandDirDiviation,
							m_bRandGravity,
							m_bRandVelocity,
							m_bRandVelocityChange;

	grRandom* m_pRand;

};

#endif	// _H_GRPARTICLESYSTEM_
