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
		if ( direction.x < -1.0f || direction.x > 1.0f ||
			direction.y < -1.0f || direction.y > 1.0f )
		{
			dir.Normalize();
		}
		m_ParticleBlueprint->Direction = dir;
		SetSpread( randomRangeInDeg );
	}
	inline void SetRotation( const float degrees )
	{
		float deg = ( degrees > 360.0f ) ? degrees - 360.0f : ( degrees < 0.0f ) ? 360.0f - degrees : degrees;
		m_SysRotInDeg = ( grMath::CmpFloat( deg, grMath::Epsilon ) == true ) ? 0.0f : deg;
		float rad = m_SysRotInDeg * grMath::DegToRad;
		grV2f dir = grV2f( 0.0f, -1.0f );
		grMath::RotatePoint( &dir, rad );
		SetDirection( dir, GetSpread() );
	}
	inline void SetSpread( const float inDegrees = 0.0f )
	{
		float d = inDegrees;
		if ( inDegrees < 0.0f )
		{
			d = 360.0f + inDegrees;
		}
		d = grMath::Clamp( inDegrees, 0.0f, 360.0f );
		if ( grMath::CmpFloat( 0.0f, d ) == true )
		{
			d = 0.0f;
		}
		m_bRandSpread = ( d > 0.0f ) ? true : false;
		m_RandSpreadRange = ( m_bRandSpread == true ) ? d * 0.5f : d;
	}
	inline void SetGravity( const grV2f& rGravity, const float randomRange = 0.0f )
	{
		m_ParticleBlueprint->Gravity = rGravity;
		SetGravityRange( randomRange );
	}
	inline void	SetSpeed( const float speed, const float randomRange = 0.0f )
	{
		float vel = speed;
		m_ParticleBlueprint->Speed = grMath::Clamp( vel, 0.0f, grMath::Abs( vel ) );
		SetSpeedRange( randomRange );
	}
	inline void SetSpeedChange( const float speedChange, const float randomRange = 0.0f )
	{
		float change = speedChange;
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
	inline void SetGravityRange( const float range )
	{
		float r = range;
		if ( grMath::CmpFloat( 0.0f, r ) == true )
		{
			r = 0.0f;
		}
		m_bRandGravity = r != 0.0f ? true : false;
		if ( m_bRandGravity == true )
		{
			m_RandGravityRange = grMath::Abs( r ) * 0.5f;
		}
	}
	inline void SetSpeedRange( const float range )
	{
		float r = range;
		r = grMath::Clamp( r, 0.0f, grMath::Abs( r ) );
		if ( grMath::CmpFloat( 0.0f, r ) == true )
		{
			r = 0.0f;
		}
		m_bRandSpeed = ( r > 0.0f ) ? true : false;
		m_RandSpeedRange = ( m_bRandSpeed == true ) ? r * 0.5f : r;
	}
	inline void SetSpeedChangeRange( float range )
	{
		float r = range;
		r = grMath::Clamp( r, 0.0f, grMath::Abs( r ) );
		if ( grMath::CmpFloat( 0.0f, r ) == true )
		{
			r = 0.0f;
		}
		m_bRandSpeedChange = ( r > 0.0f ) ? true : false;
		m_RandSpeedChangeRange = ( m_bRandSpeedChange == true ) ? r * 0.5f : r;
	}

	//////////////////////////////////////////////////

	inline grV2f& GetPosition( void )
	{
		return m_ParticleBlueprint->Position;
	}
	inline grV2f& GetDirection( void )
	{
		return m_ParticleBlueprint->Direction;
	}
	inline float GetRotationInDeg( void )
	{
		return m_SysRotInDeg;
	}
	inline float GetSpread( void )
	{
		return m_RandSpreadRange * 2.0f;
	}
	inline float GetSpeed( void ) const
	{
		return m_ParticleBlueprint->Speed;
	}
	inline float GetSpeedChange( void ) const
	{
		return m_ParticleBlueprint->SpeedChange;
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

	Particle*				m_ParticleBlueprint;

	std::vector<Particle*>	m_VecParticles;

	grV2f					m_RandPosRange;

	float					m_SpawnsPerSec,
							m_SpawnInMilliSec,
							m_SpawnTimer;

	float					m_RandSpreadRange,
							m_RandGravityRange,
							m_RandSpeedRange,
							m_RandSpeedChangeRange;

	float					m_SysRotInDeg;

	uInt					m_ActiveParticles;

	bool					m_bRandPosition,
							m_bRandSpread,
							m_bRandGravity,
							m_bRandSpeed,
							m_bRandSpeedChange;

	grRandom*				m_pRandGen;

};

#endif	// _H_GRPARTICLESYSTEM_
