#ifndef		_H_GRPARTICLESYSTEM_
#define		_H_GRPARTICLESYSTEM_

#define		PARTICLE_QUANTITY	500		// TODO: Some time in the future, make this to be individual per particle system, based on spawn per sec over lifetime, or something

#include	"grCommon.h"
#include	"grMath.h"
#include	"grV2.h"
#include	"grParticleManager.h"

class		grRandom;


// grParticleSystem
//////////////////////////////////////////////////
class grParticleSystem
{
public:

	friend class grParticleManager;

	//////////////////////////////////////////////////

	grParticleSystem( void );
	~grParticleSystem( void );

	//////////////////////////////////////////////////

	// TODO: Implement position things
	// TODO: Implement direction things
	// TODO: Implement lifetime things

	inline void SetPosition( const grV2f& rPos, const grV2f& randomRange = grV2f() )
	{
		m_ParticleBlueprint->Position = rPos;
		SetPositionRange( randomRange );
	}
	inline void SetDirection( const grV2f& direction, const float randomRangeInDeg = 0.0f )
	{
		grV2f dir = direction;
		if ( ( direction.x < -1.0f || direction.x > 1.0f ) ||
			( direction.y < -1.0f || direction.y > 1.0f ) )
		{
			dir.Normalize();
		}
		m_ParticleBlueprint->Direction = dir;
		//SetDirectionRange( randomRangeInDeg );
	}
	inline void SetGravity( const grV2f& rGravity, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint->Gravity = rGravity;
		SetGravityRange( randomRange );
	}
	inline void	SetSpeed( const float velocity, const float randomRange = 0.0f )
	{
		float vel = velocity;
		m_ParticleBlueprint->Speed = grMath::Clamp( vel, 0.0f, grMath::Abs( vel ) );
		SetSpeedRange( randomRange );
	}
	inline void SetSpeedChange( const float velocityChange, const float randomRange = 0.0f )
	{
		float change = velocityChange;
		m_ParticleBlueprint->SpeedChange = grMath::Clamp( change, 0.0f, grMath::Abs( change ) );
		SetSpeedChangeRange( randomRange );
	}

	//////////////////////////////////////////////////

	inline void SetPositionRange( const grV2f& randomRange )
	{
		grV2f range = randomRange;
		m_bRandPosition = range != grV2f() ? true : false;
		if ( m_bRandPosition == true )
		{
			m_RandPosRange = range * 0.5f;
		}
	}
	inline void SetDirectionRange( const float deviationInDeg )
	{
		float deviation = deviationInDeg;
		deviation = grMath::Clamp( deviationInDeg, 0.0f, 360.0f );
		if ( grMath::CmpFloat( 0.0f, deviation ) == true )
		{
			deviation = 0.0f;
		}
		m_bRandDirDiviation = ( deviation > 0.0f ) ? true : false;
		m_RandDiviationInDeg = ( m_bRandDirDiviation == true ) ? deviation * 0.5f : deviation;
	}
	inline void SetGravityRange( const float randomRange )
	{
		float range = randomRange;
		if ( grMath::CmpFloat( 0.0f, range ) == true )
		{
			range = 0.0f;
		}
		m_bRandGravity = range != 0.0f ? true : false;
		if ( m_bRandGravity == true )
		{
			m_RandGravityRange = grMath::Abs( range ) * 0.5f;
		}
	}
	inline void SetSpeedRange( const float randomRange )
	{
		float range = randomRange;
		range = grMath::Clamp( range, 0.0f, grMath::Abs( range ) );
		if ( grMath::CmpFloat( 0.0f, range ) == true )
		{
			range = 0.0f;
		}
		m_bRandSpeed = ( range > 0.0f ) ? true : false;
		m_RandSpeedRange = ( m_bRandSpeed == true ) ? range * 0.5f : range;
	}
	inline void SetSpeedChangeRange( float randomRange )
	{
		float range = randomRange;
		range = grMath::Clamp( range, 0.0f, grMath::Abs( range ) );
		if ( grMath::CmpFloat( 0.0f, range ) == true )
		{
			range = 0.0f;
		}
		m_bRandSpeedChange = ( range > 0.0f ) ? true : false;
		m_RandSpeedChangeRange = ( m_bRandSpeedChange == true ) ? range * 0.5f : range;
	}

	//////////////////////////////////////////////////

	inline grV2f& GetDirection( void )
	{
		return m_ParticleBlueprint->Direction;
	}
	inline float GetSpeed( void ) const
	{
		return m_ParticleBlueprint->Speed;
	}
	inline float GetSpeedChange( void ) const
	{
		return m_ParticleBlueprint->SpeedChange;
	}

	inline float GetDirectionRange( void )
	{
		return m_RandDiviationInDeg * 2.0f;
	}
	inline float GetSpeedRange( void )
	{
		return m_RandSpeedRange * 2.0f;
	}
	inline float GetSpeedChangeRange( void )
	{
		return m_RandSpeedChangeRange * 2.0f;
	}

	//////////////////////////////////////////////////

	void Init ( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float m_SpawnsPerSec );

	//////////////////////////////////////////////////

private:

	void DeactivateParticle( void );
	void ActivateParticle( const float deltaT );
	void UpdateParticle( const float deltaT );

	//////////////////////////////////////////////////

	struct Particle
	{
		Particle( void )
		{
			Position = grV2f();
			Direction = grV2f();
			Gravity = grV2f();
			Speed = 0.0f;
			SpeedChange = 0.0f;
			LifeTime = 0.0f;
		}

		grV2f Position;
		grV2f Direction;
		grV2f Gravity;
		float Speed;
		float SpeedChange;
		float LifeTime;
	};

	//////////////////////////////////////////////////

	// TODO: Add color

	Particle* m_ParticleBlueprint;

	std::vector<Particle*>	m_VecParticles;

	grV2f					m_RandPosRange;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_RandDiviationInDeg,
							m_RandGravityRange,
							m_RandSpeedRange,
							m_RandSpeedChangeRange;

	uInt					m_ActiveParticles;

	bool					m_bRandPosition,
							m_bRandDirDiviation,
							m_bRandGravity,
							m_bRandSpeed,
							m_bRandSpeedChange;

	grRandom*				m_pRandGen;

};

#endif	// _H_GRPARTICLESYSTEM_
