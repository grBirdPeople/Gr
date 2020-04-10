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

	// TODO: Add gravity

	inline void SetPosition( const grV2f& rPos, const grV2f& randomRange = grV2f() )
	{
		m_ParticleBlueprint->Position = rPos;
		SetPositionRange( randomRange );
	}
	inline void SetDirection( const grV2f& direction, const float randomRangeInDeg = 0.0f )
	{
		grV2f dir = direction;
		if ( direction.x < -1.0f || direction.x > 1.0f ||
			direction.y < -1.0f || direction.y > 1.0f )
		{
			dir.Normalize();
		}
		m_ParticleBlueprint->Direction = dir;
		SetDirectionRange( randomRangeInDeg );
	}
	inline void SetRotation( const float degrees )
	{
		// TODO: Makes all this better
		float deg = grMath::Clamp( degrees, -360.0f, 360.0f );
		if ( deg < 0.0f )
		{
			deg = 360.0f + deg;
		}
		m_SysRotInDeg = ( grMath::CmpFloat( deg, grMath::Epsilon ) == true ) ? 0.0f : deg;
		float rad = m_SysRotInDeg * grMath::DegToRad;
		grV2f dir = grV2f( 0.0f, -1.0f );
		grMath::RotatePoint( &dir, rad );
		SetDirection( dir, GetDirectionRange() );
	}
	inline void	SetGravity( const grV2f& gravityDir, const float randomRange )
	{
		m_ParticleBlueprint->Gravity = gravityDir;
		SetGravityRange( randomRange );
	}
	inline void	SetSpeed( const float speed, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint->Speed = grMath::Clamp( speed, 0.0f, grMath::Abs( speed ) );
		SetSpeedRange( randomRange );
	}
	inline void SetSpeedChange( const float speedChange, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint->SpeedChange = grMath::Clamp( speedChange, 0.0f, grMath::Abs( speedChange ) );
		SetSpeedChangeRange( randomRange );
	}
	inline void SetLifetime( const float lifetime, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint->LifeTime = grMath::Abs( lifetime );
		SetLifetimeRange( randomRange );
	}

	//////////////////////////////////////////////////

	// TODO: Probably make theese here private

	inline void SetPositionRange( const grV2f& range )
	{
		m_RandPosRange.x = grMath::Clamp( range.x, 0.0f, grMath::Abs( range.x ) );
		m_RandPosRange.y = grMath::Clamp( range.y, 0.0f, grMath::Abs( range.y ) );
		m_bRandPosition = ( grMath::CmpFloat( 0.0f, m_RandPosRange.x ) == true &&
							grMath::CmpFloat( 0.0f, m_RandPosRange.y ) == true ) ? false : true;
	}
	inline void SetDirectionRange( const float degrees )
	{
		float deg = grMath::Clamp( degrees, -360.0f, 360.0f );
		if ( degrees < 0.0f )
		{
			deg = 360.0f + deg;
		}
		m_SysRotInDeg = ( grMath::CmpFloat( deg, grMath::Epsilon ) == true ) ? 0.0f : deg;
		m_bRandDirection = ( m_SysRotInDeg > 0.0f ) ? true : false;
		m_RandDirectionRange = ( m_bRandDirection == true ) ? deg * 0.5f : deg;
	}
	inline void SetGravityRange( const float range )
	{
		m_bRandGravity = ( range != 0.0f ) ? true : false;
		m_RandGravityRange = range;
	}
	inline void SetSpeedRange( const float range )
	{
		m_RandSpeedRange = grMath::Clamp( range, 0.0f, m_ParticleBlueprint->Speed );
		m_bRandSpeed = ( grMath::CmpFloat( 0.0f, m_RandSpeedRange ) == true ) ? false : true;
	}
	inline void SetSpeedChangeRange( float range )
	{
		m_RandSpeedChangeRange = grMath::Clamp( range, 0.0f, m_ParticleBlueprint->Speed );
		m_bRandSpeedChange = ( grMath::CmpFloat( 0.0f, m_RandSpeedChangeRange ) == true ) ? false : true;
	}
	inline void SetLifetimeRange( float range )
	{
		m_RandLifeRange = grMath::Clamp( range, 0.0f, m_ParticleBlueprint->LifeTime );
		m_bRandLife = ( grMath::CmpFloat( 0.0f, m_ParticleBlueprint->LifeTime ) == true ) ? false : true;
	}

	//////////////////////////////////////////////////

	inline const grV2f& GetPosition( void )
	{
		return m_ParticleBlueprint->Position;
	}
	inline const grV2f& GetDirection( void )
	{
		return m_ParticleBlueprint->Direction;
	}
	inline const float GetRotationInDeg( void )
	{
		return m_SysRotInDeg;
	}
	inline const grV2f& GetGravity( void )
	{
		return m_ParticleBlueprint->Gravity;
	}
	inline const float GetSpeed( void ) const
	{
		return m_ParticleBlueprint->Speed;
	}
	inline const float GetSpeedChange( void ) const
	{
		return m_ParticleBlueprint->SpeedChange;
	}
	inline const float GetLifetime( void ) const
	{
		return m_ParticleBlueprint->LifeTime;
	}

	// TODO: Probably make theese here private too

	inline const grV2f& GetPositionRange( void )
	{
		return m_RandPosRange;
	}
	inline const float GetDirectionRange( void )
	{
		return m_RandDirectionRange * 2.0f;
	}
	inline const float GetGravityRange( void )
	{
		return m_RandGravityRange;
	}
	inline const float GetSpeedRange( void )
	{
		return m_RandSpeedRange;
	}
	inline const float GetSpeedChangeRange( void )
	{
		return m_RandSpeedChangeRange;
	}
	inline const float GetLifetimeRange( void )
	{
		return m_RandLifeRange;
	}

	//////////////////////////////////////////////////

	void Init( const grV2f& position, const grV2f& direction, const float velocity, const float lifetime, const float m_SpawnsPerSec );

	//////////////////////////////////////////////////

private:

	
	// Below functions causes pointer chasing
	void DeactivateParticle( void );
	void ActivateParticle( const float deltaT );
	void UpdateParticle( const float deltaT );

	//////////////////////////////////////////////////

	struct Particle
	{
		Particle( void )
		{
			Position = grV2f();
			Direction = grV2f( 0.0f, -1.0f );
			Gravity = grV2f();
			Speed = 50.0f;
			SpeedChange = 0.0f;
			LifeTime = 3.0f;
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

	Particle*				m_ParticleBlueprint;

	std::vector<Particle*>	m_VecParticles;

	grV2f					m_RandPosRange;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_RandDirectionRange,	// Degrees
							m_RandGravityRange,		// Degrees
							m_RandSpeedRange,
							m_RandSpeedChangeRange,
							m_RandLifeRange;		// Seconds

	float					m_SysRotInDeg;

	uInt					m_ActiveParticles;

	bool					m_bRandPosition,
							m_bRandDirection,
							m_bRandGravity,
							m_bRandSpeed,
							m_bRandSpeedChange,
							m_bRandLife;

	grRandom*				m_pRandGen;

};

#endif	// _H_GRPARTICLESYSTEM_
